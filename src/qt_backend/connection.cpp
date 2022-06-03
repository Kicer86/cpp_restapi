
#include "connection.hpp"

#include <cassert>
#include <string>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>


namespace GitHub { namespace QtBackend
{
    Connection::Connection(QNetworkAccessManager& manager, const QString& address, const QString& token)
        : BaseConnection(address.toStdString(), token.toStdString())
        , m_networkManager(manager)
    {

    }


    Connection::~Connection()
    {

    }


    std::pair<std::string, std::string> Connection::fetchPage(const std::string& page)
    {
        std::pair<std::string, std::string> result;

        QNetworkRequest request = prepareRequest();
        const QUrl url(QString::fromStdString(page));
        request.setUrl(url);

        QEventLoop loop;
        QNetworkReply* reply = m_networkManager.get(request);

        connect(reply, &QNetworkReply::readChannelFinished, [&result, &loop, reply]()
        {
            QString header;
            QList<QByteArray> headerList = reply->rawHeaderList();
            for(const QByteArray& head: headerList)
                header.append(
                    QString("%1: %2\n").arg(head.constData()).arg(reply->rawHeader(head).constData())
                );

            const QByteArray rawData = reply->readAll();
            result.first = rawData.data();
            result.second = header.toStdString();

            reply->deleteLater();

            loop.exit();
        });

        connect(reply, &QNetworkReply::errorOccurred, [reply, &page](QNetworkReply::NetworkError code)
        {
            qDebug() << QString("Error (%1 - %2) occured when processing request %3")
                .arg(code)
                .arg(reply->errorString())
                .arg(page.c_str());
        });

        connect(reply, &QNetworkReply::sslErrors, [&page](const QList<QSslError>& errors)
        {
            qDebug() << QString("Ssl errors occured when processing request %1:")
                .arg(page.c_str());

            for(const auto& error: qAsConst(errors))
                qDebug() << error.errorString();
        });

        loop.exec();

        return result;
    }


    QNetworkRequest Connection::prepareRequest()
    {
        QNetworkRequest request;

        const std::string& userToken = token();
        if (userToken.empty() == false)
        {
            const QByteArray key("Authorization");
            const QByteArray value = QString("token %1").arg(userToken.c_str()).toLatin1();
            request.setRawHeader(key, value);
        }

        request.setRawHeader("User-Agent", "github_api/1.0");

        return request;
    }
}}
