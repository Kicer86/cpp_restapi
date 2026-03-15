#ifndef SSE_CONNECTION_CPP_HTTPLIB_HPP_INCLUDED
#define SSE_CONNECTION_CPP_HTTPLIB_HPP_INCLUDED

#include <map>
#include <string>

#include "isse_connection.hpp"
#include "sse_parser.hpp"
#include "cpp_restapi_export.h"


namespace cpp_restapi::CppHttplibBackend
{
    /**
     * @brief cpp-httplib-based SSE connection
     *
     * Uses cpp-httplib to establish a streaming HTTP connection
     * to an SSE endpoint and delivers parsed events via callback.
     * The subscribe() call blocks until the connection is closed
     * (either by the server or via close()).
     */
    class CPP_RESTAPI_EXPORT SseConnection: public cpp_restapi::ISseConnection
    {
    public:
        SseConnection(const std::string& address,
                      const std::map<std::string, std::string>& headerEntries);

        SseConnection(const SseConnection&) = delete;
        SseConnection& operator=(const SseConnection&) = delete;

        void subscribe(const std::string& request, EventCallback callback) override;
        void close() override;

    private:
        std::string m_address;
        std::map<std::string, std::string> m_headerEntries;
        SseParser m_parser;
        EventCallback m_callback;
        bool m_running = false;
    };
}

#endif
