
#ifndef CONNECTION_QT_HPP
#define CONNECTION_QT_HPP

#include "base_connection.hpp"
#include "cpp_restapi_export.h"


namespace cpp_restapi { namespace CurlBackend {

    class CPP_RESTAPI_EXPORT Connection: public BaseConnection
    {
        public:
            Connection(const std::string& address, const std::map<std::string, std::string>& headerEntries);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

            std::pair<std::string, std::string> fetchPage(const std::string& request) override;
    };

}}

#endif
