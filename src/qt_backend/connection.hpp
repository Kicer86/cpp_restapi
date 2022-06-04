
#ifndef CONNECTION_QT__HPP
#define CONNECTION_QT__HPP

#include <QNetworkRequest>
#include <QSignalMapper>
#include <QString>

#include "base_connection.hpp"


class QNetworkAccessManager;

namespace GitHub { namespace QtBackend {

    class Connection: public QObject, public BaseConnection
    {
        public:
            Connection(QNetworkAccessManager &, const QString& address, const QString& token);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

            std::pair<std::string, std::string> fetchPage(const std::string& request) override;

        private:
            QNetworkAccessManager& m_networkManager;

            QNetworkRequest prepareRequest();
    };

}}

#endif
