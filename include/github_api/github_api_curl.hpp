
#ifndef GITHUBAPI_CURL_HPP
#define GITHUBAPI_CURL_HPP

// Based on:
// https://developer.github.com/guides/getting-started/
// https://developer.github.com/v3/

#include <memory>

#include "iconnection.hpp"
#include "igithub_api.hpp"
#include "github_api_export.h"

namespace GitHub { namespace CurlBackend {

    class GITHUB_API_EXPORT Api: public IApi
    {
        public:
            Api(const std::string& addr = "https://api.github.com");
            Api(const Api &) = delete;
            ~Api();

            std::unique_ptr<GitHub::IConnection> connect() override;
            std::unique_ptr<GitHub::IConnection> connect(const std::string& token) override;

            Api& operator=(const Api &) = delete;

        private:
            std::string m_addres;
    };

}}

#endif
