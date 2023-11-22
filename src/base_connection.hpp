
#ifndef BASE_CONNECTION_HPP_INCLUDED
#define BASE_CONNECTION_HPP_INCLUDED

#include <cpp_restapi/iconnection.hpp>

namespace GitHub
{
    class BaseConnection: public cpp_restapi::IConnection
    {
    public:
        explicit BaseConnection(const std::string& address, const std::string& token);

        std::string get(const std::string &) final;
        virtual std::pair<std::string, std::string> fetchPage(const std::string& request) = 0;

    protected:
        const std::string& address() const;
        const std::string& token() const;

    private:
        const std::string m_address;
        const std::string m_token;
    };
}

#endif
