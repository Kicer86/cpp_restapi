
#ifndef GITHUBAPI_HPP
#define GITHUBAPI_HPP

#include <string>

namespace GitHub {

// common interface for all Api backends
class IApi
{
public:
    virtual std::unique_ptr<GitHub::IConnection> connect() = 0;                             // anonymous access
    virtual std::unique_ptr<GitHub::IConnection> connect(const std::string& token) = 0;     // authorized connection
};

}

#endif
