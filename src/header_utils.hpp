
/******************************************************************************
 *
 * @author Kicer86@gmail.com
 *
 * @brief Http header parsing utils by Kevin Mukuna(mukuna.kevin@gmail.com)
 *        used to determine if results are returned in pages.
 *
 ******************************************************************************/

#ifndef HEADER_UTILS_HPP_INCLUDED
#define HEADER_UTILS_HPP_INCLUDED

#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>
#include <string>


namespace HeaderUtils
{
    /**
    * @brief the method takes in a http header and returns
    *        the next link used for pagination
    *
    * @param header header data from the return data from
    *               the api call
    * @return next page url
    */
    inline std::string getNextPageUrl(const std::string& header)
    {
        std::istringstream header_(header);
        std::string line;
        std::string link;

        const std::regex nextPageRegex(R"(^[Ll]ink\:.*<([^>]+)>; rel="next".*)");

        // iterating through each line of string
        while (std::getline(header_, line))
        {
            line.erase(
                std::remove_if(line.begin(), line.end(), iscntrl),
                line.end()
            );

            std::smatch nextPageMatch;
            const bool match = std::regex_match(line, nextPageMatch, nextPageRegex);

            if (match)
            {
                link = nextPageMatch[1];
                break;
            }
        }

        return link;
    }
}

#endif
