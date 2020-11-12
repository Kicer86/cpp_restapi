
#include <github_api/github_api.hpp>

#include "connection.hpp"


namespace GitHub { namespace QtBackend {

Api::Api(QNetworkAccessManager& manager, const QString& addr): m_manager(manager), m_addres(addr)
{

}


std::unique_ptr<GitHub::IConnection> Api::connect()
{
    std::unique_ptr<GitHub::IConnection> result(new Connection(m_manager, m_addres, ""));

    return result;
}


std::unique_ptr<GitHub::IConnection> Api::connect(const QString& token)
{
    std::unique_ptr<GitHub::IConnection> result(new Connection(m_manager, m_addres, token));

    return result;
}

}}
