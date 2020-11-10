
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <github_api/iconnection.hpp>
#include <QNetworkRequest>


class QNetworkAccessManager;
class QNetworkReply;
class QString;
class QSignalMapper;

namespace GitHub
{
    class Connection: public QObject, public IConnection
    {
            Q_OBJECT

        public:
            Connection(QNetworkAccessManager &, const QString& address, const QString& token);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

        private:
            QNetworkAccessManager& m_networkManager;
            QSignalMapper* m_signalMapper;
            const QString m_address;
            const QString m_token;
            std::map<QObject *, Callback> m_replys;

            QNetworkRequest prepareRequest();

            // AConnection overrides:
            void get(const QString &, const Callback&) override;

        private slots:
            void gotReply(QObject *);
    };
}

#endif // CONNECTION_HPP
