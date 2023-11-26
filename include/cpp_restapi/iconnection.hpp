
#ifndef ACONNECTION_HPP
#define ACONNECTION_HPP

#include <string>

namespace cpp_restapi
{
    /**
     * @brief Interface representing connection with rest api server
     */
    struct IConnection
    {
        public:
            virtual ~IConnection() = default;

            /**
             * @brief perform a request to api
             * @param request api request. For example "users/SomeUserName/repos"
             * @return api response in json format
             */
            virtual std::string get(const std::string& request) = 0;
    };
}

#endif
