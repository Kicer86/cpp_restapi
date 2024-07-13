
#include <iostream>

#include <cpp_restapi/cpp-httplib_connection.hpp>


int main(int argc, char** argv)
{
    // Access The Star Wars API
    cpp_restapi::CppHttplibBackend::Connection connection("https://swapi.dev/api", {});

    std::cout << connection.get("people/1") << '\n';
    std::cout << connection.get("starships/12/") << '\n';

    return 0;
}
