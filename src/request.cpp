
#include <github_api/request.hpp>
#include <github_api/iconnection.hpp>


namespace GitHub
{

    Request::Request(IConnection* connection): m_connection(connection)
    {
    }


    Request::~Request()
    {

    }


    std::string Request::getUserInfo(const std::string& user)
    {
        const std::string request = std::string("users/") + user;
        return doRequest(request);
    }


    std::string Request::getReleases(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/releases";
        return doRequest(request);
    }


    std::string Request::getRelease(const std::string& owner, const std::string& repo, int id)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/releases/" + std::to_string(id);
        return doRequest(request);
    }


    std::string Request::getRateLimit()
    {
        return doRequest("rate_limit");
    }


    std::string Request::doRequest(const std::string& request)
    {
        return m_connection->get(request);
    }
}
