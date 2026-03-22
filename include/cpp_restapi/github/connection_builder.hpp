
#ifndef CONNECTION_BUILDER_HPP_INCLUDED
#define CONNECTION_BUILDER_HPP_INCLUDED

#include <functional>
#include <map>
#include <memory>

#include <cpp_restapi/iconnection.hpp>


namespace cpp_restapi::GitHub
{
    /**
     * @brief Connection build for GitHub api
     */

    class ConnectionBuilder
    {
        public:
            ConnectionBuilder()
                : m_address("https://api.github.com")
            {
            }

            ConnectionBuilder(const ConnectionBuilder &) = delete;

            /**
             * @brief change github api address.
             * @param address api address
             *
             * Default value is https://api.github.com
             * Api address should not be change in normal conditions. It is used for testing.
             */
            ConnectionBuilder& setAddress(const std::string& address)
            {
                m_address = address;

                return *this;
            }

            /**
             * @brief set token used for authorization
             * @param token token to be used for authorization
             */
            ConnectionBuilder& setToken(const std::string& token)
            {
                m_headerEntries.emplace("Authorization", "token " + token);

                return *this;
            }

            /**
             * @brief build @ref cpp_restapi::IConnection object using a factory function
             * @param factory callable that creates a connection (e.g. @ref cpp_restapi::createCurlConnection)
             * @param args extra arguments passed to the factory before address and headers
             *
             * Usage:
             * @code
             * auto conn = builder.build(cpp_restapi::createCurlConnection);
             * auto conn = builder.build(cpp_restapi::createQtConnection, networkManager);
             * @endcode
             */
            template<typename Factory, typename... Args>
            std::unique_ptr<IConnection> build(Factory&& factory, Args&&... args)
            {
                return std::invoke(std::forward<Factory>(factory),
                                   std::forward<Args>(args)..., m_address, m_headerEntries);
            }

            /**
             * @brief build @ref cpp_restapi::IConnection object
             * @tparam CT connection type (Qt or Curl backend). @ref cpp_restapi::CurlBackend::Connection or @ref cpp_restapi::QtBackend::Connection
             * @param args backend specific arguments to be passed to connection.
             * @deprecated Use build(factory, args...) with a factory function instead (e.g. build(createCurlConnection))
             */
            template<typename CT, typename... Args>
            [[deprecated("Use build(factory, args...) with a factory function instead")]]
            std::shared_ptr<IConnection> build(Args&&... args)
            {
                return std::make_shared<CT>(std::forward<Args>(args)..., m_address, m_headerEntries);
            }

        private:
            std::map<std::string, std::string> m_headerEntries;
            std::string m_address;
    };
}

#endif
