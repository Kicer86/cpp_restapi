#include <cpp_restapi/curl_sse_connection.hpp>

#include <curl/curl.h>


namespace cpp_restapi::CurlBackend
{

SseConnection::SseConnection(const std::string& address,
                             const std::map<std::string, std::string>& headerEntries)
    : m_address(address)
    , m_headerEntries(headerEntries)
{
}


SseConnection::~SseConnection()
{
    close();
}


void SseConnection::subscribe(const std::string& request, EventCallback callback)
{
    m_callback = std::move(callback);
    m_parser = SseParser{};
    m_running = true;

    const std::string fullUrl = m_address + "/" + request;

    CURL* curl = curl_easy_init();

    if (curl)
    {
        struct WriteContext
        {
            SseParser* parser;
            ISseConnection::EventCallback* callback;
            bool* running;
        };

        WriteContext ctx{&m_parser, &m_callback, &m_running};

        typedef size_t (*WriteCallback)(char* ptr, size_t size, size_t nmemb, void* userdata);
        WriteCallback write_callback = [](char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t
        {
            auto* ctx = static_cast<WriteContext*>(userdata);

            if (!*ctx->running)
                return 0;  // returning 0 aborts the transfer

            const std::string chunk(ptr, size * nmemb);
            const auto events = ctx->parser->feed(chunk);

            for (const auto& event : events)
                (*ctx->callback)(event);

            return size * nmemb;
        };

        curl_slist* list = nullptr;

        for (const auto& [k, v] : m_headerEntries)
        {
            const std::string entry = k + ": " + v;
            list = curl_slist_append(list, entry.c_str());
        }

        list = curl_slist_append(list, "Accept: text/event-stream");

        curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "cpp_restapi/2.0");

        curl_easy_perform(curl);

        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
    }

    m_running = false;
}


void SseConnection::close()
{
    m_running = false;
    m_callback = nullptr;
}

}
