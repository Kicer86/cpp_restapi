#ifndef ISSE_CONNECTION_HPP_INCLUDED
#define ISSE_CONNECTION_HPP_INCLUDED

#include <functional>
#include <string>

#include "sse_event.hpp"

namespace cpp_restapi
{
    /**
     * @brief Interface for SSE (Server-Sent Events) connections
     *
     * Provides streaming subscription to an SSE endpoint.
     * Unlike IConnection::get() which returns a complete response,
     * SSE delivers events continuously via a callback.
     */
    struct ISseConnection
    {
        using EventCallback = std::function<void(const SseEvent&)>;

        virtual ~ISseConnection() = default;

        /**
         * @brief Subscribe to an SSE endpoint
         * @param request API endpoint path (e.g. "events" or "api/v1/events")
         * @param callback function called for each received SSE event
         */
        virtual void subscribe(const std::string& request, EventCallback callback) = 0;

        /**
         * @brief Stop receiving events and close the SSE connection
         */
        virtual void close() = 0;
    };
}

#endif
