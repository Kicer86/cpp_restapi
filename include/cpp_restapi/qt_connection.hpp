
#ifndef CONNECTION_QT__HPP
#define CONNECTION_QT__HPP

#include <QNetworkRequest>
#include <QSignalMapper>
#include <QString>

#include "base_connection.hpp"
#include "cpp_restapi_export.h"


class QNetworkAccessManager;

namespace cpp_restapi::QtBackend
{
    class CPP_RESTAPI_EXPORT Connection: public QObject, public BaseConnection
    {
        public:
            Connection(QNetworkAccessManager &, const std::string& address, const std::map<std::string, std::string>& headerEntries);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

            using BaseConnection::fetch;

            Response fetchPage(const std::string& request) override;
            std::unique_ptr<ISseConnection> subscribe(const std::string& request, EventCallback callback) override;

        protected:
            void fetchAsync(const std::string& fullUrl, CancellationToken cancel, FetchCallback onSuccess, ErrorCallback onError) override;

        private:
            QNetworkAccessManager& m_networkManager;

            QNetworkRequest prepareRequest();
    };
}

#endif
