
#include <github_api/github_api_qt.hpp>

#include "connection.hpp"


namespace GitHub { namespace QtBackend {

Api::Api(QNetworkAccessManager& manager, const QString& addr): m_manager(manager), m_addres(addr)
{

}


std::unique_ptr<cpp_restapi::IConnection> Api::connect()
{
    std::unique_ptr<cpp_restapi::IConnection> result(new Connection(m_manager, m_addres, ""));

    return result;
}


std::unique_ptr<cpp_restapi::IConnection> Api::connect(const std::string& token)
{
    std::unique_ptr<cpp_restapi::IConnection> result(new Connection(m_manager, m_addres, token.c_str()));

    return result;
}


std::string Api::address() const
{
    return m_addres.toStdString();
}

}}
