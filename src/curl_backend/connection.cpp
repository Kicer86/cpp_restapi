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

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <curl/curl.h>

#include "connection.hpp"
#include "header_utils.hpp"


namespace
{
    /**
     * @brief The method takes list of multiple response of the json object
     *        returned from the git api as one string. and then formats it to,
     *        a single string equivalent of json object string.
     *
     *        This method makes it possible to save the output as a json
     *        file if need be, without having to do anything else.
     *
     *        git api returns data in json formats. making it easier for data
     *         abstraction
     *
     * @param result a combination of response returned from the git api
     */
    std::string format_string(const std::string& result)
    {
        std::string formattedPack;

        int open_br = 0;
        int close_br = 0;

        for (std::string::size_type i=0; i < result.size(); i++)
        {
            if (result[i] == '[' && result[i+1] == '{')
            {
                open_br = 1;
                continue;
            }
            if (open_br == 1)
            {
                open_br = 0;
            }
            if (result[i+1] == ']' && result[i-1] == '}' )
            {
                close_br = 1;
                continue;
            }
            if (close_br == 1)
            {
                formattedPack += "},";
                close_br = 0;
                continue;
            }

            formattedPack += result[i];
        }

        return formattedPack;
    }

    /**
     * @brief The original get method used for querying a request
     *
     * @param request request link
     * @param m_token github auth token
     * @return std::pair<std::string, std::string> returns response body and response headers
     */
    std::pair<std::string, std::string> performQuery(const std::string& request, const std::string& m_token)
    {
        std::string result;
        std::string header_links;

        CURL* curl = curl_easy_init();

        if (curl)
        {
            curl_slist *list = nullptr;
            std::string authorization;

            typedef size_t (*WriteCallback)(char *ptr, size_t size, size_t nmemb, void *userdata);
            typedef size_t (*HeaderCallback)(char *buffer, size_t size, size_t nitems, void *userdata);
            WriteCallback write_callback = [](char *ptr, size_t size, size_t nmemb, void* result_raw)
            {
                assert(size == 1);

                std::string& result = *static_cast<std::string*>(result_raw);
                std::copy(ptr, ((ptr + nmemb)), std::back_inserter(result));

                return nmemb;
            };
            /**
             * @brief This is used as a callback that receives
             *        header data, the header data is used for pagination.
             *        see the following link for more info
             *        https://curl.se/libcurl/c/CURLOPT_HEADERFUNCTION.html
             */
            HeaderCallback header_callback = [](char *buffer, size_t size,size_t nitems, void *userdata)
            {
                std::string& header_links = *static_cast<std::string*>(userdata);
                std::copy(buffer, buffer+nitems, std::back_inserter(header_links));

                return (size * nitems);
            };

            // const std::string full_addr = m_address + "/" + request;

            curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_links);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");

            if (m_token.empty() == false)
            {
                authorization = std::string("Authorization: token ") + m_token;
                list = curl_slist_append(list, authorization.c_str());

                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
            }

            curl_easy_perform(curl);

            curl_slist_free_all(list);
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
        return std::make_pair(result, header_links);
    }
}


GitHub::CurlBackend::Connection::Connection(const std::string& address,
                                             const std::string& token)
    : m_address(address)
    , m_token(token)
{

}


GitHub::CurlBackend::Connection::~Connection()
{

}


std::string GitHub::CurlBackend::Connection::get(const std::string& request)
{
    std::string output;

    const std::string full_addr = m_address + "/" + request;
    const std::pair<std::string, std::string> response = performQuery(full_addr, m_token); // initial execution
    const std::string& result = response.first;
    const std::string& header_links = response.second;

    // checks if the link keyword exists in header response
    std::string key_word = "link:";
    std::regex re("link:");
    std::smatch match;
    regex_search(header_links, match, re);

    const bool paginate = std::regex_search(header_links, match, re);
    if (paginate)
    {
        // next link url and the number of pages
        std::pair <std::string, int> pagination = HeaderUtils::checkPaginationLInk(header_links);
        std::string next_link = pagination.first;
        int no_page = pagination.second;

        // make a subsequent request to performQuery method and append
        // data to result pointer
        for(int pageNo = 0; pageNo < no_page; pageNo++)
        {
            next_link = next_link.substr(0, next_link.size()-1);
            next_link +=  std::to_string(pageNo + 1);
            performQuery(next_link, m_token);
        }
        // format the string data to meet the json format
        output = format_string(result);

        // removing the trailing comma, to avoid error when converting to json format
        int last_comma = output.length()-1;
        output.erase(last_comma, 1);
    }
    else
    {
        output = result;
    }

    output = "[" + output + "]";

    return output;
}
