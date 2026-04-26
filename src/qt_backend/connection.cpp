
#include <cpp_restapi/base_connection.hpp>
#include <cpp_restapi/create_qt_connection.hpp>
#include "qt_sse_connection.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>


namespace
{
    class QtConnection final: public QObject, public cpp_restapi::BaseConnection
    {
        public:
            QtConnection(QNetworkAccessManager &, const std::string& address, const std::map<std::string, std::string>& headerEntries);
            QtConnection(const QtConnection &) = delete;

            ~QtConnection() override;

            QtConnection& operator=(const QtConnection &) = delete;

            cpp_restapi::Response fetchPage(const std::string& request) override;
            std::unique_ptr<cpp_restapi::ISseConnection> subscribe(const std::string& request, cpp_restapi::IConnection::EventCallback callback) override;

        protected:
            void fetchAsync(const std::string& fullUrl,
                            cpp_restapi::CancellationToken cancel,
                            cpp_restapi::IConnection::FetchCallback onSuccess,
                            cpp_restapi::IConnection::ErrorCallback onError) override;

        private:
            QNetworkAccessManager& m_networkManager;

            QNetworkRequest prepareRequest();
    };


    QtConnection::QtConnection(QNetworkAccessManager& manager, const std::string& address, const std::map<std::string, std::string>& headerEntries)
        : cpp_restapi::BaseConnection(address, headerEntries)
        , m_networkManager(manager)
    {

    }


    QtConnection::~QtConnection()
    {

    }


    cpp_restapi::Response QtConnection::fetchPage(const std::string& page)
    {
        cpp_restapi::Response result;

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


    QNetworkRequest QtConnection::prepareRequest()
    {
        QNetworkRequest request;

        const auto header_entries = getHeaderEntries();

        for(const auto& [k, v]: header_entries)
            request.setRawHeader(k.c_str(), v.c_str());

        request.setRawHeader("User-Agent", "cpp_restapi/2.0");

        return request;
    }


    std::unique_ptr<cpp_restapi::ISseConnection> QtConnection::subscribe(const std::string& request, cpp_restapi::IConnection::EventCallback callback)
    {
        auto sse = std::make_unique<cpp_restapi::QtBackend::SseConnection>(m_networkManager, address(), getHeaderEntries());
        sse->subscribe(request, std::move(callback));
        return sse;
    }


    void QtConnection::fetchAsync(const std::string& url,
                                  cpp_restapi::CancellationToken cancel,
                                  cpp_restapi::IConnection::FetchCallback onSuccess,
                                  cpp_restapi::IConnection::ErrorCallback onError)
    {
        QNetworkRequest request = prepareRequest();
        request.setUrl(QUrl(QString::fromStdString(url)));
        QNetworkReply* reply = m_networkManager.get(request);

        QObject::connect(reply, &QNetworkReply::finished,
            [reply, cancel = std::move(cancel), onSuccess = std::move(onSuccess), onError = std::move(onError)]()
        {
            reply->deleteLater();

            if (cancel->load(std::memory_order_acquire))
                return;

            if (reply->error() == QNetworkReply::NoError)
            {
                cpp_restapi::Response resp;
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
                    onError(cpp_restapi::HttpError{code, reply->readAll().toStdString(), reply->errorString().toStdString()});
                }
            }
        });
    }
}


namespace cpp_restapi
{
    std::unique_ptr<IConnection> createQtConnection(
        QNetworkAccessManager& manager,
        const std::string& address,
        const std::map<std::string, std::string>& headerEntries)
    {
        return std::make_unique<QtConnection>(manager, address, headerEntries);
    }
}
