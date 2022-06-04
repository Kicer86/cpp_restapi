
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <memory>
#include <string>

#include "github_api_export.h"


namespace GitHub
{
    struct IConnection;

    /**
     * @brief GitHub api actions.
     *
     * Class contains a convenient set of actions which can be
     * executed on GitHub's api.
     *
     * Before one can use it a connection with github needs to be established.
     * Use \ref GitHub::QtBackend::Api or \ref GitHub::CurlBackend::Api
     * to construct an \ref GitHub::IConnection object.
     *
     * All methods return a response in json format.
     */
    class GITHUB_API_EXPORT Request
    {
        public:
            Request(std::unique_ptr<IConnection>);
            Request(const Request &) = delete;
            ~Request();

            Request& operator=(const Request &) = delete;

            /**
             * @brief Request user info
             * @param user GitHub user name
             * @return api response in json format
             *
             * Request user information. Equivalent of fetching https://api.github.com/users/<user>
             */
            std::string getUserInfo(const std::string& user);

            /**
             * @brief Request releases for repository
             * @param user GitHub user name
             * @param repo user's repository name
             * @return api response in json format
             *
             * Request list of releases for repository. Equivalent of fetching https://api.github.com/repos/<user>/<repo>/releases
             */
            std::string getReleases(const std::string& user, const std::string& repo);

            /**
             * @brief Request release details
             * @param user GitHub user name
             * @param repo user's repository name
             * @param id release id. Id is returned as a part of \ref getReleases
             * @return api response in json format
             *
             * Request details of release. Equivalent of fetching https://api.github.com/repos/<user>/<repo>/releases/<id>
             */
            std::string getRelease(const std::string& user, const std::string& repo, int id);

            /**
             * @brief Request api limits
             * @return api response in json format
             *
             * Request limits for api calls. Equivalent of fetching https://api.github.com/rate_limit
             */
            std::string getRateLimit();

            /**
             * @brief Request list of user repositories
             * @param user GitHub user name
             * @return api response in json format
             *
             * Request list of repositories for user. Equivalent of fetching https://api.github.com/users/<user>/repos
             */
            std::string listUserRepo(const std::string& user);

        private:
            std::unique_ptr<IConnection> m_connection;

            std::string doRequest(const std::string &);
    };
}

#endif // REQUEST_H
