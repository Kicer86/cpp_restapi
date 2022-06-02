

#include "header_utils.hpp"


namespace HeaderUtils
{

    std::pair <std::string, int> checkPaginationLInk(const std::string& header)
    {
        std::istringstream header_(header);
        std::string line;
        std::string link;
        int numberOfPage;
        // iterating through each line of string
        while (std::getline(header_, line)) {
            std::istringstream eachLine(line);
            // itereating through each word in line, words are seperate by space
            bool found = false;
            while (eachLine && !found) {
                std::string word;
                eachLine >> word;
                if (word == "rel=\"next\",") {
                    eachLine >> word;
                    for (int i = 0; i < word.size(); i++) {
                        if (word[i] == '<') {
                            continue;
                        } else if (word[i] == '>') {
                            numberOfPage = word[i-1] - '0';
                            break;
                        } else {
                            link = std::string(link)+word[i];
                        }
                    }
                    found = true;
                }
            }
        }
        return std::make_pair(link, numberOfPage);
    }

}
