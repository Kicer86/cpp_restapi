
#ifndef CONNECTION_BUILDER_HPP_INCLUDED
#define CONNECTION_BUILDER_HPP_INCLUDED

#include <map>
#include <memory>

#include <cpp_restapi/iconnection.hpp>


namespace cpp_restapi::GitHub
{
    class ConnectionBuilder
    {
        public:
            ConnectionBuilder()
                : m_address("https://api.github.com")
            {
            }

            ConnectionBuilder(const ConnectionBuilder &) = delete;

            ConnectionBuilder& setAddress(const std::string& address)
            {
                m_address = address;

                return *this;
            }

            ConnectionBuilder& setToken(const std::string& token)
            {
                m_headerEntries.emplace("Authorization", "token " + token);

                return *this;
            }

            template<typename CT, typename... Args>
            std::unique_ptr<IConnection> build(Args&&... args)
            {
                return std::make_unique<CT>(std::forward<Args>(args)..., m_address, m_headerEntries);
            }

        private:
            std::map<std::string, std::string> m_headerEntries;
            std::string m_address;
    };
}

#endif
