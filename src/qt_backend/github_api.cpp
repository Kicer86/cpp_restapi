
#include <cpp_restapi/github/github_api_qt.hpp>
#include <cpp_restapi/qt_connection.hpp>


namespace cpp_restapi::GitHub::QtBackend
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
