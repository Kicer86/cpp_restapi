
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <expected>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "http_error.hpp"
#include "ipagination_strategy.hpp"
#include "isse_connection.hpp"
#include "sse_event.hpp"

namespace cpp_restapi
{

    /**
     * @brief HTTP response containing body, raw headers and the HTTP status code
     */
    struct Response
    {
        std::string body;
        std::string headers;
        int         statusCode = 0;  ///< HTTP status code (e.g. 200, 404). 0 means no response was received (network error).
    };

    /**
     * @brief Interface representing connection with rest api server
     */
    struct IConnection
    {
        public:
            using EventCallback   = std::function<void(const SseEvent&)>;
            using FetchCallback   = std::function<void(Response)>;
            using ErrorCallback   = std::function<void(HttpError)>;

            virtual ~IConnection() = default;

            /**
             * @brief perform a request to api
             * @param request api request. For example "users/SomeUserName/repos"
             * @return api response in json format
             * @deprecated Use fetch() for single requests or fetch() with IPaginationStrategy for paginated requests.
             *             This method has hardcoded GitHub-specific pagination logic.
             */
            [[deprecated("Use fetch() or fetch() with IPaginationStrategy instead")]]
            virtual std::string get(const std::string& request) = 0;

            /**
             * @brief Perform a single HTTP request
             * @param request relative API path (e.g. "api/v1/disks")
             * @return response body on success, or an HttpError describing the failure
             *
             * Returns std::unexpected(HttpError) when:
             * - The server returns a 4xx or 5xx status code
             * - A network-level failure occurs (statusCode == 0 in the error)
             */
            virtual std::expected<std::string, HttpError> fetch(const std::string& request) = 0;

            /**
             * @brief Perform requests with automatic pagination
             * @param request relative API path
             * @param strategy pagination strategy defining how to discover next page and merge results
             * @return merged response body from all pages on success, or an HttpError on first failure
             */
            virtual std::expected<std::string, HttpError> fetch(const std::string& request, IPaginationStrategy& strategy) = 0;

            /**
             * @brief Perform a single HTTP request returning the full response
             * @param url full URL to fetch
             * @return full Response (body, headers, statusCode) on success, or an HttpError on failure
             */
            virtual std::expected<Response, HttpError> fetchResponse(const std::string& url) = 0;

            /**
             * @brief return API url
             */
            virtual const std::string& url() const = 0;

            /**
             * @brief Subscribe to an SSE endpoint
             * @param request API endpoint path (e.g. "events" or "api/v1/events")
             * @param callback function called for each received SSE event
             * @return SSE connection handle; use its close() method to stop
             */
            virtual std::unique_ptr<ISseConnection> subscribe(const std::string& request, EventCallback callback) = 0;

            /**
             * @brief Perform an HTTP GET request asynchronously
             *
             * Non-blocking. @p onSuccess is called with the full response when the
             * request completes successfully; @p onError is called with an HttpError
             * if the request fails. The callbacks may be invoked from a
             * background thread (non-Qt backends) or from the Qt event-loop thread
             * (Qt backend) — callers must handle thread-safety accordingly.
             *
             * @param request relative API path (e.g. "users/octocat")
             * @param onSuccess called with Response{body, headers, statusCode} on success
             * @param onError   called with an HttpError on failure (optional)
             */
            virtual void fetch(const std::string& request,
                               FetchCallback onSuccess,
                               ErrorCallback onError = {}) = 0;
    };
}

#endif
