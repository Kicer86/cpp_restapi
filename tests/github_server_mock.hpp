
#ifndef GITHUB_SERVER_MOCK_HPP_INCLUDED
#define GITHUB_SERVER_MOCK_HPP_INCLUDED


#include <gmock/gmock.h>
#include <httpmockserver/mock_server.h>

class GithubServerMock: public httpmock::MockServer {
  public:
    explicit GithubServerMock(int port = 9200): MockServer(port) {}

    using httpmock::MockServer::Response;

    MOCK_METHOD(Response,
                responseHandler,
                (const std::string &,
                 const std::string &,
                 const std::string &,
                 const std::vector<UrlArg> &,
                 const std::vector<Header> &),
                (override)
    );
};

#endif
