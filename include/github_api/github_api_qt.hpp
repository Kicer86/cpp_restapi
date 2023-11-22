
#ifndef GITHUBAPI_QT_HPP
#define GITHUBAPI_QT_HPP

// Based on:
// https://developer.github.com/guides/getting-started/
// https://developer.github.com/v3/

#include <memory>

#include <QString>

#include "cpp_restapi/iconnection.hpp"
#include "igithub_api.hpp"
#include "github_api_export.h"

class QNetworkAccessManager;

namespace GitHub { namespace QtBackend
{

    /**
     * @brief Class for establishing connection with GitHub api with Qt's QNetworkAccessManager
     */
    class GITHUB_API_EXPORT Api: public IApi
    {
        public:
            Api(QNetworkAccessManager &, const QString& addr = "https://api.github.com");
            Api(const Api &) = delete;

            std::unique_ptr<cpp_restapi::IConnection> connect() override;
            std::unique_ptr<cpp_restapi::IConnection> connect(const std::string& token) override;
            std::string address() const override;

            Api& operator=(const Api &) = delete;

        private:
            QNetworkAccessManager& m_manager;
            QString m_addres;
    };

}}

#endif
