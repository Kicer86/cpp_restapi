
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <memory>
#include <string>

#include <cpp_restapi/iconnection.hpp>
#include "github_api_export.h"


namespace cpp_restapi::GitHub
{
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
            Request(std::unique_ptr<cpp_restapi::IConnection>);
            Request(const Request &) = delete;
            ~Request();

            Request& operator=(const Request &) = delete;

            /**
             * @brief Request user info
             * @param user GitHub user name
             * @return api response in json format
             *
             * Request user information. Equivalent of fetching https://api.github.com/users/\<user>
             */
            std::string getUserInfo(const std::string& user);

            /**
             * @brief Request releases for repository
             * @param user GitHub user name
             * @param repo user's repository name
             * @return api response in json format
             *
             * Request list of releases for repository. Equivalent of fetching https://api.github.com/repos/\<user>/\<repo>/releases
             */
            std::string getReleases(const std::string& user, const std::string& repo);

            /**
             * @brief Request release details
             * @param user GitHub user name
             * @param repo user's repository name
             * @param id release id. Id is returned as a part of \ref getReleases
             * @return api response in json format
             *
             * Request details of release. Equivalent of fetching https://api.github.com/repos/\<user>/\<repo>/releases/\<id>
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
             * Request list of repositories for user. Equivalent of fetching https://api.github.com/users/\<user>/repos
             */
            std::string listUserRepo(const std::string& user);


            // --------------- user info related api

            /**
             * @brief get the authenticated user info
             *
             * @return std::string json std::string response
             */
            std::string getAuntenticatedUser();

            /**
             * @brief Lists all users, in the order that they
             * signed up on GitHub. This list includes personal
             * user accounts and organization accounts.
             *
             * @return std::string list of gitub account
             */
            std::string listUsers();
            /**
             * @brief Provides publicly available information about
             * someone with a GitHub account.
             *
             * @param username github user name
             * @return std::string
             */
            std::string getUser(const std::string& username);


            // issues related api methods
            /**
             * @brief List issues assigned to the authenticated user
             * across all visible repositories including owned
             * repositories, member repositories, and organization
             * repositories.
             *
             * @return std::string list of issues assigned to the user
             */
            std::string issues();

            /**
             * @brief List issues in an organization assigned to the
             * authenticated user.
             *
             * @param org github organization
             * @return std::string
             */
            std::string orgIssues(const std::string& org);

            /**
             * @brief List issues in a repository.
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return std::string list of isues associated with owner
             */
            std::string listRepoIssues(const std::string& owner, const std::string& repo);

            /**
             * @brief
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @param issueNumber The number that identifies the issue.
             * @return std::string all info associated with the issue
             */
            std::string getIssue(const std::string& owner, const std::string& repo, const std::string& issueNumber);

            // pull request related api methods
            /**
             * @brief List pull request in a repository.
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return std::string list of pull request associated with owner
             */
            std::string listPullRequest(const std::string& owner, const std::string& repo);

            /**
             * @brief Lists details of a pull request by providing its number.
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @param pullNumber The number that identifies the PR.
             * @return std::string, all info associated with the PR
             */
            std::string getPullRequest(const std::string& owner, const std::string& repo, const std::string& pullNumber);

            /**
             * @brief Lists a maximum of 250 commits for a pull request
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository
             * @param pullNumber The number that identifies the PR.
             * @return std::string,  Lists up to 250 commits for a pull request
             */
            std::string listPullRequestCommit(const std::string& owner, const std::string& repo, const std::string& pullNumber);

            /**
             * @brief Responses include a maximum of 3000 files.
             *      The paginated response returns 30 files per
             *      page by default.
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository
             * @param pullNumber The number that identifies the PR.
             * @return std::string,  list of dict with details of each committed file
             */
            std::string listPullRequestfiles(const std::string& owner, const std::string& repo, const std::string& pullNumber);

            /**
             * @brief Check if a pull request has been merged
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository
             * @param pullNumber The number that identifies the PR.
             * @return std::string,  Status: 204 if  if pull request has been merged
             */
            std::string isPRmerged(const std::string& owner, const std::string& repo, const std::string& pullNumber);

