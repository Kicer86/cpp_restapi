
#include <gmock/gmock.h>

#include <chrono>
#include <future>
#include <thread>

#include <cpp_restapi/threaded_connection.hpp>
#include <cpp_restapi/link_header_pagination_strategy.hpp>

using namespace cpp_restapi;


namespace
{
    class StubConnection: public ThreadedConnection
    {
    public:
        using ThreadedConnection::ThreadedConnection;

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


    class ThrowingStubConnection : public ThreadedConnection
    {
    public:
        using ThreadedConnection::ThreadedConnection;

        std::atomic<bool> fetchPageCalled{false};

        Response fetchPage(const std::string&) override
        {
            fetchPageCalled.store(true, std::memory_order_release);
            throw std::runtime_error("simulated fetch error");
        }

        std::unique_ptr<ISseConnection> subscribe(const std::string&, EventCallback) override
        {
            return nullptr;
        }
    };


    class SlowStubConnection : public ThreadedConnection
    {
    public:
        using ThreadedConnection::ThreadedConnection;

        ~SlowStubConnection() override { waitForPending(); }

        Response fetchPage(const std::string&) override
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return {"slow body", "", 200};
        }

        std::unique_ptr<ISseConnection> subscribe(const std::string&, EventCallback) override
        {
            return nullptr;
        }
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


// --- async fetch(url, onSuccess, onError) ---

TEST(ThreadedConnectionTest, fetchCallsOnSuccessWithBodyAndHeaders)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/data", {"async body", "X-Custom: header\n"});

    std::promise<Response> promise;
    auto future = promise.get_future();

    conn.fetch("api/data",
        [&promise](Response resp) { promise.set_value(std::move(resp)); });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    const auto resp = future.get();
    EXPECT_EQ(resp.body,    "async body");
    EXPECT_EQ(resp.headers, "X-Custom: header\n");
}

TEST(ThreadedConnectionTest, fetchCallsOnErrorWhenFetchPageThrows)
{
    ThrowingStubConnection conn("http://localhost", {});

    std::promise<HttpError> promise;
    auto future = promise.get_future();

    conn.fetch("api/data",
        [](Response) {},
        [&promise](HttpError err) { promise.set_value(std::move(err)); });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    const auto err = future.get();
    EXPECT_EQ(err.statusCode, 0);
    EXPECT_EQ(err.message, "simulated fetch error");
}

TEST(ThreadedConnectionTest, fetchWithoutErrorCallbackDoesNotCrashOnException)
{
    ThrowingStubConnection conn("http://localhost", {});

    // Pass no error callback — exception must be swallowed silently.
    conn.fetch("api/data", {}, {});

    // Spin-wait until fetchPage() has been entered (and the throw issued).
    // Once fetchPageCalled is true, 'this' is no longer accessed by the thread,
    // so destroying 'conn' at scope exit is safe.
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (!conn.fetchPageCalled.load(std::memory_order_acquire) &&
           std::chrono::steady_clock::now() < deadline)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_TRUE(conn.fetchPageCalled.load());
    // Reaching here without a crash confirms the missing callback is handled safely.
}

TEST(ThreadedConnectionTest, cancelledFetchSuppressesCallbacks)
{
    SlowStubConnection conn("http://localhost", {});

    std::atomic<bool> callbackInvoked{false};

    auto token = conn.fetch("api/data",
        [&callbackInvoked](Response) { callbackInvoked.store(true); },
        [&callbackInvoked](HttpError)   { callbackInvoked.store(true); });

    // Cancel immediately, before the slow fetchPage() completes.
    token->store(true, std::memory_order_release);

    // Wait for the background thread to finish — destructor joins.
    // After that, neither callback should have been invoked.
    // (SlowStubConnection's destructor waits for active threads.)
}

TEST(ThreadedConnectionTest, cancelledFetchCallbacksNotInvoked)
{
    std::atomic<bool> callbackInvoked{false};

    {
        SlowStubConnection conn("http://localhost", {});

        auto token = conn.fetch("api/data",
            [&callbackInvoked](Response) { callbackInvoked.store(true); },
            [&callbackInvoked](HttpError)   { callbackInvoked.store(true); });

        token->store(true, std::memory_order_release);
    }
    // conn destroyed — destructor waited for the thread.
    EXPECT_FALSE(callbackInvoked.load());
}
