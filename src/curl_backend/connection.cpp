/****************************************************************
 * @file connection.cpp
 * @author Kicer86@gmail.com 
 * @brief The initial file and api is return by Michał Walenciak. 
 *        The script below makes curl request to github rest api v3 
 *        and returns a response.
 *        Kevin Mukuna(mukuna.kevin@gmail.com) modified the file
 *        to allow pagination.
 * @version 0.1
 * @date 2022-05-25
 * 
 * @copyright Copyright (c) 2020 Michał Walenciak
 ****************************************************************/
#include <iterator>
#include <cassert>
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>  // for copy
#include <utility>  // for pair
#include <string>  // for string
#include "connection.hpp"

std::string finalResult;  // combination of result

GitHub::CurlBackend::Connection::Connection(const std::string& address,
                                             const std::string& token)
    : m_address(address)
    , m_token(token) {
}


GitHub::CurlBackend::Connection::~Connection() {
}

std::pair <std::string, int> checkPaginationLInk(const std::string& header);
/**
 * @brief the method takes in a http header and returns 
 *        the next link used for pagination and the 
 *        number of pages 
 * 
 * @param header header data from the return data from 
 *               the api call 
 * @return std::pair <std::string, int> the last link 
 *         and the number of page 
 */
std::pair <std::string, int> checkPaginationLInk(const std::string& header) {
    std::istringstream header_(header);
    std::string line;
    std::string link;
    int numberOfPage;
    // iterating through each line of string
    while (std::getline(header_, line)) {
        std::istringstream eachLine(line);
        // itereating through each word in line, words are seperate by space
        bool found = false;
        while (eachLine && !found) {
            std::string word;
            eachLine >> word;
            if (word == "rel=\"next\",") {
                eachLine >> word;
                for (int i = 0; i < word.size(); i++) {
                    if (word[i] == '<') {
                        continue;
                    } else if (word[i] == '>') {
                        numberOfPage = word[i-1] - '0';
                        break;
                    } else {
                        link = std::string(link)+word[i];
                    }
                }
                found = true;
            }
        }
    }
    return std::make_pair(link, numberOfPage);
}
std::string GitHub::CurlBackend::Connection::get(const std::string& request) {
    std::string result;
    std::string header_links;
    int pageNo = 1;
    CURL* curl = curl_easy_init();

    if (curl) {
        curl_slist *list = nullptr;
        std::string authorization;

        typedef size_t (*WriteCallback)
            (char *ptr, size_t size, size_t nmemb, void *userdata);
        typedef size_t (*HeaderCallback)
            (char *buffer, size_t size, size_t nitems, void *userdata);

        WriteCallback write_callback = [](char *ptr, size_t size,
                                size_t nmemb, void* result_raw) {
            assert(size == 1);

            std::string& result = *static_cast<std::string*>(result_raw);
            std::copy(ptr, ptr + nmemb, std::back_inserter(result));

            return nmemb;
        };
        /**
         * @brief This is used as a callback that receives 
         *        header data, the header data is used for pagination. 
         *        see the following link for more info 
         *        https://curl.se/libcurl/c/CURLOPT_HEADERFUNCTION.html
         */
        HeaderCallback header_callback = [](char *buffer, size_t size,
                            size_t nitems, void *userdata){
            std::string& header_links = *static_cast<std::string*>(userdata);
            std::copy(buffer, buffer+nitems, std::back_inserter(header_links));
            return (size * nitems);
        };

        const std::string full_addr = m_address + "/" + request;

        /**
         * @brief this is a lambda function. This gives the api flexibility
         *        to acces the next page by calling this function as 
         *        many time as the number of pages by passing in the next link
         *        as oposed to running it once and only getting the first page,
         *        
         *        a typical example of this would be, let say a repo has 
         *        thousands of pull request. github v3 api only returns 100 
         *        pull request per request. this method will allow you to 
         *        get all pull request request through pagination.
         */
        auto execute = [&](std::string endpoint){
            curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_links);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");

            if (m_token.empty() == false) {
                authorization = std::string("Authorization: token ") + m_token;
                list = curl_slist_append(list, authorization.c_str());

                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
            }
            curl_easy_perform(curl);
        };

        /**
         * @brief merges current page response with all other previous response.
         * previous response could be one or more
         */
        auto mergeResult = [&](std::string prev_response, std::string cur_link){
            execute(cur_link);
            if (prev_response[0] == '[') {
                prev_response =
                    prev_response.substr(1, prev_response.size() - 1);
                result = result.substr(1, result.size() - 1);
                finalResult = "[" + prev_response + "," + result + "]";
            } else if (prev_response[0] == '{') {
                prev_response =
                    prev_response.substr(1, prev_response.size() - 1);
                result = result.substr(1, result.size() - 1);
                finalResult = "{" + prev_response + ',' + result + "}";
            } else {
                std::cout << "The data type used is unknown" << std::endl;
            }
        };

        bool paginate = true;
        execute(full_addr);  // initial execution

        // check if link contained in the header response
        if (header_links.find("link") != std::string::npos) {
            std::cout << "yes" << std::endl;
            paginate = true;
        } else {
            std::cout << "no" << std::endl;
            finalResult = result;
            paginate = false;
        }
        if (paginate) {
            std::pair <std::string, int> pagination =
                                checkPaginationLInk(header_links);
            int no_page = pagination.second;
            std::string next_link = pagination.first;

            if (finalResult.empty()) {
                std::cout << " it empty" << std::endl;
            }

            while (pageNo < no_page) {
                pageNo++;
                next_link = next_link.substr(0, next_link.size()-1);
                next_link +=  std::to_string(pageNo);
                result = (finalResult.empty()) ? result : finalResult;
                std::cout << next_link << std::endl;
                mergeResult(result, next_link);
            }
            paginate = false;
        }
        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
    }
    return finalResult;
}
