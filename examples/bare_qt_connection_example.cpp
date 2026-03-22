
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/create_qt_connection.hpp>
#include <cpp_restapi/iconnection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    // Access The Star Wars API
    auto connection = cpp_restapi::createQtConnection(manager, "https://swapi.dev/api", {});

    // fetch() returns std::expected<std::string, HttpError>
    for (const auto& endpoint: {"people/1", "starships/12/"})
    {
        const auto result = connection->fetch(endpoint);
        if (result)
            std::cout << result.value() << '\n';
        else
            std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

    return 0;
}
