#ifndef IPAGINATION_STRATEGY_HPP_INCLUDED
#define IPAGINATION_STRATEGY_HPP_INCLUDED

#include <string>
#include <vector>

namespace cpp_restapi
{

    /**
     * @brief Interface for pagination strategies
     *
     * Defines how to discover the next page URL from response headers
     * and how to merge multiple pages into a single result.
     */
    struct IPaginationStrategy
    {
        virtual ~IPaginationStrategy() = default;

        /**
         * @brief Extract next page URL from response headers
         * @param responseHeaders raw HTTP headers from the response
         * @return URL of the next page, or empty string if no more pages
         */
        virtual std::string nextPageUrl(const std::string& responseHeaders) = 0;

        /**
         * @brief Merge all collected pages into a single result
         * @param pages response bodies from all pages
         * @return merged result
         */
        virtual std::string merge(const std::vector<std::string>& pages) = 0;
    };

}

#endif
