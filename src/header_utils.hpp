
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

#include <sstream>


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
    std::string getNextPageUrl(const std::string& header);
}

#endif
