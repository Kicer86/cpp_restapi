
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/qt_connection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    // Access The Star Wars API
    cpp_restapi::QtBackend::Connection connection(manager, "https://swapi.dev/api", {});

    std::cout << connection.get("people/1") << '\n';
    std::cout << connection.get("starships/12/") << '\n';

    return 0;
}
