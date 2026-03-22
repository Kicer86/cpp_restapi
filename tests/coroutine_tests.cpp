
#include <chrono>
#include <future>

#include <gmock/gmock.h>

#include <cpp_restapi/coroutine.hpp>
#include <cpp_restapi/link_header_pagination_strategy.hpp>

#include "test_stubs.hpp"

using namespace cpp_restapi;
using test_stubs::StubConnection;
using test_stubs::ThrowingStubConnection;


namespace
{

    // Fire-and-forget coroutine driver for tests.
    // Starts eagerly, self-destroys on completion.
    struct SyncRunner
    {
        struct promise_type
        {
            SyncRunner get_return_object() { return {}; }
            std::suspend_never initial_suspend() noexcept { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { throw; }
        };
    };

}


TEST(CoroutineTest, coFetchReturnsSuccessfulResponse)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/data", {"coroutine body", "X-Hdr: val\n", 200});

    std::promise<std::expected<Response, HttpError>> promise;
    auto future = promise.get_future();

    [](IConnection& c, auto& p) -> SyncRunner
    {
        auto result = co_await coFetch(c, "api/data");
        p.set_value(std::move(result));
    }(conn, promise);

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    auto result = future.get();

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->body, "coroutine body");
    EXPECT_EQ(result->headers, "X-Hdr: val\n");
    EXPECT_EQ(result->statusCode, 200);
}


TEST(CoroutineTest, coFetchReturnsErrorOnFailure)
{
    ThrowingStubConnection conn("http://localhost", {});

    std::promise<std::expected<Response, HttpError>> promise;
    auto future = promise.get_future();

    [](IConnection& c, auto& p) -> SyncRunner
    {
        auto result = co_await coFetch(c, "api/data");
        p.set_value(std::move(result));
    }(conn, promise);

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    auto result = future.get();

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().statusCode, 0);
    EXPECT_EQ(result.error().message, "simulated fetch error");
}


TEST(CoroutineTest, coFetchWithPaginationMergesPages)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/api/items",
                 {R"([{"id":1}])", "Link: <http://localhost/api/items?page=2>; rel=\"next\"\r\n", 200});
    conn.addPage("http://localhost/api/items?page=2",
                 {R"([{"id":2}])", "", 200});

    std::promise<std::expected<std::string, HttpError>> promise;
    auto future = promise.get_future();

    LinkHeaderPaginationStrategy strategy;

    [](IConnection& c, IPaginationStrategy& s, auto& p) -> SyncRunner
    {
        auto result = co_await coFetch(c, "api/items", s);
        p.set_value(std::move(result));
    }(conn, strategy, promise);

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    auto result = future.get();

    ASSERT_TRUE(result.has_value());
    EXPECT_THAT(*result, testing::HasSubstr("\"id\":1"));
    EXPECT_THAT(*result, testing::HasSubstr("\"id\":2"));
}
