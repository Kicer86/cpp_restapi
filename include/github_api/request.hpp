
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <memory>
#include <string>
#include <QObject>

#include "github_api_export.h"

class QEventLoop;

namespace GitHub
{
    struct IConnection;

    class GITHUB_API_EXPORT Request
    {
        public:
            Request(IConnection *);
            Request(const Request &) = delete;
            ~Request();

            Request& operator=(const Request &) = delete;

            const std::string& getUserInfo(const std::string& user);
            const std::string& getReleases(const std::string& owner, const std::string& repo);
            const std::string& getRelease(const std::string& owner, const std::string& repo, int id);

        private:
            IConnection* m_connection;
            QEventLoop* m_eventLoop;
            std::string m_result;

            const std::string& doRequest(const std::string &);
            void waitForReply();
            void gotReply(const std::string &);
    };
}

#endif // REQUEST_H
