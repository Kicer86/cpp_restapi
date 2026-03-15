#include <iostream>

#include <cpp_restapi/curl_sse_connection.hpp>


int main(int argc, char** argv)
{
    // Connect to an SSE endpoint (blocks until connection closes)
    cpp_restapi::CurlBackend::SseConnection connection("http://localhost:8080", {});

    connection.subscribe("events", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
        std::cout << "---\n";
    });

    return 0;
}
