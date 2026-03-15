#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/qt_sse_connection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    // Connect to an SSE endpoint
    cpp_restapi::QtBackend::SseConnection connection(manager, "http://localhost:8080", {});

    connection.subscribe("events", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
        std::cout << "---\n";
    });

    return qapp.exec();
}
