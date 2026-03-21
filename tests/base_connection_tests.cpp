
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
            int statusCode = 200;
        };

        void addPage(const std::string& url, PageResponse response)
        {
            m_pages[url] = std::move(response);
        }

        Response fetchPage(const std::string& request) override
        {
            auto it = m_pages.find(request);
            if (it != m_pages.end())
                return {it->second.body, it->second.headers, it->second.statusCode};

            return {"" , "", 0};
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
    conn.addPage("http://localhost/api/data", {"response body", "", 200});

    const auto result = conn.fetch("api/data");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "response body");
}

TEST(BaseConnectionTest, fetchResponseReturnsBodyAndHeaders)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/data", {"body", "Content-Type: application/json\r\n", 200});

    const auto resp = conn.fetchResponse("http://localhost/api/data");
    ASSERT_TRUE(resp.has_value());
    EXPECT_EQ(resp->body, "body");
    EXPECT_EQ(resp->headers, "Content-Type: application/json\r\n");
    EXPECT_EQ(resp->statusCode, 200);
}

TEST(BaseConnectionTest, fetchWithStrategyCollectsAllPages)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items",
        {R"([{"id":1}])", R"(Link: <http://localhost/items?page=2>; rel="next")" "\r\n", 200});
    conn.addPage("http://localhost/items?page=2",
        {R"([{"id":2}])", R"(Link: <http://localhost/items?page=3>; rel="next")" "\r\n", 200});
    conn.addPage("http://localhost/items?page=3",
        {R"([{"id":3}])", "", 200});

    LinkHeaderPaginationStrategy strategy;
    const auto result = conn.fetch("items", strategy);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "[{\"id\":1},{\"id\":2},{\"id\":3}]\n");
}

TEST(BaseConnectionTest, fetchWithStrategySinglePage)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items", {R"({"key":"value"})", "", 200});

    LinkHeaderPaginationStrategy strategy;
    const auto result = conn.fetch("items", strategy);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "{\"key\":\"value\"}\n");
}

TEST(BaseConnectionTest, fetchReturnsNetworkError)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/data", {"", "", 0});

    const auto result = conn.fetch("api/data");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().statusCode, 0);
}

TEST(BaseConnectionTest, fetchReturnsHttpError404)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/data", {"Not Found", "", 404});

    const auto result = conn.fetch("api/data");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().statusCode, 404);
    EXPECT_EQ(result.error().body, "Not Found");
}

TEST(BaseConnectionTest, fetchWithStrategyStopsOnError)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items",
        {R"([{"id":1}])", R"(Link: <http://localhost/items?page=2>; rel="next")" "\r\n", 200});
    conn.addPage("http://localhost/items?page=2",
        {"Internal Server Error", "", 500});

    LinkHeaderPaginationStrategy strategy;
    const auto result = conn.fetch("items", strategy);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().statusCode, 500);
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
