
#ifndef GITHUBAPI_HPP
#define GITHUBAPI_HPP

// Based on:
// https://developer.github.com/guides/getting-started/
// https://developer.github.com/v3/

#include <memory>

#include <QString>

#include "iconnection.hpp"
#include "github_api_export.h"

class QNetworkAccessManager;

class GITHUB_API_EXPORT GitHubApi
{
    public:
        GitHubApi(QNetworkAccessManager &, const QString& addr = "https://api.github.com");
        GitHubApi(const GitHubApi &) = delete;
        ~GitHubApi();

        std::unique_ptr<GitHub::IConnection> connect();                        // anonymous access
        std::unique_ptr<GitHub::IConnection> connect(const QString& token);

        GitHubApi& operator=(const GitHubApi &) = delete;

    private:
        QNetworkAccessManager& m_manager;
        QString m_addres;
};

#endif // GITHUBAPI_HPP

