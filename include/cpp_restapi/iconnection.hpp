
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <functional>
#include <memory>
#include <string>

#include "sse_event.hpp"

namespace cpp_restapi
{
    struct ISseConnection;

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
