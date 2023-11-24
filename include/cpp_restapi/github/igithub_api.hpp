
#ifndef GITHUBAPI_HPP
#define GITHUBAPI_HPP

#include <memory>
#include <string>

#include <cpp_restapi/iconnection.hpp>


namespace cpp_restapi::GitHub
{

/**
 * @brief common interface for all api backends. Deprecated, use GitHubConnectionBuilder instead
 */
class [[deprecated("Use GitHubConnectionBuilder")]] IApi
{
public:
    virtual ~IApi() = default;

    /**
     * @brief open authorized connection with GitHub api
     * @param token GitHub's authentication token. One can be generated on https://github.com/settings/tokens
     * @return \ref GitHub::IConnection object which can be used with \ref GitHub::Request
     */
    virtual std::unique_ptr<cpp_restapi::IConnection> connect(const std::string& token) = 0;

    /**
     * @return GitHub api address
     */
    virtual const std::string& address() const = 0;
};

}

#endif
