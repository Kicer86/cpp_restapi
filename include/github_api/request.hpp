
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <memory>
#include <string>

#include "github_api_export.h"


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

            std::string getUserInfo(const std::string& user);
            std::string getReleases(const std::string& owner, const std::string& repo);
            std::string getRelease(const std::string& owner, const std::string& repo, int id);
            std::string getRateLimit();

        private:
            IConnection* m_connection;

            std::string doRequest(const std::string &);
    };
}

#endif // REQUEST_H
