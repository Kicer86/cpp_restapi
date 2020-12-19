
#include "connection.hpp"

#include <cassert>
#include <string>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>


namespace GitHub { namespace QtBackend
{
    Connection::Connection(QNetworkAccessManager& manager, const QString& address, const QString& token):
        m_networkManager(manager),
        m_address(address),
        m_token(token)
    {

    }


    Connection::~Connection()
    {

    }


    std::string Connection::get(const std::string& query)
    {
        std::string result;

        QNetworkRequest request = prepareRequest();
        const QString url_str = QString("%1/%2").arg(m_address).arg(query.c_str());
        const QUrl url(url_str);
        request.setUrl(url);

        QEventLoop loop;
        QNetworkReply* reply = m_networkManager.get(request);

        connect(reply, &QNetworkReply::readChannelFinished, [&result, &loop, reply]() {

            const QByteArray rawData = reply->readAll();
            result = rawData.data();

            loop.exit();
        });

        loop.exec();

        return result;
    }


    QNetworkRequest Connection::prepareRequest()
    {
        QNetworkRequest request;

        if (m_token.isEmpty() == false)
        {
            const QByteArray key("Authorization");
            const QByteArray value = QString("token %1").arg(m_token).toLatin1();
            request.setRawHeader(key, value);
        }

        request.setRawHeader("User-Agent", "github_api/1.0");

        return request;
    }
}}
