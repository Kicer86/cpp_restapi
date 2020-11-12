
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QNetworkRequest>
#include <QSignalMapper>

#include <github_api/iconnection.hpp>


class QNetworkAccessManager;
class QNetworkReply;
class QString;

namespace GitHub
{
    class Connection: public QObject, public IConnection
    {
        public:
            Connection(QNetworkAccessManager &, const QString& address, const QString& token);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

        private:
            QNetworkAccessManager& m_networkManager;
            QSignalMapper m_signalMapper;
            const QString m_address;
            const QString m_token;
            std::map<QObject *, Callback> m_replys;

            QNetworkRequest prepareRequest();

            // AConnection overrides:
            void get(const std::string &, const Callback&) override;

            void gotReply(QObject *);
    };
}

#endif // CONNECTION_HPP
