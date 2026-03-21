#ifndef THREADED_CONNECTION_HPP_INCLUDED
#define THREADED_CONNECTION_HPP_INCLUDED

#include <condition_variable>
#include <mutex>

#include "base_connection.hpp"
#include "cpp_restapi_export.h"


namespace cpp_restapi
{
    /**
     * @brief BaseConnection subclass that implements fetch(url, cb) via a background std::thread.
     *
     * Non-Qt backends (CppHttplib, Curl) inherit from this class to get a
     * thread-based async fetch() without changes to their own code. The callback
     * is invoked from the worker thread; callers are responsible for any required
     * synchronisation.
     *
     * The destructor blocks until all in-flight requests complete, so it is
     * always safe to destroy the connection object.
     */
    class CPP_RESTAPI_EXPORT ThreadedConnection : public BaseConnection
    {
    public:
        using BaseConnection::BaseConnection;
        using BaseConnection::fetch;

        ~ThreadedConnection() override;

        void fetch(const std::string& url,
                   FetchCallback onSuccess,
                   ErrorCallback onError = {}) override;

    private:
        std::mutex m_mutex;
        std::condition_variable m_cv;
        unsigned m_activeCount = 0;
    };
}

#endif
