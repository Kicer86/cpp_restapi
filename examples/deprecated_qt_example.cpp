
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>

#include <github_api/github_api_qt.hpp>
#include <github_api/request.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    GitHub::QtBackend::Api github(manager);
    GitHub::Request request(github.connect());

    qInfo() << request.getRateLimit().c_str();
    qInfo() << request.getUserInfo("Kicer86").c_str();

    return 0;
}
