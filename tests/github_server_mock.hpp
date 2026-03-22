
#ifndef GITHUB_SERVER_MOCK_HPP_INCLUDED
#define GITHUB_SERVER_MOCK_HPP_INCLUDED

#include <atomic>
#include <thread>
#include <vector>
#include <gmock/gmock.h>
#include <httplib.h>

class GithubServerMock
{
  public:
    struct Response
    {
        std::string      body;
        httplib::Headers headers;
        int              statusCode = 200;
    };

    struct SseEvent
    {
        std::string event;
        std::string data;
        std::string id;
    };

    explicit GithubServerMock(int port = 9200)
      : m_port(port)
    {
      m_svr.set_socket_options([](socket_t sock) {
        int yes = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&yes), sizeof(yes));
      });
    }

    ~GithubServerMock()
    {
      m_svr.stop();
      if (m_svrThread.joinable())
        m_svrThread.join();
    }

    void setSseEvents(std::vector<SseEvent> events)
    {
      m_sseEvents = std::move(events);
    }

    void listen()
    {
      m_svr.Get("/sse", [this](const httplib::Request&, httplib::Response& res)
      {
        std::string payload;
        for (const auto& ev : m_sseEvents)
        {
          if (!ev.event.empty())
            payload += "event: " + ev.event + "\n";
          if (!ev.id.empty())
            payload += "id: " + ev.id + "\n";
          payload += "data: " + ev.data + "\n\n";
        }

        res.set_content(payload, "text/event-stream");
      });

      m_svr.Get("/.*", [this](const httplib::Request& req, httplib::Response& res)
      {
        const auto response = request(req.path, req.headers);
        res.status = response.statusCode;
        res.set_content(response.body, "text/plain");

        for(const auto& [key, value]: response.headers)
          res.set_header(key, value);
      });

      m_svrThread = std::thread([this]()
      {
        m_svr.listen("127.0.0.1", m_port);
      });

      // wait for server to be ready
      m_svr.wait_until_ready();
    }

    MOCK_METHOD(Response, request, (const std::string& path, const httplib::Headers& header), (const));

  private:
    httplib::Server m_svr;
    std::thread m_svrThread;
    int m_port;
    std::vector<SseEvent> m_sseEvents;
};

#endif
