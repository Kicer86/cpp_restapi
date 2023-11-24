
#include "github_api/github_api_base.hpp"


namespace cpp_restapi
{

    GitHubBase::GitHubBase(const std::string& address)
        : m_address(address)
    {

    }

    const std::string& GitHubBase::address() const
    {
        return m_address;
    }
}