            // commits related api methods
            /**
             * @brief List commits.
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return The response will include a verification object that
             *          describes the result of verifying the commit's signature.
             *          returns list of objects
             */
            std::string listCommits(const std::string& owner, const std::string& repo);

            /**
             * @brief List branches for HEAD commit
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository
             * @param commitSha The SHA of the commit.
             * @return  All branches where the given commit
             *          SHA is the HEAD, or latest commit for the branch.
             */
            std::string listBranchHeadCommit(const std::string& owner, const std::string& repo,const std::string& commitSha);

            /**
             * @brief List pull requests associated with a commit
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository
             * @param commitSha The SHA of the commit.
             * @return All open,closed pull requests associated with the commit.
             */
            std::string listCommitPullRequest(const std::string& owner, const std::string& repo, const std::string& commitSha);

            /**
             * @brief Get a commit
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @param reference ref parameter
             * @return Returns the contents of a single commit reference.
             *         You must have read access for the repository to use this
             *         endpoint.
             */
            std::string getCommits(const std::string& owner, const std::string& repo, const std::string& reference);

            // metrics related api methods
            /**
             * @brief Get the weekly commit activity
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return a weekly aggregate of the number of additions and
             *         deletions pushed to a repository.
             */
            std::string getWeeklyCommit(const std::string& owner, const std::string& repo);

            /**
             * @brief Get the last year of commit activity
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return std::string Returns the last year of commit activity grouped by week.
             *         The days array is a group of commits per day, starting on Sunday
             */
            std::string getLastYearCommit(const std::string& owner, const std::string& repo);
            /**
             * @brief Get all contributor commit activity
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return std::string Returns the total number of commits authored by the
             *         contributor. In addition, the response includes a Weekly
             *         Hash (weeks array) with the following information:
                    w - Start of the week, given as a Unix timestamp.
                    a - Number of additions
                    d - Number of deletions
                    c - Number of commits
            */
            std::string getcontributorsActivity(const std::string& owner, const std::string& repo);

            /**
             * @brief Get the weekly commit count
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return The total commit counts for the owner and total commit counts in
             *         all. All is everyone combined, including the owner in the last 52
             *         weeks. If you'd like to get the commit counts for non-owners, you
             *         can subtract owner from all.
             */
            std::string getCommitCount(const std::string& owner, const std::string& repo);

            /**
             * @brief Get the hourly commit count for each day
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return std::string Returns array containing the day number, hour number,
             *         and number of commits:
             *         For example, [2, 14, 25] indicates that there
             *         were 25 total commits, during the 2:00pm hour on Tuesdays
             */
            std::string getHourlyCommitCount(const std::string& owner, const std::string& repo);

            /**
             * @brief Get community profile metrics
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return all community profile metrics, including an overall health
             *         score, repository description, the presence of documentation
             *         detected code of conduct, detected license,and the presence of
             *         ISSUE_TEMPLATE, PULL_REQUEST_TEMPLATE,README, and CONTRIBUTING
             *         files
             */
            std::string getCommunityProfileMetrics(const std::string& owner, const std::string& repo);

            /**
             * @brief Get repository clones
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return return Get the total number of clones and breakdown per day
             *         or week for the last 14 days. Timestamps are aligned to UTC
             *         midnight of the beginning of the day or week. Week begins on
             *         Monday.
             */
            std::string getRepoClones(const std::string& owner, const std::string& repo);

            /**
             * @brief Get top referral paths
             *
             * @param owner The account owner of the repository.
             * @param repo the name of the repository.
             * @return return lists of the top 10 popular contents over the last
             *         14 days.
             */
            std::string getRefferalPaths(const std::string& owner, const std::string& repo);

            /**
             * @brief Get top referral sources
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return std::string Get the top 10 referrers over the last 14 days.
             */
            std::string getTopreferralSource(const std::string& owner, const std::string& repo);

