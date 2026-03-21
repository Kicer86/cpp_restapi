#include <iostream>
#include <future>

#include <cpp_restapi/cpp-httplib_connection.hpp>


int main()
{
    cpp_restapi::CppHttplibBackend::Connection connection("https://swapi.dev/api", {});

    // Asynchronous fetch — returns immediately, callback runs on a background thread
    std::promise<void> done;
    auto future = done.get_future();

    auto cancel = connection.fetch("people/1",
        [&done](cpp_restapi::Response resp)
        {
            std::cout << "Status: " << resp.statusCode << '\n';
            std::cout << "Body:   " << resp.body << '\n';
            done.set_value();
        },
        [&done](cpp_restapi::HttpError err)
        {
            std::cerr << "Error " << err.statusCode << ": " << err.message << '\n';
            done.set_value();
        });

    std::cout << "Request in flight — doing other work...\n";
    future.wait();

    return 0;
}
