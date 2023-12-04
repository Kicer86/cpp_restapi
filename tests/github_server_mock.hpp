
#ifndef GITHUB_SERVER_MOCK_HPP_INCLUDED
#define GITHUB_SERVER_MOCK_HPP_INCLUDED

#include <thread>
#include <gmock/gmock.h>
#include <httplib.h>

class GithubServerMock
{
  public:
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

      m_svrThread = std::thread([this]()
      {
        m_svr.listen("localhost", m_port);
      });

      // wait for server to be ready
      m_svr.wait_until_ready();
    }

    void responseHandler(const std::string& request, int status, std::string response, const std::map<std::string, std::string>& header = {})
    {
      assert(not m_svrThread.joinable());

      m_svr.Get(request, [response, header](const httplib::Request& req, httplib::Response& res)
      {
        res.set_content(response, "text/plain");

        for (const auto& [key, value]: header)
          res.set_header(key, value);
      });
    }

  private:
    httplib::Server m_svr;
    std::thread m_svrThread;
    int m_port;
};

#endif
