
#ifndef GITHUB_API_BASE_HPP_INCLUDED
#define GITHUB_API_BASE_HPP_INCLUDED

#include "github_api/igithub_api.hpp"

namespace cpp_restapi
{

    class GitHubBase: public GitHub::IApi
    {
        public:
            GitHubBase(const std::string& address);

            const std::string& address() const override;

        private:
            std::string m_address;
    };

}

#endif
