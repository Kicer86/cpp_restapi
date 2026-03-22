#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/create_qt_connection.hpp>
#include <cpp_restapi/iconnection.hpp>
#include <cpp_restapi/isse_connection.hpp>
#include <cpp_restapi/sse_event.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    auto connection = cpp_restapi::createQtConnection(manager, "https://sse.dev", {});

    auto sse = connection->subscribe("test", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
        std::cout << "---\n";
    });

    return qapp.exec();
}
