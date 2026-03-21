/// @file cpp-httplib_coroutine_example.cpp
/// @brief Demonstrates using C++20 coroutines with the cpp-httplib backend.
///
/// See curl_coroutine_example.cpp for a detailed explanation of the
/// FetchAwaitable adapter pattern used here.

#include <coroutine>
#include <expected>
#include <future>
#include <iostream>

#include <cpp_restapi/cpp-httplib_connection.hpp>


// ---------------------------------------------------------------------------
// Minimal awaitable adapter — wraps fetch(request, onSuccess, onError)
// ---------------------------------------------------------------------------

struct FetchAwaitable
{
    FetchAwaitable(cpp_restapi::IConnection& conn, std::string req)
        : connection(conn), request(std::move(req)) {}

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> h)
    {
        cancel = connection.fetch(request,
            [this, h](cpp_restapi::Response resp) mutable
            {
                result.emplace(std::move(resp));
                h.resume();
            },
            [this, h](cpp_restapi::HttpError err) mutable
            {
                result.emplace(std::unexpected(std::move(err)));
                h.resume();
            });
    }

    std::expected<cpp_restapi::Response, cpp_restapi::HttpError> await_resume()
    {
        return std::move(*result);
    }

private:
    cpp_restapi::IConnection& connection;
    std::string request;
    std::optional<std::expected<cpp_restapi::Response, cpp_restapi::HttpError>> result;
    cpp_restapi::CancellationToken cancel;
};

FetchAwaitable fetch(cpp_restapi::IConnection& conn, std::string request)
{
    return {conn, std::move(request)};
}


// ---------------------------------------------------------------------------
// Minimal coroutine Task type
// ---------------------------------------------------------------------------

struct Task
{
    struct promise_type
    {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};


// ---------------------------------------------------------------------------
// The actual coroutine
// ---------------------------------------------------------------------------

Task run(cpp_restapi::IConnection& conn, std::promise<void>& done)
{
    std::cout << "Fetching Luke Skywalker...\n";
    auto luke = co_await fetch(conn, "people/1");
    if (luke)
        std::cout << "Luke: " << luke->body << "\n\n";
    else
        std::cerr << "Error: " << luke.error().message << '\n';

    std::cout << "Fetching Millennium Falcon...\n";
    auto falcon = co_await fetch(conn, "starships/10");
    if (falcon)
        std::cout << "Falcon: " << falcon->body << "\n\n";
    else
        std::cerr << "Error: " << falcon.error().message << '\n';

    done.set_value();
}


int main()
{
    cpp_restapi::CppHttplibBackend::Connection connection("https://swapi.dev/api", {});

    std::promise<void> done;
    auto future = done.get_future();

    run(connection, done);

    future.wait();

    return 0;
}
