
#ifndef CONNECTION_QT__HPP
#define CONNECTION_QT__HPP

#include <QNetworkRequest>
#include <QSignalMapper>
#include <QString>

#include <github_api/iconnection.hpp>


class QNetworkAccessManager;

namespace GitHub { namespace QtBackend {

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

}}

#endif
