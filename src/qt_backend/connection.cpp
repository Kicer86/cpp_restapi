
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

        connect(reply, &QNetworkReply::readChannelFinished, [&result, &loop, reply]() 
        {
            const QByteArray rawData = reply->readAll();
            result = rawData.data();
            reply->deleteLater();

            loop.exit();
        });

        connect(reply, &QNetworkReply::errorOccurred, [reply, &query](QNetworkReply::NetworkError code) 
        {
            qDebug() << QString("Error (%1 - %2) occured when processing request %3")
                .arg(code)
                .arg(reply->errorString())
                .arg(query.c_str());
        });

        connect(reply, &QNetworkReply::sslErrors, [&query](const QList<QSslError>& errors)
        {
            qDebug() << QString("Ssl errors occured when processing request %1:")
                .arg(query.c_str());

            for(const auto& error: qAsConst(errors))
                qDebug() << error.errorString();
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
