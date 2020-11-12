
#include <cassert>
#include <QNetworkReply>
#include <QEventLoop>

#include <github_api/request.hpp>
#include <github_api/iconnection.hpp>


namespace GitHub
{

    Request::Request(IConnection* connection): m_connection(connection), m_eventLoop(nullptr), m_result()
    {
    }


    Request::~Request()
    {

    }


    const std::string& Request::getUserInfo(const std::string& user)
    {
        const std::string request = std::string("users/") + user;
        return doRequest(request);
    }


    const std::string& Request::getReleases(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/releases";
        return doRequest(request);
    }


    const std::string& Request::getRelease(const std::string& owner, const std::string& repo, int id)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/releases/" + std::to_string(id);
        return doRequest(request);
    }


    const std::string& Request::doRequest(const std::string& request)
    {
        auto callback = std::bind(&Request::gotReply, this, std::placeholders::_1);

        m_result.clear();
        m_connection->get(request, callback);

        waitForReply();

        return m_result;
    }


    void Request::waitForReply()
    {
        assert(m_eventLoop == nullptr);

        m_eventLoop = new QEventLoop;
        m_eventLoop->exec();
        delete m_eventLoop, m_eventLoop = nullptr;
    }


    void Request::gotReply(const std::string& json)
    {
        m_result = json;
        m_eventLoop->exit();
    }

}
