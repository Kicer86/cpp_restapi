
#ifndef CONNECTION_QT_HPP
#define CONNECTION_QT_HPP

#include <github_api/iconnection.hpp>


namespace GitHub { namespace CurlBackend {

    class Connection: public IConnection
    {
        public:
            Connection(const std::string& address, const std::string& token);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

            std::string get(const std::string &) override;

        private:
            const std::string m_address;
            const std::string m_token;
    };

}}

#endif
