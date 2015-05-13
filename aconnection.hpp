
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <functional>

#include <QObject>
#include <QNetworkReply>
#include <QList>

class QString;
class QJsonDocument;

namespace GitHub
{
    struct AConnection
    {
        public:
            typedef std::function<void(const QJsonDocument &)> Callback;

            virtual ~AConnection() {}

        private:
            friend class Request;
            virtual void get(const QString &, const Callback &) = 0;
    };
}

#endif
