
#ifndef CPP_HTTPLIB_CONNECTION_HPP_INCLUDED
#define CPP_HTTPLIB_CONNECTION_HPP_INCLUDED

#include "base_connection.hpp"
#include "cpp_restapi_export.h"


namespace cpp_restapi::CppHttplibBackend
{
    class CPP_RESTAPI_EXPORT Connection: public BaseConnection
    {
        public:
            Connection(const std::string& address, const std::map<std::string, std::string>& headerEntries);
            Connection(const Connection &) = delete;

            Connection& operator=(const Connection &) = delete;

            std::pair<std::string, std::string> fetchPage(const std::string& request) override;
    };
}

#endif
