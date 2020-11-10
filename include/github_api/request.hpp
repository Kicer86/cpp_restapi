
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <memory>

#include <QObject>
#include <QJsonDocument>

#include "github_api_export.h"

class QEventLoop;

namespace GitHub
{

    struct IConnection;

    class GITHUB_API_EXPORT Request
    {
        public:
            Request(IConnection *);
            Request(const Request &) = delete;
            ~Request();

            Request& operator=(const Request &) = delete;

            const QJsonDocument& getUserInfo(const QString& user);
            const QJsonDocument& getReleases(const QString& owner, const QString& repo);
            const QJsonDocument& getRelease(const QString& owner, const QString& repo, int id);

        private:
            IConnection* m_connection;
            QEventLoop* m_eventLoop;
            QJsonDocument m_result;

            const QJsonDocument& doRequest(const QString &);
            void waitForReply();
            void gotReply(const QJsonDocument &);
    };

}

#endif // REQUEST_H
