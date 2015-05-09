
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <QObject>

class QString;

namespace GitHub
{
    struct AConnection: public QObject
    {
        virtual ~AConnection() {}

        virtual void get(const QString &) = 0;
        
        Q_OBJECT
    };
}

#endif
