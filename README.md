
# Rest API for c++

This is a c++ library originally written for accessing GitHub REST API v3.
Currently reorganized to be easily used with any Rest API available.

It supports two backends for establishing connections with remote API servers:
Qt5/6 and Curl.

##### Warning:
The library is being renamed from GitHub_API to cpp_RestAPI.
At this moment, to provide backward compatibility, old interfaces are still available but are about to be removed.
Do not use classes marked as deprecated in new projects.

# Submodules

This repository has submodules which are not necessary to build and use this project.<br>
You may need them if you want to build unit tests (`CppRestAPI_Tests` cmake varible set to `TRUE`).

Another submodule is `vcpkg` which can simplify build by providing required dependencies.<br>
Please mind that vcpkg uses **telemetry**.<br>
Visit https://learn.microsoft.com/vcpkg/about/privacy for more details:

## How to use it

This is a CMake based project and is meant to be included as a subproject.

Simply embed cpp_restapi's sources in your project,
choose which http backend you prefer (both can be used simoultanously) and include `cpp_restapi` project in your `CMakeLists.txt` like this:

```cmake
set(CppRestAPI_QtBackend ON)      # use this line if you prefer Qt backend
set(CppRestAPI_CurlBackend ON)    # use this line if you prefer Curl backend
add_subdirectory(cpp_restapi)
```

Then you can link your application against cpp_restapi:

```cmake
target_link_libraries(app
    PRIVATE
        cpp_restapi
)
```

and that's all.

##### Note:
Depending on your choice of backend you may need to install libcurl and/or Qt libraries.

Qt backend can be compiled with Qt5 (default) or Qt6.
Set `CppRestAPI_UseQt6` CMake variable to `TRUE` to use Qt6.


## Examples

## Simplest usage

```c++
#include <iostream>

#include <cpp_restapi/curl_connection.hpp>


int main(int argc, char** argv)
{
    // Access The Star Wars API
    cpp_restapi::CurlBackend::Connection connection("https://swapi.dev/api", {});

    std::cout << connection.get("people/1") << '\n';
    std::cout << connection.get("starships/12/") << '\n';

    return 0;
}
```

This example accesses The Star Wars API using curl backend.<br>
As you can see it is enought to instantiate `cpp_restapi::CurlBackend::Connection` object providing API url and after that request can be made.

Qt version:
```c++
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/curl_connection.hpp>


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
```

### Dedicated GitHub helpers

For accessing GitHub API it is possible to use exactly the same apporach as presented above.<br>
However, for conveniance, there are also additional helpers available:

#### Qt example

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

Here connection is being build with `ConnectionBuilder`.<br>
Builder provides methods for setting additional connection parameters (passed as a second argument to `Connection` after API url).<br>
It also sets the API url automatically.<br>
Refer documentation of `ConnectionBuilder` for more details.

Additionaly there is also `cpp_restapi::GitHub::Request` class available which comes with accessors to most common API requests.

#### libcurl example

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

Code documentation available at https://kicer86.github.io/cpp_restapi/index.html
