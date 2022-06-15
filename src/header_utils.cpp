
#include <regex>
#include <sstream>

#include "header_utils.hpp"


namespace HeaderUtils
{

    std::string getNextPageUrl(const std::string& header)
    {
        std::istringstream header_(header);
        std::string line;
        std::string link;

        const std::regex nextPageRegex(R"(^link\: <([^>]+)>; rel="next".*)");

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
