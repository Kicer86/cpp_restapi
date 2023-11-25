
#include <curl/curl.h>

#include <cpp_restapi/github/github_api_curl.hpp>
#include "connection.hpp"


namespace cpp_restapi::GitHub::CurlBackend
{

Api::Api(const std::string& addr)
    : cpp_restapi::GitHubBase(addr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}


Api::~Api()
{
    curl_global_cleanup();
}


std::unique_ptr<cpp_restapi::IConnection> Api::connect(const std::string& token)
{
    std::map<std::string, std::string> headerEntries;

    if (token.empty() == false)
        headerEntries.emplace("Authorization", "token " + token);

    return std::make_unique<cpp_restapi::CurlBackend::Connection>(address(), headerEntries);
}

}
