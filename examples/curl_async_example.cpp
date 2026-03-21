#include <iostream>
#include <future>

#include <cpp_restapi/curl_connection.hpp>


int main()
{
    cpp_restapi::CurlBackend::Connection connection("https://swapi.dev/api", {});

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

    // Do other work here while the request runs in the background...
    std::cout << "Request in flight — doing other work...\n";

    // Wait for the async request to complete
    future.wait();

    return 0;
}
