
#include <httplib.h>

#include <cpp_restapi/cpp-httplib_connection.hpp>


namespace cpp_restapi::CppHttplibBackend
{
    Connection::Connection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
        : BaseConnection(address, headerEntries)
    {

    }


    Connection::~Connection()
    {

    }


    std::pair<std::string, std::string> Connection::fetchPage(const std::string& page)
    {

    }
}
