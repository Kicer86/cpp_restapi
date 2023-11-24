
#ifndef CONNECTION_QT_HPP
#define CONNECTION_QT_HPP

#include "base_connection.hpp"


namespace cpp_restapi { namespace CurlBackend {

    class Connection: public BaseConnection
    {
        public:
            [[deprecated]] Connection(const std::string& address, const std::string& token);
            Connection(const std::string& address, const std::map<std::string, std::string>& token);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

            std::pair<std::string, std::string> fetchPage(const std::string& request) override;
    };

}}

#endif
