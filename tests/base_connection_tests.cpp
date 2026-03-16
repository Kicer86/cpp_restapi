
#include <gmock/gmock.h>

#include <cpp_restapi/base_connection.hpp>
#include <cpp_restapi/link_header_pagination_strategy.hpp>

using namespace cpp_restapi;


namespace
{
    class StubConnection: public BaseConnection
    {
    public:
        using BaseConnection::BaseConnection;

        struct PageResponse
        {
            std::string body;
            std::string headers;
        };

        void addPage(const std::string& url, PageResponse response)
        {
            m_pages[url] = std::move(response);
        }

        std::pair<std::string, std::string> fetchPage(const std::string& request) override
        {
            auto it = m_pages.find(request);
            if (it != m_pages.end())
                return {it->second.body, it->second.headers};

            return {"", ""};
        }

        std::unique_ptr<ISseConnection> subscribe(const std::string&, EventCallback) override
        {
            return nullptr;
        }

    private:
        std::map<std::string, PageResponse> m_pages;
    };
}


TEST(BaseConnectionTest, fetchReturnsSinglePageBody)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/data", {"response body", ""});

    const auto result = conn.fetch("api/data");
    EXPECT_EQ(result, "response body");
}

TEST(BaseConnectionTest, fetchResponseReturnsBodyAndHeaders)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/data", {"body", "Content-Type: application/json\r\n"});

    const auto resp = conn.fetchResponse("http://localhost/api/data");
    EXPECT_EQ(resp.body, "body");
    EXPECT_EQ(resp.headers, "Content-Type: application/json\r\n");
}

TEST(BaseConnectionTest, fetchWithStrategyCollectsAllPages)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items",
        {R"([{"id":1}])", R"(Link: <http://localhost/items?page=2>; rel="next")" "\r\n"});
    conn.addPage("http://localhost/items?page=2",
        {R"([{"id":2}])", R"(Link: <http://localhost/items?page=3>; rel="next")" "\r\n"});
    conn.addPage("http://localhost/items?page=3",
        {R"([{"id":3}])", ""});

    LinkHeaderPaginationStrategy strategy;
    const auto result = conn.fetch("items", strategy);
    EXPECT_EQ(result, "[{\"id\":1},{\"id\":2},{\"id\":3}]\n");
}

TEST(BaseConnectionTest, fetchWithStrategySinglePage)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items", {R"({"key":"value"})", ""});

    LinkHeaderPaginationStrategy strategy;
    const auto result = conn.fetch("items", strategy);
    EXPECT_EQ(result, "{\"key\":\"value\"}\n");
}

TEST(BaseConnectionTest, getDelegatesToFetchWithPagination)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items",
        {R"({"a":1})", R"(Link: <http://localhost/items?page=2>; rel="next")" "\r\n"});
    conn.addPage("http://localhost/items?page=2",
        {R"({"b":2})", ""});

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    const auto result = conn.get("items");
#pragma GCC diagnostic pop
    EXPECT_EQ(result, "{\"a\":1,\"b\":2}\n");
}
