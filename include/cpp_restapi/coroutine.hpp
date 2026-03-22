
#ifndef COROUTINE_HPP_INCLUDED
#define COROUTINE_HPP_INCLUDED

#include <coroutine>
#include <exception>
#include <expected>
#include <optional>
#include <string>

#include "iconnection.hpp"

namespace cpp_restapi
{

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
     * @brief Asynchronously fetch a single resource
     *
     * Returns an awaitable — use with `co_await`:
     * @code
     * auto result = co_await coFetch(connection, "users/1");
     * if (result)
     *     std::cout << result->body;
     * @endcode
     */
    inline detail::FetchAwaitable
    coFetch(IConnection& connection, const std::string& request)
    {
        return {connection, request, {}, {}};
    }

    /**
     * @brief Asynchronously fetch with pagination
     *
     * Returns an awaitable — use with `co_await`:
     * @code
     * LinkHeaderPaginationStrategy strategy;
     * auto result = co_await coFetch(connection, "repos/owner/repo/issues", strategy);
     * @endcode
     */
    inline detail::PaginatedFetchAwaitable
    coFetch(IConnection& connection, const std::string& request, IPaginationStrategy& strategy)
    {
        return {connection, request, strategy, {}, {}};
    }

    /**
     * @brief Fire-and-forget coroutine wrapper
     *
     * Starts eagerly, self-destroys on completion.
     * Useful for launching coroutines from non-coroutine contexts (e.g. main()).
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
