
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <string>

namespace GitHub
{
    struct IConnection
    {
        public:
            virtual ~IConnection() = default;

            virtual std::string get(const std::string &) = 0;
    };
}

#endif
