#include <iostream>
#include <future>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/create_qt_connection.hpp>
#include <cpp_restapi/iconnection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    auto connection = cpp_restapi::createQtConnection(manager, "https://swapi.dev/api", {});

    // Asynchronous fetch — callback runs on the Qt event-loop thread
    auto cancel = connection->fetch("people/1",
        [&qapp](cpp_restapi::Response resp)
        {
            std::cout << "Status: " << resp.statusCode << '\n';
            std::cout << "Body:   " << resp.body << '\n';
            qapp.quit();
        },
        [&qapp](cpp_restapi::HttpError err)
        {
            std::cerr << "Error " << err.statusCode << ": " << err.message << '\n';
            qapp.quit();
        });

    return qapp.exec();
}
