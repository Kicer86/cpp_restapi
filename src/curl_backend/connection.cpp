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
#include <string>
#include <curl/curl.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "connection.hpp"
#include "header_utils.hpp"


namespace
{
    /**
     * @brief Method used for querying a request
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

    void update(Json::Value& a, const Json::Value& b)
    {
        if (a.isArray() && b.isArray())
            for (const auto& entry : b)
                a.append(entry);

        if (a.isObject() && b.isObject())
        {
            for (const auto& key : b.getMemberNames())
                if (a[key].isObject())
                    update(a[key], b[key]);
                else
                    a[key] = b[key];
        }
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
    std::string nextPage = m_address + "/" + request;

    Json::Value output;

    do
    {
        Json::Reader reader;

        const std::pair<std::string, std::string> response = performQuery(nextPage, m_token);
        const std::string& header_links = response.second;

        Json::Value partialOutput;
        reader.parse(response.first, partialOutput);

        if (output.isNull())
            output.swap(partialOutput);
        else
            update(output, partialOutput);

        nextPage = HeaderUtils::getNextPageUrl(header_links);
    }
    while (nextPage.empty() == false);

    Json::FastWriter fastWriter;
    const std::string strOutput = fastWriter.write(output);

    return strOutput;
}
