
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/coroutine.hpp>
#include <cpp_restapi/create_qt_connection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    auto connection = cpp_restapi::createQtConnection(manager, "https://swapi.dev/api", {});

    [&]() -> cpp_restapi::Detached
    {
        auto result = co_await cpp_restapi::coFetch(*connection, "people/1");

        if (result)
            std::cout << "Status: " << result->statusCode << '\n'
                      << "Body:   " << result->body << '\n';
        else
            std::cerr << "Error: " << result.error().message << '\n';

        qapp.quit();
    }();

    return qapp.exec();
}
