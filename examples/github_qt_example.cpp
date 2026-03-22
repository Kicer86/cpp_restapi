
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>

#include <cpp_restapi/create_qt_connection.hpp>
#include <cpp_restapi/github/connection_builder.hpp>
#include <cpp_restapi/github/request.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    auto connection = cpp_restapi::GitHub::ConnectionBuilder().build(cpp_restapi::createQtConnection, manager);
    cpp_restapi::GitHub::Request request(connection);

    qInfo() << request.getRateLimit().c_str();
    qInfo() << request.getUserInfo("Kicer86").c_str();

    return 0;
}
