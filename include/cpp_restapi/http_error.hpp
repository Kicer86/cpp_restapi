#ifndef HTTP_ERROR_HPP_INCLUDED
#define HTTP_ERROR_HPP_INCLUDED

#include <string>

namespace cpp_restapi
{
    /**
     * @brief Represents an HTTP or network-level error
     *
     * Returned by IConnection::fetch() and IConnection::fetchResponse() when
     * a request cannot be completed successfully.
     *
     * @code
     * auto result = connection.fetch("users/foo");
     * if (!result)
     * {
     *     if (result.error().statusCode == 0)
     *         // network-level failure (timeout, DNS, etc.)
     *     else if (result.error().statusCode == 404)
     *         // resource not found
     *     else
     *         // other HTTP error
     * }
     * @endcode
     */
    struct HttpError
    {
        int         statusCode;  ///< HTTP status code (e.g. 404, 500). 0 means a network-level failure — no HTTP response was received.
        std::string body;        ///< Raw response body (often contains error details for 4xx/5xx responses)
        std::string message;     ///< Human-readable error description
    };
}

#endif
