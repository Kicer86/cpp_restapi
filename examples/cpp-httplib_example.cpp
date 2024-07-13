
#include <iostream>

#include <cpp_restapi/cpp-httplib_connection.hpp>
#include <cpp_restapi/github/connection_builder.hpp>
#include <cpp_restapi/github/request.hpp>


int main(int argc, char** argv)
{
    auto connection = cpp_restapi::GitHub::ConnectionBuilder().build<cpp_restapi::CppHttplibBackend::Connection>();
    cpp_restapi::GitHub::Request request(connection);

    std::cout << request.getRateLimit() << '\n';
    std::cout << request.getUserInfo("Kicer86") << '\n';

    return 0;
}
