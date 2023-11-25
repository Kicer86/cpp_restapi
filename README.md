# GitHub API for c++

This is a c++ library originally written for accessing GitHub REST API v3.
Currently reorganized to be easily used with any API available.

It supports two backends for establishing connections with remote API servers:
Qt5/6 and Curl.

## How to use it

This is a CMake based project and is meant to be included as a subproject.

Simply embed github_api's sources in your project,
choose which http backend you prefer (both can be used simoultanously) and include github_api project in your CMakeLists.txt like this:

```cmake
set(GitHubAPI_QtBackend ON)      # use this line if you prefer Qt backend
set(GitHubAPI_CurlBackend ON)    # use this line if you prefer Curl backend
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

Qt backend can be compiled with Qt5 (default) or Qt6.
Set GitHubAPI_UseQt6 CMake variable to TRUE to use Qt6.


## Qt example

```c++
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>

#include <cpp_restapi/qt_connection.hpp>
#include <cpp_restapi/github/connection_builder.hpp>
#include <cpp_restapi/github/request.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    auto connection = cpp_restapi::GitHub::ConnectionBuilder().build<cpp_restapi::QtBackend::Connection>(manager);
    cpp_restapi::GitHub::Request request(connection);

    qInfo() << request.getRateLimit().c_str();
    qInfo() << request.getUserInfo("Kicer86").c_str();

    return 0;
}
```

## libcurl example

```c++
#include <iostream>

#include <cpp_restapi/curl_connection.hpp>
#include <cpp_restapi/github/connection_builder.hpp>
#include <cpp_restapi/github/request.hpp>


int main(int argc, char** argv)
{
    auto connection = cpp_restapi::GitHub::ConnectionBuilder().build<cpp_restapi::CurlBackend::Connection>();
    cpp_restapi::GitHub::Request request(connection);

    std::cout << request.getRateLimit() << '\n';
    std::cout << request.getUserInfo("Kicer86") << '\n';

    return 0;
}
```

Also please look into 'examples' directory for details.

## Links

Code documentation available at https://kicer86.github.io/github_api/index.html
