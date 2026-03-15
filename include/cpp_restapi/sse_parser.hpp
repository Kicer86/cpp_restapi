#ifndef SSE_PARSER_HPP_INCLUDED
#define SSE_PARSER_HPP_INCLUDED

#include <string>
#include <vector>

#include "cpp_restapi_export.h"
#include "sse_event.hpp"


namespace cpp_restapi
{
    /**
     * @brief Incremental parser for Server-Sent Events stream
     *
     * Accumulates raw data from an SSE stream and extracts
     * complete events as they become available.
     * Implements parsing rules from the SSE specification:
     * https://html.spec.whatwg.org/multipage/server-sent-events.html
     */
    class CPP_RESTAPI_EXPORT SseParser
    {
    public:
        /**
         * @brief Feed raw data from the SSE stream
         * @param chunk raw bytes received from the HTTP response
         * @return vector of complete events parsed from the accumulated data
         */
        std::vector<SseEvent> feed(const std::string& chunk);

    private:
        std::string m_buffer;

        static SseEvent parseBlock(const std::string& block);
    };
}

#endif
