#ifndef LINK_HEADER_PAGINATION_STRATEGY_HPP_INCLUDED
#define LINK_HEADER_PAGINATION_STRATEGY_HPP_INCLUDED

#include <cpp_restapi/ipagination_strategy.hpp>
#include "cpp_restapi_export.h"

namespace cpp_restapi::GitHub
{

    /**
     * @brief Pagination strategy using RFC 5988 Link headers
     *
     * Discovers the next page via the Link: <url>; rel="next" header
     * and merges JSON responses by concatenating arrays or deep-merging objects.
     */
    class CPP_RESTAPI_EXPORT LinkHeaderPaginationStrategy: public IPaginationStrategy
    {
    public:
        std::string nextPageUrl(const std::string& responseHeaders) override;
        std::string merge(const std::vector<std::string>& pages) override;
    };

}

#endif
