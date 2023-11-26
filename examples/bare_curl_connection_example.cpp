
#include <iostream>

#include <cpp_restapi/curl_connection.hpp>


int main(int argc, char** argv)
{
    // Access The Star Wars API
    cpp_restapi::CurlBackend::Connection connection("https://swapi.dev/api", {});

    std::cout << connection.get("people/1") << '\n';
    std::cout << connection.get("starships/12/") << '\n';

    return 0;
}
