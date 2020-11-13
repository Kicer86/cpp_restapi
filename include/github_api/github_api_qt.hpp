
#ifndef GITHUBAPI_QT_HPP
#define GITHUBAPI_QT_HPP

// Based on:
// https://developer.github.com/guides/getting-started/
// https://developer.github.com/v3/

#include <memory>

#include <QString>

#include "iconnection.hpp"
#include "github_api_export.h"

class QNetworkAccessManager;

namespace GitHub { namespace QtBackend {

    class GITHUB_API_EXPORT Api
    {
        public:
            Api(QNetworkAccessManager &, const QString& addr = "https://api.github.com");
            Api(const Api &) = delete;

            std::unique_ptr<GitHub::IConnection> connect();                        // anonymous access
            std::unique_ptr<GitHub::IConnection> connect(const QString& token);

            Api& operator=(const Api &) = delete;

        private:
            QNetworkAccessManager& m_manager;
            QString m_addres;
    };

}}

#endif
