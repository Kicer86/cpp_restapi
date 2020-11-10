
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


    const QJsonDocument& Request::getUserInfo(const QString& user)
    {
        const QString request = QString("users/%1").arg(user);
        return doRequest(request);
    }


    const QJsonDocument& Request::getReleases(const QString& owner, const QString& repo)
    {
        const QString request = QString("repos/%1/%2/releases").arg(owner).arg(repo);
        return doRequest(request);
    }


    const QJsonDocument& Request::getRelease(const QString& owner, const QString& repo, int id)
    {
        const QString request = QString("repos/%1/%2/releases/%3").arg(owner).arg(repo).arg(id);
        return doRequest(request);
    }


    const QJsonDocument& Request::doRequest(const QString& request)
    {
        auto callback = std::bind(&Request::gotReply, this, std::placeholders::_1);

        m_result = QJsonDocument();
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


    void Request::gotReply(const QJsonDocument& json)
    {
        m_result = json;
        m_eventLoop->exit();
    }

}
