#include "qt_sse_connection.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>


namespace cpp_restapi::QtBackend
{

SseConnection::SseConnection(QNetworkAccessManager& manager,
                             const std::string& address,
                             const std::map<std::string, std::string>& headerEntries)
    : m_networkManager(manager)
    , m_address(address)
    , m_headerEntries(headerEntries)
{
}


SseConnection::~SseConnection()
{
    close();
}


void SseConnection::subscribe(const std::string& request, EventCallback callback)
{
    close();

    m_callback = std::move(callback);

    const std::string fullUrl = m_address + "/" + request;
    QNetworkRequest netRequest = prepareRequest(fullUrl);

    m_reply = m_networkManager.get(netRequest);

    connect(m_reply, &QNetworkReply::readyRead, this, [this]()
    {
        const QByteArray raw = m_reply->readAll();
        const std::string chunk(raw.constData(), raw.size());

        const auto events = m_parser.feed(chunk);
        for (const auto& event : events)
            m_callback(event);
    });

    connect(m_reply, &QNetworkReply::finished, this, [this]()
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    });
}


void SseConnection::close()
{
    if (m_reply != nullptr)
    {
        m_reply->disconnect(this);
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    m_parser = SseParser{};
    m_callback = nullptr;
}


QNetworkRequest SseConnection::prepareRequest(const std::string& url) const
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromStdString(url)));

    for (const auto& [k, v] : m_headerEntries)
        request.setRawHeader(k.c_str(), v.c_str());

    request.setRawHeader("User-Agent", "cpp_restapi/2.0");
    request.setRawHeader("Accept", "text/event-stream");
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    return request;
}

}
