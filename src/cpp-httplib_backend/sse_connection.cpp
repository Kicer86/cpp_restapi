#include "cpp-httplib_sse_connection.hpp"

#include <httplib.h>


namespace cpp_restapi::CppHttplibBackend
{

SseConnection::SseConnection(const std::string& address,
                             const std::map<std::string, std::string>& headerEntries)
    : m_address(address)
    , m_headerEntries(headerEntries)
{
}


SseConnection::~SseConnection()
{
    closeImpl();
}


void SseConnection::subscribe(const std::string& request, EventCallback callback)
{
    closeImpl();

    m_running = true;
    m_thread = std::thread(&SseConnection::run, this, request, std::move(callback));
}


void SseConnection::close()
{
    closeImpl();
}


void SseConnection::closeImpl()
{
    m_running = false;

    if (m_thread.joinable() && std::this_thread::get_id() != m_thread.get_id())
        m_thread.join();
}


void SseConnection::run(const std::string& request, EventCallback callback)
{
    SseParser parser;

    auto pos = m_address.find("//");
    if (pos == std::string::npos)
        pos = 0;
    else
        pos += 2;

    pos = m_address.find("/", pos);

    std::string server;
    std::string basePath;

    if (pos == std::string::npos)
    {
        server = m_address;
    }
    else
    {
        server = m_address.substr(0, pos);
        basePath = m_address.substr(pos);
    }

    const std::string query = basePath + "/" + request;

    httplib::Client cli(server);

    httplib::Headers headers(m_headerEntries.begin(), m_headerEntries.end());
    headers.emplace("Accept", "text/event-stream");

    cli.Get(query, headers,
        [this, &parser, &callback](const char* data, size_t len) -> bool
        {
            if (!m_running)
                return false;

            const std::string chunk(data, len);
            const auto events = parser.feed(chunk);

            for (const auto& event : events)
                callback(event);

            return m_running.load();
        }
    );
}

}
