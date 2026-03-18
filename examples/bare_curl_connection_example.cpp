
#include <iostream>

#include <cpp_restapi/curl_connection.hpp>


int main(int argc, char** argv)
{
    // Access The Star Wars API
    cpp_restapi::CurlBackend::Connection connection("https://swapi.dev/api", {});

    // fetch() returns std::expected<std::string, HttpError>
    for (const auto& endpoint: {"people/1", "starships/12/"})
    {
        const auto result = connection.fetch(endpoint);
        if (result)
            std::cout << result.value() << '\n';
        else
            std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

    return 0;
}
