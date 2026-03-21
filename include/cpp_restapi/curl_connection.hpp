
#ifndef CONNECTION_QT_HPP
#define CONNECTION_QT_HPP

#include "threaded_connection.hpp"
#include "cpp_restapi_export.h"


namespace cpp_restapi::CurlBackend
{
    class CPP_RESTAPI_EXPORT Connection: public ThreadedConnection
    {
        public:
            Connection(const std::string& address, const std::map<std::string, std::string>& headerEntries);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

            Response fetchPage(const std::string& request) override;
            std::unique_ptr<ISseConnection> subscribe(const std::string& request, EventCallback callback) override;
    };
}

#endif
