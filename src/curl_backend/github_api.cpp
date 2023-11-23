
#include <curl/curl.h>

#include <github_api/github_api_curl.hpp>
#include "connection.hpp"


namespace GitHub { namespace CurlBackend {

Api::Api(const std::string& addr)
    : m_addres(addr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}


Api::~Api()
{
    curl_global_cleanup();
}


std::unique_ptr<cpp_restapi::IConnection> Api::connect()
{
    return connect(std::string());
}


std::unique_ptr<cpp_restapi::IConnection> Api::connect(const std::string& token)
{
    return std::unique_ptr<cpp_restapi::IConnection>(new cpp_restapi::CurlBackend::Connection(m_addres, token));
}


std::string Api::address() const
{
    return m_addres;
}

}}
