
#include <iostream>
#include <future>

#include <cpp_restapi/coroutine.hpp>
#include <cpp_restapi/create_cpp-httplib_connection.hpp>


cpp_restapi::Detached fetchAndPrint(cpp_restapi::IConnection& connection, std::promise<void>& done)
{
    auto result = co_await cpp_restapi::coFetch(connection, "people/1");

    if (result)
    {
        std::cout << "Status: " << result->statusCode << '\n';
        std::cout << "Body:   " << result->body << '\n';
    }
    else
    {
        std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

    done.set_value();
}


int main()
{
    auto connection = cpp_restapi::createCppHttplibConnection("https://swapi.dev/api", {});

    std::promise<void> done;
    auto future = done.get_future();

    fetchAndPrint(*connection, done);

    future.wait();

    return 0;
}
