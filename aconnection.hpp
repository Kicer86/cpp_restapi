
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <QObject>
#include <QNetworkReply>
#include <QList>

class QString;
class QJsonDocument;

namespace GitHub
{
    struct AConnection: public QObject
    {
            Q_OBJECT

        public:
            virtual ~AConnection() {}

        private:
            friend class Request;
            virtual void get(const QString &) = 0;

        signals:
            void gotReply(const QJsonDocument &);
    };
}

#endif
