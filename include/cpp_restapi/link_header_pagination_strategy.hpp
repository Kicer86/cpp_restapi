#ifndef LINK_HEADER_PAGINATION_STRATEGY_HPP_INCLUDED
#define LINK_HEADER_PAGINATION_STRATEGY_HPP_INCLUDED

#include <cpp_restapi/ipagination_strategy.hpp>
#include "cpp_restapi_json_pagination_export.h"

namespace cpp_restapi
{

    /**
     * @brief Pagination strategy using RFC 5988 Link headers
     *
     * Discovers the next page via the `Link: <url>; rel="next"` header
     * and merges JSON responses by concatenating arrays or deep-merging objects.
     * Used by GitHub API and other services that paginate with Link headers.
     *
     * Provided by the optional `cpp_restapi_json_pagination` CMake target
     * (requires jsoncpp).
     */
    class CPP_RESTAPI_JSON_PAGINATION_EXPORT LinkHeaderPaginationStrategy: public IPaginationStrategy
    {
    public:
        std::string nextPageUrl(const std::string& responseHeaders) override;
        std::string merge(const std::vector<std::string>& pages) override;
    };

}

#endif
