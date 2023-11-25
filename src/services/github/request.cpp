
#include <cpp_restapi/github/request.hpp>


// Based on:
// https://developer.github.com/guides/getting-started/
// https://developer.github.com/v3/


namespace cpp_restapi::GitHub
{

    Request::Request(std::unique_ptr<cpp_restapi::IConnection> connection)
        : m_connection(std::move(connection))
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

    std::string Request::listUserRepo(const std::string& username)
    {
        const std::string request = std::string("users/") + username +"/repos";
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


    std::string Request::getAuntenticatedUser()
    {
        const std::string request = std::string("user");
        return doRequest(request);
    }

    std::string Request::listUsers()
    {
        const std::string request = std::string("users");
        return doRequest(request);
    }

    std::string Request::getUser(const std::string& username)
    {
        const std::string request = std::string("users/") + username;
        return doRequest(request);
    }

    std::string Request::issues()
    {
        const std::string request = std::string("issues");
        return doRequest(request);
    }

    std::string Request::orgIssues(const std::string& org)
    {
        const std::string request = std::string("orgs/") + org + "/issues";
        return doRequest(request);
    }

    std::string Request::listRepoIssues(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/issues";
        return doRequest(request);
    }

    std::string Request::getIssue(const std::string& owner, const std::string& repo, const std::string& issueNumber)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/issues/" + issueNumber;
        return doRequest(request);
    }

    std::string Request::listPullRequest(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/pulls";
        return doRequest(request);
    }

    std::string Request::getPullRequest(const std::string& owner, const std::string& repo, const std::string& pullNumber)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/pulls/" + pullNumber;
        return doRequest(request);
    }

    std::string Request::listPullRequestCommit(const std::string& owner,const std::string& repo, const std::string& pullNumber)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/pulls/" + pullNumber + "/commits";
        return doRequest(request);
    }

    std::string Request::listPullRequestfiles(const std::string& owner, const std::string& repo, const std::string& pullNumber)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/pulls/" + pullNumber + "/files";
        return doRequest(request);
    }

    std::string Request::isPRmerged(const std::string& owner, const std::string& repo, const std::string& pullNumber)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/pulls/"+pullNumber+"/merge";
        return doRequest(request);
    }

    std::string Request::listCommits(const std::string& owner, const std::string& repo) {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/issues";
        return doRequest(request);
    }

    std::string Request::listBranchHeadCommit(const std::string& owner, const std::string& repo, const std::string& commitSha)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/commits/" + commitSha + "/branches-where-head";
        return doRequest(request);
    }

    std::string Request::listCommitPullRequest(const std::string& owner, const std::string& repo, const std::string& commitSha)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/commits/" + commitSha + "/pulls";
        return doRequest(request);
    }

    std::string Request::getCommits(const std::string& owner, const std::string& repo, const std::string& reference)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/commits/"+reference;
        return doRequest(request);
    }

    std::string Request::getWeeklyCommit(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/stats/code_frequency";
        return doRequest(request);
    }

    std::string Request::getLastYearCommit(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/stats/commit_activity";
        return doRequest(request);
    }

    std::string Request::getcontributorsActivity(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/stats/contributors";
        return doRequest(request);
    }

    std::string Request::getCommitCount(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/stats/participation";
        return doRequest(request);
    }

    std::string Request::getHourlyCommitCount(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/stats/punch_card";
        return doRequest(request);
    }

    std::string Request::getCommunityProfileMetrics(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner +"/"+ repo + "/community/profile";
        return doRequest(request);
    }

    std::string Request::getRepoClones(const std::string& owner,const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/traffic/clones";
        return doRequest(request);
    }

    std::string Request::getRefferalPaths(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/traffic/popular/paths";
        return doRequest(request);
    }

    std::string Request::getTopreferralSource(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/traffic/popular/referrers";
        return doRequest(request);
    }

    std::string Request::getPageViews(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/traffic/views";
        return doRequest(request);
    }

    std::string Request::listNetworkRepoEvent(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("network/") + owner + "/" + repo + "/events";
        return doRequest(request);
    }

    std::string Request::listOrgEvent(const std::string& org)
    {
        const std::string request = std::string("orgs/") + org + "/" + "/events";
        return doRequest(request);
    }

    std::string Request::listRepoEvent(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/events";
        return doRequest(request);
    }

    std::string Request::listUserEvent(const std::string& username)
    {
        const std::string request = std::string("users/") + username + "/events";
        return doRequest(request);
    }

    std::string Request::listStargazers(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/stargazers";
        return doRequest(request);
    }

    std::string Request::listUserStarredRepo(const std::string& username)
    {
        const std::string request = std::string("users/") + username
                                    + "/starred";
        return doRequest(request);
    }

    std::string Request::listRepoWatchers(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/subscribers";
        return doRequest(request);
    }


    std::string Request::getRepoSubscription(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/subscription";
        return doRequest(request);
    }

    std::string Request::listUserWatchedRepos(const std::string& username)
    {
        const std::string request = std::string("users/") + username + "/subscription";
        return doRequest(request);
    }

    std::string Request::listRepoCollaborators(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/collaborators";
        return doRequest(request);
    }

    std::string Request::getOrgRepo(const std::string& org)
    {
        const std::string request = std::string("orgs/") + org + "/repos";
        return doRequest(request);
    }

    std::string Request::getRepository(const std::string& owner, const std::string& repo)
    {
        const std::string request = std::string("repos/") + owner + "/" + repo;
        return doRequest(request);
    }

    std::string Request::listAuthUserRepo()
    {
        const std::string request = std::string("user/repos");
        return doRequest(request);
    }

    std::string Request::getRepoLang(const std::string& owner, const std::string& repo) {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/languages";
        return doRequest(request);
    }

    std::string Request::repoContributors(const std::string& owner, const std::string& repo) {
        const std::string request = std::string("repos/") + owner + "/" + repo + "/contributors";
        return doRequest(request);
    }


    std::string Request::doRequest(const std::string& request)
    {
        return m_connection->get(request);
    }
}
