
#include <curl/curl.h>

#include <github_api/github_api_curl.hpp>
#include "connection.hpp"


namespace GitHub::CurlBackend
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
