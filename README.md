# GitHub API for c++

This is a c++ library for accessing GitHub REST API v3.

For connection with GitHub Qt5 or libcurl are needed.
It is up to the user which to use.

Currently offered functionality is limited but very easy to extend.

## How to use it

This is a CMake based project and is meant to be included as a subproject.

Simply embed github_api's sources in your project,
choose which http backend you prefer and include github_api project in your CMakeLists.txt like this:

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


## Qt example

```c++
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
```

## libcurl example

```c++
#include <iostream>

#include <github_api/github_api_curl.hpp>
#include <github_api/request.hpp>


int main(int argc, char** argv)
{
    GitHub::CurlBackend::Api github;
    GitHub::Request request(github.connect());

    std::cout << request.getRateLimit() << '\n';
    std::cout << request.getUserInfo("Kicer86") << '\n';

    return 0;
}
```

Also please look into 'examples' directory for details.
