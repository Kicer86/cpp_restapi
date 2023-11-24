
#include <github_api/github_api_qt.hpp>

#include "connection.hpp"


namespace GitHub::QtBackend
{

Api::Api(QNetworkAccessManager& manager, const QString& addr)
    : cpp_restapi::GitHubBase(addr.toStdString())
    , m_manager(manager)
{

}


std::unique_ptr<cpp_restapi::IConnection> Api::connect(const std::string& token)
{
    std::map<std::string, std::string> headerEntries;

    if (token.empty() == false)
        headerEntries.emplace("Authorization", "token " + token);

    return std::make_unique<cpp_restapi::QtBackend::Connection>(m_manager, address(), headerEntries);
}

}