            /**
             * @brief Get page views. Get the total number of views and breakdown
             *        per day or week for the last 14 days.
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return std::string Get the top 10 referrers over the last 14 days.
             */
            std::string getPageViews(const std::string& owner, const std::string& repo);

            // event related apis
            /**
             * @brief List public events for a network of
             *        repositories
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return public event associated with repo
             */
            std::string listNetworkRepoEvent(const std::string& owner, const std::string& repo);

            /**
             * @brief List public organization events
             *
             * @param org The organization name.
             * @return public event associated with repo
             */
            std::string listOrgEvent(const std::string& org);

            /**
             * @brief List repository events
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return public event associated with repo
             */
            std::string listRepoEvent(const std::string& owner, const std::string& repo);

            /**
             * @brief List events for the authenticated user
             *
             * @param username The handle for the GitHub user account
             * @return If you are authenticated as the given user, you will see your
             *         private events. Otherwise, you'll only see public events.
             */
            std::string listUserEvent(const std::string& username);

            // staring related api methods

            /**
             * @brief Lists the people that have starred the repository.
             *
             * @param owner The account owner of the repository.
             * @param repo The name of the repository.
             * @return returns Lists of people that have starred the repository.
             */
            std::string listStargazers(const std::string& owner, const std::string& repo);

            /**
             * @brief List repositories starred by a user
             *
             * @param username The handle for the GitHub user account
             * @return returns a list of repositories a user has starred
             */
            std::string listUserStarredRepo(const std::string& username);

            // watching related api

            /**
             * @brief Lists the people watching the specified repository.
             *
             * @param owner The account owner of the repository
             * @param repo The name of the repository
             * @return list of github account watching the repository
             */
            std::string listRepoWatchers(const std::string& owner, const std::string& repo);

            /**
             * @brief Get a repository subscription.
             *
             * @param owner The account owner of the repository
             * @param repo The name of the repository
             * @return list of users subscribe to the repository
             */
            std::string getRepoSubscription(const std::string& owner, const std::string& repo);

            /**
             * @brief List repositories watched by a user
             *
             * @param username The handle for the GitHub user account
             * @return Lists repositories a user is watching.
             */
            std::string listUserWatchedRepos(const std::string& username);

            /**
             * @brief List repository collaborators
             *
             * @param owner The account owner of the repository
             * @param repo The name of the repository
             * @return  list of collaborators includes outside collaborators,
             *          organization members that are direct collaborators,
             *          organization members with access through team memberships,
             *          organization members with access through default
             *          organization permissions, and organization owners
             */
            std::string listRepoCollaborators(const std::string& owner, const std::string& repo);

            /**
             * @brief List organization repositories
             *
             * @param org The organization name
             * @return Lists repositories for the specified organization.
             */
            std::string getOrgRepo(const std::string& org);

            /**
             * @brief Get a repository
             *
             * @param owner The account owner of the repository
             * @param repo The name of the repository
             * @return The parent and source objects are present
             *          when the repository is a fork. parent is
             *          the repository this repository was forked from,
             *          source is the ultimate source for the network.
             */
            std::string getRepository(const std::string& owner, const std::string& repo);

            /**
             * @brief List repositories for the authenticated user
             *
             * @return Lists repositories that the authenticated user
             *         has explicit permission (:read, :write, or :admin)
             *         to access.
             */
            std::string listAuthUserRepo();

            /**
             * @brief List repository languages
             *
             * @param owner The account owner of the repository
             * @param repo The name of the repository
             * @return Lists languages for the specified repository.
             *         The value shown for each language is the number
             *         of bytes of code written in that language.
             */
            std::string getRepoLang(const std::string& owner, const std::string& repo);

            /**
             * @brief List repository contributors
             *
             * @param owner The account owner of the repository
             * @param repo The name of the repository
             * @return Lists contributors to the specified repository
             *         and sorts them by the number of commits per
             *         contributor in descending order
             */
            std::string repoContributors(const std::string& owner, const std::string& repo);

        private:
            std::unique_ptr<cpp_restapi::IConnection> m_connection;

            std::string doRequest(const std::string &);
    };
}

#endif // REQUEST_H
