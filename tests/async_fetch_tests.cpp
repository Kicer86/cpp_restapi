#include <gmock/gmock.h>

#include <chrono>
#include <future>
#include <thread>

#include <cpp_restapi/link_header_pagination_strategy.hpp>

#include "test_stubs.hpp"

using namespace cpp_restapi;
using test_stubs::StubConnection;
using test_stubs::ThrowingStubConnection;
using test_stubs::SlowStubConnection;


// --- async fetch(request, onSuccess, onError) ---

TEST(AsyncFetchTest, callsOnSuccessWithBodyAndHeaders)
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

TEST(AsyncFetchTest, callsOnErrorWhenFetchPageThrows)
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

TEST(AsyncFetchTest, missingErrorCallbackDoesNotCrashOnException)
{
    ThrowingStubConnection conn("http://localhost", {});

    conn.fetch("api/data", {}, {});

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (!conn.fetchPageCalled.load(std::memory_order_acquire) &&
           std::chrono::steady_clock::now() < deadline)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_TRUE(conn.fetchPageCalled.load());
}


// --- cancellation ---

TEST(AsyncFetchTest, cancelledFetchSuppressesCallbacks)
{
    SlowStubConnection conn("http://localhost", {});

    std::atomic<bool> callbackInvoked{false};

    auto token = conn.fetch("api/data",
        [&callbackInvoked](Response) { callbackInvoked.store(true); },
        [&callbackInvoked](HttpError)   { callbackInvoked.store(true); });

    token->store(true, std::memory_order_release);
}

TEST(AsyncFetchTest, cancelledFetchCallbacksNotInvoked)
{
    std::atomic<bool> callbackInvoked{false};

    {
        SlowStubConnection conn("http://localhost", {});

        auto token = conn.fetch("api/data",
            [&callbackInvoked](Response) { callbackInvoked.store(true); },
            [&callbackInvoked](HttpError)   { callbackInvoked.store(true); });

        token->store(true, std::memory_order_release);
    }
    EXPECT_FALSE(callbackInvoked.load());
}


// --- async fetch with pagination strategy ---

TEST(AsyncFetchTest, paginatedFetchCollectsAllPages)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items",
        {R"([{"id":1}])", R"(Link: <http://localhost/items?page=2>; rel="next")" "\r\n", 200});
    conn.addPage("http://localhost/items?page=2",
        {R"([{"id":2}])", R"(Link: <http://localhost/items?page=3>; rel="next")" "\r\n", 200});
    conn.addPage("http://localhost/items?page=3",
        {R"([{"id":3}])", "", 200});

    LinkHeaderPaginationStrategy strategy;

    std::promise<std::string> promise;
    auto future = promise.get_future();

    conn.fetch("items", strategy,
        [&promise](std::string merged) { promise.set_value(std::move(merged)); });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    EXPECT_EQ(future.get(), "[{\"id\":1},{\"id\":2},{\"id\":3}]\n");
}

TEST(AsyncFetchTest, paginatedFetchStopsOnError)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items",
        {R"([{"id":1}])", R"(Link: <http://localhost/items?page=2>; rel="next")" "\r\n", 200});
    conn.addPage("http://localhost/items?page=2",
        {"Internal Server Error", "", 500});

    LinkHeaderPaginationStrategy strategy;

    std::promise<HttpError> promise;
    auto future = promise.get_future();

    conn.fetch("items", strategy,
        [](std::string) { FAIL() << "onSuccess should not be called"; },
        [&promise](HttpError err) { promise.set_value(std::move(err)); });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    EXPECT_EQ(future.get().statusCode, 500);
}

TEST(AsyncFetchTest, paginatedFetchSinglePage)
{
    StubConnection conn("http://localhost", {});
    conn.addPage("http://localhost/items", {R"({"key":"value"})", "", 200});

    LinkHeaderPaginationStrategy strategy;

    std::promise<std::string> promise;
    auto future = promise.get_future();

    conn.fetch("items", strategy,
        [&promise](std::string merged) { promise.set_value(std::move(merged)); });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    EXPECT_EQ(future.get(), "{\"key\":\"value\"}\n");
}
