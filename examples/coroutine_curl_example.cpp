
#include <iostream>
#include <future>

#include <cpp_restapi/coroutine.hpp>
#include <cpp_restapi/create_curl_connection.hpp>


int main()
{
    auto connection = cpp_restapi::createCurlConnection("https://swapi.dev/api", {});

    std::promise<void> done;
    auto future = done.get_future();

    [&]() -> cpp_restapi::Detached
    {
        auto result = co_await cpp_restapi::coFetch(*connection, "people/1");

        if (result)
            std::cout << "Status: " << result->statusCode << '\n'
                      << "Body:   " << result->body << '\n';
        else
            std::cerr << "Error: " << result.error().message << '\n';

        done.set_value();
    }();

    future.wait();
    return 0;
}
