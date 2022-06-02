
#ifndef GITHUBAPI_HPP
#define GITHUBAPI_HPP

#include <string>

namespace GitHub {

// common interface for all Api backends
class IApi
{
public:
    virtual ~IApi() = default;

    virtual std::unique_ptr<GitHub::IConnection> connect() = 0;                             // anonymous access
    virtual std::unique_ptr<GitHub::IConnection> connect(const std::string& token) = 0;     // authorized connection
    virtual std::string address() const = 0;                                                // access to server address
};

}

#endif
