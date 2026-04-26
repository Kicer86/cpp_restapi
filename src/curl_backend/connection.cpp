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
#include <utility>
#include <curl/curl.h>

#include <cpp_restapi/create_curl_connection.hpp>
#include <cpp_restapi/threaded_connection.hpp>
#include "curl_sse_connection.hpp"


namespace
{
    class CurlConnection final: public cpp_restapi::ThreadedConnection
    {
        public:
            CurlConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries);
            CurlConnection(const CurlConnection &) = delete;

            ~CurlConnection() override;

            CurlConnection& operator=(const CurlConnection &) = delete;

            cpp_restapi::Response fetchPage(const std::string& request) override;
            std::unique_ptr<cpp_restapi::ISseConnection> subscribe(const std::string& request, cpp_restapi::IConnection::EventCallback callback) override;
    };


    CurlConnection::CurlConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
        : cpp_restapi::ThreadedConnection(address, headerEntries)
    {

    }


    CurlConnection::~CurlConnection()
    {

    }


    cpp_restapi::Response CurlConnection::fetchPage(const std::string& page)
    {
        std::string body;
        std::string headers;
        int statusCode = 0;

        CURL* curl = curl_easy_init();

        if (curl)
        {
            curl_slist *list = nullptr;

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

            curl_easy_setopt(curl, CURLOPT_URL, page.c_str());
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headers);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "cpp_restapi/2.0");

            const auto header_entries = getHeaderEntries();

            for(const auto& [k, v]: header_entries)
            {
                const std::string entry = k + ": " + v;
                list = curl_slist_append(list, entry.c_str());
            }

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

            if (curl_easy_perform(curl) == CURLE_OK)
            {
                long code = 0;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
                statusCode = static_cast<int>(code);
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(list);
        }

        return {std::move(body), std::move(headers), statusCode};
    }


    std::unique_ptr<cpp_restapi::ISseConnection> CurlConnection::subscribe(const std::string& request, cpp_restapi::IConnection::EventCallback callback)
    {
        auto sse = std::make_unique<cpp_restapi::CurlBackend::SseConnection>(address(), getHeaderEntries());
        sse->subscribe(request, std::move(callback));
        return sse;
    }
}


namespace cpp_restapi
{
    std::unique_ptr<IConnection> createCurlConnection(
        const std::string& address,
        const std::map<std::string, std::string>& headerEntries)
    {
        return std::make_unique<CurlConnection>(address, headerEntries);
    }
}
