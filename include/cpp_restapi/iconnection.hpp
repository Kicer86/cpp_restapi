
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ipagination_strategy.hpp"
#include "isse_connection.hpp"
#include "sse_event.hpp"

namespace cpp_restapi
{

    /**
     * @brief HTTP response containing body and raw headers
     */
    struct Response
    {
        std::string body;
        std::string headers;
    };

    /**
     * @brief Interface representing connection with rest api server
     */
    struct IConnection
    {
        public:
            using EventCallback = std::function<void(const SseEvent&)>;

            virtual ~IConnection() = default;

            /**
             * @brief perform a request to api
             * @param request api request. For example "users/SomeUserName/repos"
             * @return api response in json format
             */
            virtual std::string get(const std::string& request) = 0;

            /**
             * @brief Perform a single HTTP request
             * @param request relative API path (e.g. "api/v1/disks")
             * @return response body
             */
            std::string fetch(const std::string& request)
            {
                return fetchResponse(url() + "/" + request).body;
            }

            /**
             * @brief Perform requests with automatic pagination
             * @param request relative API path
             * @param strategy pagination strategy defining how to discover next page and merge results
             * @return merged response body from all pages
             */
            std::string fetch(const std::string& request, IPaginationStrategy& strategy)
            {
                std::string nextUrl = url() + "/" + request;
                std::vector<std::string> pages;

                do
                {
                    auto resp = fetchResponse(nextUrl);
                    pages.push_back(std::move(resp.body));
                    nextUrl = strategy.nextPageUrl(resp.headers);
                }
                while (!nextUrl.empty());

                return strategy.merge(pages);
            }

            /**
             * @brief Perform a single HTTP request returning full response
             * @param url full URL to fetch
             * @return response with body and raw headers
             */
            virtual Response fetchResponse(const std::string& url) = 0;

            /**
             * @brief return API url
             */
            virtual const std::string& url() const = 0;

            /**
             * @brief Subscribe to an SSE endpoint
             * @param request API endpoint path (e.g. "events" or "api/v1/events")
             * @param callback function called for each received SSE event
             * @return SSE connection handle; use its close() method to stop
             */
            virtual std::unique_ptr<ISseConnection> subscribe(const std::string& request, EventCallback callback) = 0;
    };
}

#endif
