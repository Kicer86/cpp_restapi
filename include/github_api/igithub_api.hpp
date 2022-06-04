
#ifndef GITHUBAPI_HPP
#define GITHUBAPI_HPP

#include <string>

namespace GitHub
{

/**
 * @brief common interface for all api backends
 */
class IApi
{
public:
    virtual ~IApi() = default;

    /**
     * @brief open anonymous connection with GitHub api
     * @return \ref GitHub::IConnection object which can be used with \ref GitHub::Request
     */
    virtual std::unique_ptr<GitHub::IConnection> connect() = 0;

    /**
     * @brief open authorized connection with GitHub api
     * @param token GitHub's authentication token. One can be generated on https://github.com/settings/tokens
     * @return \ref GitHub::IConnection object which can be used with \ref GitHub::Request
     */
    virtual std::unique_ptr<GitHub::IConnection> connect(const std::string& token) = 0;

    /**
     * @return GitHub api address
     */
    virtual std::string address() const = 0;
};

}

#endif
