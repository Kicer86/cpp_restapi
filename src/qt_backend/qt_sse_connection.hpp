#ifndef SSE_CONNECTION_QT_HPP_INCLUDED
#define SSE_CONNECTION_QT_HPP_INCLUDED

#include <map>
#include <string>

#include <QObject>
#include <QNetworkRequest>

#include <cpp_restapi/isse_connection.hpp>
#include <cpp_restapi/sse_parser.hpp>
#include "cpp_restapi_export.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace cpp_restapi::QtBackend
{
    /**
     * @brief Qt-based SSE connection
     *
     * Uses QNetworkAccessManager to establish a streaming HTTP connection
     * to an SSE endpoint and delivers parsed events via callback.
     */
    class CPP_RESTAPI_EXPORT SseConnection: public QObject, public cpp_restapi::ISseConnection
    {
    public:
        SseConnection(QNetworkAccessManager& manager,
                      const std::string& address,
                      const std::map<std::string, std::string>& headerEntries);

        ~SseConnection() override;

        SseConnection(const SseConnection&) = delete;
        SseConnection& operator=(const SseConnection&) = delete;

        void subscribe(const std::string& request, EventCallback callback) override;
        void close() override;

    private:
        QNetworkAccessManager& m_networkManager;
        std::string m_address;
        std::map<std::string, std::string> m_headerEntries;
        QNetworkReply* m_reply = nullptr;
        SseParser m_parser;
        EventCallback m_callback;

        QNetworkRequest prepareRequest(const std::string& url) const;
    };
}

#endif
