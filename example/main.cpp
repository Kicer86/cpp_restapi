
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>

#include <github_api/github_api.hpp>
#include <github_api/request.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    GitHub::QtBackend::Api github(manager);
    std::unique_ptr<GitHub::IConnection> connection = github.connect();
    GitHub::Request request(connection.get());

    const std::string json = request.getUserInfo("Kicer86");

    qInfo() << json.c_str();

    return 0;
}
