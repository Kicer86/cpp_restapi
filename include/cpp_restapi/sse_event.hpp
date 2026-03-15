#ifndef SSE_EVENT_HPP_INCLUDED
#define SSE_EVENT_HPP_INCLUDED

#include <string>

namespace cpp_restapi
{
    /**
     * @brief Represents a single Server-Sent Event
     *
     * Fields follow the SSE specification:
     * https://html.spec.whatwg.org/multipage/server-sent-events.html
     */
    struct SseEvent
    {
        std::string event;    ///< Event type (from "event:" field, empty if not set)
        std::string data;     ///< Event data (from "data:" field(s), joined with newlines)
        std::string id;       ///< Last event ID (from "id:" field)
        int retry = -1;       ///< Reconnection time in ms (from "retry:" field, -1 if not set)
    };
}

#endif
