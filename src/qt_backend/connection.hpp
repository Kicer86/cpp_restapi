
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QNetworkRequest>
#include <QSignalMapper>

#include <github_api/iconnection.hpp>


class QNetworkAccessManager;
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

            std::string get(const std::string &) override;

        private:
            QNetworkAccessManager& m_networkManager;
            const QString m_address;
            const QString m_token;

            QNetworkRequest prepareRequest();
    };
}

#endif // CONNECTION_HPP
