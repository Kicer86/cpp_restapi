
#ifndef GITHUBAPI_CURL_HPP
#define GITHUBAPI_CURL_HPP

#include <memory>

#include "iconnection.hpp"
#include "igithub_api.hpp"
#include "github_api_export.h"

namespace GitHub { namespace CurlBackend
{
    /**
     * @brief Class for establishing connection with GitHub api with Curl
     */
    class GITHUB_API_EXPORT Api: public IApi
    {
        public:
            Api(const std::string& addr = "https://api.github.com");
            Api(const Api &) = delete;
            ~Api();

            std::unique_ptr<GitHub::IConnection> connect() override;
            std::unique_ptr<GitHub::IConnection> connect(const std::string& token) override;
            std::string address() const override;

            Api& operator=(const Api &) = delete;

        private:
            std::string m_addres;
    };

}}

#endif
