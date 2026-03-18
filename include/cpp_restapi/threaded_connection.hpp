#ifndef THREADED_CONNECTION_HPP_INCLUDED
#define THREADED_CONNECTION_HPP_INCLUDED

#include "base_connection.hpp"
#include "cpp_restapi_export.h"


namespace cpp_restapi
{
    /**
     * @brief BaseConnection subclass that implements fetch(url, cb) via a detached std::thread.
     *
     * Non-Qt backends (CppHttplib, Curl) inherit from this class to get a
     * thread-based async fetch() without changes to their own code. The callback
     * is invoked from the worker thread; callers are responsible for any required
     * synchronisation.
     */
    class CPP_RESTAPI_EXPORT ThreadedConnection : public BaseConnection
    {
    public:
        using BaseConnection::BaseConnection;
        using BaseConnection::fetch;

        void fetch(const std::string& url,
                   FetchCallback onSuccess,
                   ErrorCallback onError = {}) override;
    };
}

#endif
