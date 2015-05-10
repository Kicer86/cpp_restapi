
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <memory>

#include <QObject>
#include <QList>

class QString;
class QJsonDocument;
class QNetworkReply;

namespace GitHub
{
    struct AConnection
    {
        virtual ~AConnection() {}

        virtual std::unique_ptr<QNetworkReply> get(const QString &) = 0;
    };
}

#endif
