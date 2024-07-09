
#ifndef GITHUB_SERVER_MOCK_HPP_INCLUDED
#define GITHUB_SERVER_MOCK_HPP_INCLUDED

#include <thread>
#include <gmock/gmock.h>
#include <httplib.h>

class GithubServerMock
{
  public:
    using Response = std::pair<std::string, httplib::Headers>;

    explicit GithubServerMock(int port = 9200)
      : m_port(port)
    {

    }

    ~GithubServerMock()
    {
      httplib::Client cli("localhost", m_port);
      m_svr.stop();

      m_svrThread.join();
    }

    void listen()
    {
      m_svr.Get("/.*", [this](const httplib::Request& req, httplib::Response& res)
      {
        const auto response = request(req.path, req.headers);
        res.set_content(response.first, "text/plain");

        for(const auto& [key, value]: response.second)
          res.set_header(key, value);
      });

      m_svrThread = std::thread([this]()
      {
        m_svr.listen("localhost", m_port);
      });

      // wait for server to be ready
      m_svr.wait_until_ready();
    }

    MOCK_METHOD(Response, request, (const std::string& path, const httplib::Headers& header), (const));

  private:
    httplib::Server m_svr;
    std::thread m_svrThread;
    int m_port;
};

#endif
