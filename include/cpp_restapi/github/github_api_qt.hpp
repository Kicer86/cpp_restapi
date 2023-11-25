
#ifndef GITHUBAPI_QT_HPP
#define GITHUBAPI_QT_HPP

// Based on:
// https://developer.github.com/guides/getting-started/
// https://developer.github.com/v3/

#include <memory>

#include <QString>

#include "cpp_restapi/iconnection.hpp"
#include "github_api_base.hpp"
#include "github_api_export.h"

class QNetworkAccessManager;

namespace cpp_restapi::GitHub::QtBackend
{

    /**
     * @brief Class for establishing connection with GitHub api with Qt's QNetworkAccessManager
     */
    class GITHUB_API_EXPORT Api: public cpp_restapi::GitHubBase
    {
        public:
            Api(QNetworkAccessManager &, const QString& addr = "https://api.github.com");
            Api(const Api &) = delete;

            std::unique_ptr<cpp_restapi::IConnection> connect(const std::string& token = "") override;

            Api& operator=(const Api &) = delete;

        private:
            QNetworkAccessManager& m_manager;
    };

}

#endif
