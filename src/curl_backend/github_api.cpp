
#include <github_api/github_api_curl.hpp>

namespace GitHub { namespace CurlBackend {

Api::Api(const std::string& addr)
    : m_addres(addr)
{

}


std::unique_ptr<GitHub::IConnection> Api::connect()
{

}


std::unique_ptr<GitHub::IConnection> Api::connect(const std::string& token)
{

}

}}
