
#ifndef COROUTINE_HPP_INCLUDED
#define COROUTINE_HPP_INCLUDED

#include <coroutine>
#include <expected>
#include <optional>
#include <string>
#include <utility>

#include "iconnection.hpp"

namespace cpp_restapi
{

    /**
     * @brief Lazy coroutine task that produces a value of type T
     *
     * Suspends at creation; starts executing when co_awaited.
     * Move-only.
     *
     * @code
     * Task<std::expected<Response, HttpError>> task = coFetch(conn, "users/1");
     * auto result = co_await std::move(task);
     * @endcode
     */
    template<typename T>
    class Task
    {
    public:
        struct promise_type
        {
            std::optional<T> value;
            std::coroutine_handle<> continuation;

            Task get_return_object()
            {
                return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
            }

            std::suspend_always initial_suspend() noexcept { return {}; }

            auto final_suspend() noexcept
            {
                struct FinalAwaiter
                {
                    bool await_ready() noexcept { return false; }

                    std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept
                    {
                        auto cont = h.promise().continuation;
                        return cont ? cont : std::noop_coroutine();
                    }

                    void await_resume() noexcept {}
                };

                return FinalAwaiter{};
            }

            void return_value(T v) { value.emplace(std::move(v)); }
            void unhandled_exception() { throw; }
        };

        Task(Task&& other) noexcept : m_handle(std::exchange(other.m_handle, {})) {}

        Task& operator=(Task&& other) noexcept
        {
            if (this != &other)
            {
                if (m_handle)
                    m_handle.destroy();

                m_handle = std::exchange(other.m_handle, {});
            }

            return *this;
        }

        ~Task()
        {
            if (m_handle)
                m_handle.destroy();
        }

        // -- awaitable interface --

        bool await_ready() const noexcept { return m_handle.done(); }

        std::coroutine_handle<> await_suspend(std::coroutine_handle<> h) noexcept
        {
            m_handle.promise().continuation = h;
            return m_handle;
        }

        T await_resume() { return std::move(*m_handle.promise().value); }

    private:
        explicit Task(std::coroutine_handle<promise_type> h) : m_handle(h) {}
        std::coroutine_handle<promise_type> m_handle;
    };

    namespace detail
    {

        /**
         * @brief Awaitable wrapping IConnection::fetch(request, onSuccess, onError)
         */
        struct FetchAwaitable
        {
            IConnection& connection;
            std::string request;
            std::optional<std::expected<Response, HttpError>> result;
            CancellationToken cancel;

            bool await_ready() const noexcept { return false; }

            void await_suspend(std::coroutine_handle<> h)
            {
                cancel = connection.fetch(request,
                    [this, h](Response resp) mutable
                    {
                        result.emplace(std::move(resp));
                        h.resume();
                    },
                    [this, h](HttpError err) mutable
                    {
                        result.emplace(std::unexpected(std::move(err)));
                        h.resume();
                    });
            }

            std::expected<Response, HttpError> await_resume()
            {
                return std::move(*result);
            }
        };

        /**
         * @brief Awaitable wrapping paginated IConnection::fetch
         */
        struct PaginatedFetchAwaitable
        {
            IConnection& connection;
            std::string request;
            IPaginationStrategy& strategy;
            std::optional<std::expected<std::string, HttpError>> result;
            CancellationToken cancel;

            bool await_ready() const noexcept { return false; }

            void await_suspend(std::coroutine_handle<> h)
            {
                cancel = connection.fetch(request, strategy,
                    [this, h](std::string body) mutable
                    {
                        result.emplace(std::move(body));
                        h.resume();
                    },
                    [this, h](HttpError err) mutable
                    {
                        result.emplace(std::unexpected(std::move(err)));
                        h.resume();
                    });
            }

            std::expected<std::string, HttpError> await_resume()
            {
                return std::move(*result);
            }
        };

    } // namespace detail

    /**
     * @brief Asynchronously fetch a single resource as a coroutine
     *
     * @param connection the connection to use
     * @param request relative API path (e.g. "users/octocat")
     * @return Task yielding std::expected<Response, HttpError>
     *
     * @code
     * auto result = co_await coFetch(connection, "users/1");
     * if (result)
     *     std::cout << result->body;
     * @endcode
     */
    inline Task<std::expected<Response, HttpError>>
    coFetch(IConnection& connection, const std::string& request)
    {
        co_return co_await detail::FetchAwaitable{connection, request, {}, {}};
    }

    /**
     * @brief Asynchronously fetch with pagination as a coroutine
     *
     * @param connection the connection to use
     * @param request relative API path (e.g. "repos/owner/repo/issues")
     * @param strategy pagination strategy
     * @return Task yielding std::expected<std::string, HttpError>
     *
     * @code
     * LinkHeaderPaginationStrategy strategy;
     * auto result = co_await coFetch(connection, "repos/owner/repo/issues", strategy);
     * if (result)
     *     std::cout << *result;
     * @endcode
     */
    inline Task<std::expected<std::string, HttpError>>
    coFetch(IConnection& connection, const std::string& request, IPaginationStrategy& strategy)
    {
        co_return co_await detail::PaginatedFetchAwaitable{connection, request, strategy, {}, {}};
    }

    /**
     * @brief Fire-and-forget coroutine wrapper
     *
     * Starts eagerly, self-destroys on completion.
     * Useful for launching coroutines from non-coroutine contexts.
     *
     * @code
     * Detached myFunc(IConnection& conn, std::promise<void>& done)
     * {
     *     auto result = co_await coFetch(conn, "users/1");
     *     // ... process result ...
     *     done.set_value();
     * }
     * @endcode
     */
    struct Detached
    {
        struct promise_type
        {
            Detached get_return_object() { return {}; }
            std::suspend_never initial_suspend() noexcept { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };
    };

} // namespace cpp_restapi

#endif
