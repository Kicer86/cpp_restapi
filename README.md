# GitHub API for c++

This is a c++ library for accessing GitHub REST API v3.
Depends on Qt5 Network and Core modules.

Currently offered functionality is limited but very easy to extend.

## Example

```c++
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>

#include <github_api/github_api.hpp>
#include <github_api/request.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    GitHubApi github(manager);
    std::unique_ptr<GitHub::IConnection> connection = github.connect();
    GitHub::Request request(connection.get());

    const QJsonDocument info = request.getUserInfo("Kicer86");
    const QByteArray raw = info.toJson();

    qInfo() << raw;

    return 0;
}
```

Also please look into 'example' directory for details.

## How to build

This is a CMake based project and is meant to be included as subproject.
Simply embed github_api's sources in your project and include it with

```cmake
add_subdirectory(github_api)
```

Then you can link your application against github_api:

```cmake
target_link_libraries(app
    PRIVATE
        github_api
)
```

and that's all.
