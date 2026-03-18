#ifndef SSE_CONNECTION_CPP_HTTPLIB_HPP_INCLUDED
#define SSE_CONNECTION_CPP_HTTPLIB_HPP_INCLUDED

#include <atomic>
#include <map>
#include <string>
#include <thread>

#include <cpp_restapi/isse_connection.hpp>
#include <cpp_restapi/sse_parser.hpp>
#include "cpp_restapi_export.h"


namespace cpp_restapi::CppHttplibBackend
{
    /**
     * @brief cpp-httplib-based SSE connection
     *
     * Uses cpp-httplib to establish a streaming HTTP connection
     * to an SSE endpoint and delivers parsed events via callback.
     * The subscribe() call is non-blocking — streaming runs on
     * an internal thread. Use close() to stop.
     */
    class CPP_RESTAPI_EXPORT SseConnection: public cpp_restapi::ISseConnection
    {
    public:
        SseConnection(const std::string& address,
                      const std::map<std::string, std::string>& headerEntries);

        ~SseConnection() override;

        SseConnection(const SseConnection&) = delete;
        SseConnection& operator=(const SseConnection&) = delete;

        void subscribe(const std::string& request, EventCallback callback) override;
        void close() override;

    private:
        void closeImpl();

        std::string m_address;
        std::map<std::string, std::string> m_headerEntries;
        std::thread m_thread;
        std::atomic<bool> m_running{false};

        void run(const std::string& request, EventCallback callback);
    };
}

#endif
