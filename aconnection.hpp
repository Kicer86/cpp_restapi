
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

class QString;

namespace GitHub
{
    struct AConnection
    {
        virtual ~AConnection() {}

        virtual void get(const QString &) = 0;
    };
}

#endif
