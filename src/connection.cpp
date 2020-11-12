
#include "connection.hpp"

#include <cassert>
#include <string>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QSignalMapper>
#include <QEventLoop>


namespace GitHub
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
        const QUrl url = QString("%1/%2").arg(m_address).arg(query.c_str());
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
        QNetworkRequest requst;

        if (m_token.isEmpty() == false)
        {
            const QByteArray key("Authorization");
            const QByteArray value = QString("token %1").arg(m_token).toLatin1();
            requst.setRawHeader(key, value);
        }

        return requst;
    }
}
