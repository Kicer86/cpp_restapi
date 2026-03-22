
#include <cpp_restapi/create_cpp-httplib_connection.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <cpp_restapi/cpp-httplib_connection.hpp>
#pragma GCC diagnostic pop

namespace cpp_restapi
{
    std::unique_ptr<IConnection> createCppHttplibConnection(
        const std::string& address,
        const std::map<std::string, std::string>& headerEntries)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        return std::make_unique<CppHttplibBackend::Connection>(address, headerEntries);
#pragma GCC diagnostic pop
    }
}
