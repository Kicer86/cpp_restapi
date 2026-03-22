
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QTimer>

#include <cpp_restapi/coroutine.hpp>
#include <cpp_restapi/create_qt_connection.hpp>


cpp_restapi::Detached fetchAndPrint(cpp_restapi::IConnection& connection, QCoreApplication& app)
{
    auto result = co_await cpp_restapi::coFetch(connection, "people/1");

    if (result)
    {
        std::cout << "Status: " << result->statusCode << '\n';
        std::cout << "Body:   " << result->body << '\n';
    }
    else
    {
        std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

    app.quit();
}


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    auto connection = cpp_restapi::createQtConnection(manager, "https://swapi.dev/api", {});

    fetchAndPrint(*connection, qapp);

    return qapp.exec();
}
