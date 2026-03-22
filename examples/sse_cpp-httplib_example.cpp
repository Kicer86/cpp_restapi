#include <iostream>
#include <thread>
#include <chrono>

#include <cpp_restapi/create_cpp-httplib_connection.hpp>
#include <cpp_restapi/iconnection.hpp>
#include <cpp_restapi/isse_connection.hpp>
#include <cpp_restapi/sse_event.hpp>


int main(int argc, char** argv)
{
    auto connection = cpp_restapi::createCppHttplibConnection("https://sse.dev", {});

    auto sse = connection->subscribe("test", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
        std::cout << "---\n";
    });

    // Do other work while events are received in the background
    std::this_thread::sleep_for(std::chrono::seconds(30));

    sse->close();

    return 0;
}
