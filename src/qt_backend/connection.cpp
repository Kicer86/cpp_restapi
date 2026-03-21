
#include <cpp_restapi/qt_connection.hpp>
#include "qt_sse_connection.hpp"

#include <cassert>
#include <string>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>


namespace cpp_restapi::QtBackend
{
    Connection::Connection(QNetworkAccessManager& manager, const std::string& address, const std::map<std::string, std::string>& headerEntries)
        : BaseConnection(address, headerEntries)
        , m_networkManager(manager)
    {

    }


    Connection::~Connection()
    {

    }


    Response Connection::fetchPage(const std::string& page)
    {
        Response result;

        QNetworkRequest request = prepareRequest();
        const QUrl url(QString::fromStdString(page));
        request.setUrl(url);

        QEventLoop loop;
        QNetworkReply* reply = m_networkManager.get(request);

        connect(reply, &QNetworkReply::readChannelFinished, [&result, &loop, reply]()
        {
            result.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            QString header;
            QList<QByteArray> headerList = reply->rawHeaderList();
            for(const QByteArray& head: headerList)
                header.append(
                    QString("%1: %2\n").arg(head.constData()).arg(reply->rawHeader(head).constData())
                );

            const QByteArray rawData = reply->readAll();
            result.body = rawData.toStdString();
            result.headers = header.toStdString();

            reply->close();
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

            for(const auto& error: std::as_const(errors))
                qDebug() << error.errorString();
        });

        loop.exec();

        return result;
    }


    QNetworkRequest Connection::prepareRequest()
    {
        QNetworkRequest request;

        const auto header_entries = getHeaderEntries();

        for(const auto& [k, v]: header_entries)
            request.setRawHeader(k.c_str(), v.c_str());

        request.setRawHeader("User-Agent", "cpp_restapi/2.0");

        return request;
    }


    std::unique_ptr<ISseConnection> Connection::subscribe(const std::string& request, EventCallback callback)
    {
        auto sse = std::make_unique<SseConnection>(m_networkManager, address(), getHeaderEntries());
        sse->subscribe(request, std::move(callback));
        return sse;
    }


    void Connection::fetchAsync(const std::string& url, FetchCallback onSuccess, ErrorCallback onError)
    {
        QNetworkRequest request = prepareRequest();
        request.setUrl(QUrl(QString::fromStdString(url)));
        QNetworkReply* reply = m_networkManager.get(request);

        QObject::connect(reply, &QNetworkReply::finished,
            [reply, onSuccess = std::move(onSuccess), onError = std::move(onError)]()
        {
            reply->deleteLater();

            if (reply->error() == QNetworkReply::NoError)
            {
                Response resp;
                resp.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                resp.body = reply->readAll().toStdString();

                QString header;
                const QList<QByteArray> headerList = reply->rawHeaderList();
                for (const QByteArray& head : headerList)
                    header.append(QString("%1: %2\n").arg(head.constData()).arg(reply->rawHeader(head).constData()));
                resp.headers = header.toStdString();

                if (onSuccess)
                    onSuccess(std::move(resp));
            }
            else
            {
                if (onError)
                {
                    const int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                    onError(HttpError{code, reply->readAll().toStdString(), reply->errorString().toStdString()});
                }
            }
        });
    }
}
