
#include <github_api/github_api.hpp>

#include "connection.hpp"

GitHubApi::GitHubApi(QNetworkAccessManager& manager, const QString& addr): m_manager(manager), m_addres(addr)
{

}


GitHubApi::~GitHubApi()
{

}


std::unique_ptr<GitHub::IConnection> GitHubApi::connect()
{
    std::unique_ptr<GitHub::IConnection> result(new GitHub::Connection(m_manager, m_addres, ""));

    return result;
}


std::unique_ptr<GitHub::IConnection> GitHubApi::connect(const QString& token)
{
    std::unique_ptr<GitHub::IConnection> result(new GitHub::Connection(m_manager, m_addres, token));

    return result;
}


