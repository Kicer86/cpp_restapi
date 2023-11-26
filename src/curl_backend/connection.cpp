/**
 * @author Kicer86@gmail.com
 * @brief The initial file and api is return by Michał Walenciak.
 *        Kevin Mukuna(mukuna.kevin@gmail.com) modified the file
 *        to allow pagination.
 */

#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>
#include <curl/curl.h>

#include <cpp_restapi/curl_connection.hpp>


namespace cpp_restapi::CurlBackend
{
    Connection::Connection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
        : BaseConnection(address, headerEntries)
    {

    }


    Connection::~Connection()
    {

    }


    std::pair<std::string, std::string> cpp_restapi::CurlBackend::Connection::fetchPage(const std::string& page)
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

            curl_easy_setopt(curl, CURLOPT_URL, page.c_str());
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_links);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");

            const auto header_entries = getHeaderEntries();

            for(const auto& [k, v]: header_entries)
            {
                const std::string entry = k + ": " + v;
                list = curl_slist_append(list, entry.c_str());
            }

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

            curl_easy_perform(curl);

            curl_slist_free_all(list);
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
        return std::make_pair(result, header_links);
    }
}
