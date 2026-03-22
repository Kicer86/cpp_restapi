
#include <iostream>

#include <cpp_restapi/create_curl_connection.hpp>
#include <cpp_restapi/iconnection.hpp>


int main(int argc, char** argv)
{
    // Access The Star Wars API
    auto connection = cpp_restapi::createCurlConnection("https://swapi.dev/api", {});

    // fetch() returns std::expected<std::string, HttpError>
    for (const auto& endpoint: {"people/1", "starships/12/"})
    {
        const auto result = connection->fetch(endpoint);
        if (result)
            std::cout << result.value() << '\n';
        else
            std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

    return 0;
}
