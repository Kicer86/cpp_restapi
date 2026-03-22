
#include <iostream>

#include <cpp_restapi/create_cpp-httplib_connection.hpp>
#include <cpp_restapi/github/connection_builder.hpp>
#include <cpp_restapi/github/request.hpp>


int main(int argc, char** argv)
{
    auto connection = cpp_restapi::GitHub::ConnectionBuilder().build(cpp_restapi::createCppHttplibConnection);
    cpp_restapi::GitHub::Request request(std::move(connection));

    std::cout << request.getRateLimit() << '\n';
    std::cout << request.getUserInfo("Kicer86") << '\n';

    return 0;
}
