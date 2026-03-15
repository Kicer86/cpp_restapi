#include <iostream>
#include <thread>
#include <chrono>

#include <cpp_restapi/cpp-httplib_sse_connection.hpp>


int main(int argc, char** argv)
{
    // Connect to an SSE endpoint (non-blocking)
    cpp_restapi::CppHttplibBackend::SseConnection connection("http://localhost:8080", {});

    connection.subscribe("events", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
        std::cout << "---\n";
    });

    // Do other work while events are received in the background
    std::this_thread::sleep_for(std::chrono::seconds(30));

    connection.close();

    return 0;
}
