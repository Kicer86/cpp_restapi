
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
    struct IConnection
    {
        public:
            virtual ~IConnection() = default;

        private:
            friend class Request;
            virtual std::string get(const std::string &) = 0;
    };
}

#endif
