
# Rest API for c++

This is a c++ library originally written for accessing GitHub REST API v3.
Currently reorganized to be easily used with any Rest API available.

It supports three backends for establishing connections with remote API servers:
Qt6/5, Curl and cpp-httplib.

# Submodules
This repository comes with submodules which are not necessary to build and use this project.<br>

As of now the only submodule is `vcpkg` which can simplify build by providing required dependencies.<br>
Please mind that vcpkg uses **telemetry**.<br>
Visit https://learn.microsoft.com/vcpkg/about/privacy for more details.

## How to use it

This is a CMake based project and is meant to be included as a subproject.

Simply embed cpp_restapi's sources in your project,
choose which http backend you prefer (all can be used simoultanously) and include `cpp_restapi` project in your `CMakeLists.txt` like this:

```cmake
set(CppRestAPI_QtBackend ON)         # use this line if you prefer Qt backend
set(CppRestAPI_CurlBackend ON)       # use this line if you prefer Curl backend
set(CppRestAPI_CppHttplibBackend ON) # use this line if you prefer cpp-httplib backend
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
Depending on your choice of backend you may need to install libcurl, Qt and/or cpp-httplib libraries.

Qt backend can be compiled with Qt6 (default) or Qt5.
If no Qt6 is found, an automatic fallback to Qt5 will happen.

Set `CppRestAPI_UseQt5` CMake variable to `TRUE` to force Qt5 usage (in case both versions are available).

##### Standalone build:
It is possible to build this project as any other regular CMake project by invoking:
```bash
cmake -B build
cmake --build build
```

It can be usefull if you want to play with examples from `examples` dir or to run unit tests.

## Examples

## Simplest usage

```c++
#include <iostream>

#include <cpp_restapi/curl_connection.hpp>


int main(int argc, char** argv)
{
    // Access The Star Wars API
    cpp_restapi::CurlBackend::Connection connection("https://swapi.dev/api", {});

    // fetch() returns std::expected<std::string, HttpError>
    for (const auto& endpoint: {"people/1", "starships/12/"})
    {
        const auto result = connection.fetch(endpoint);
        if (result)
            std::cout << result.value() << '\n';
        else
            std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

    return 0;
}
```

This example accesses The Star Wars API using curl backend.<br>
`fetch()` returns `std::expected<std::string, HttpError>` — on success the response body is available via `value()`, on failure the `HttpError` carries the HTTP status code, response body and a human-readable message.

Qt version:
```c++
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/qt_connection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    // Access The Star Wars API
    cpp_restapi::QtBackend::Connection connection(manager, "https://swapi.dev/api", {});

    // fetch() returns std::expected<std::string, HttpError>
    for (const auto& endpoint: {"people/1", "starships/12/"})
    {
        const auto result = connection.fetch(endpoint);
        if (result)
            std::cout << result.value() << '\n';
        else
            std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

    return 0;
}
```

cpp-httplib version:
```c++
#include <iostream>

#include <cpp_restapi/cpp-httplib_connection.hpp>


int main(int argc, char** argv)
{
    // Access The Star Wars API
    cpp_restapi::CppHttplibBackend::Connection connection("https://swapi.dev/api", {});

    // fetch() returns std::expected<std::string, HttpError>
    for (const auto& endpoint: {"people/1", "starships/12/"})
    {
        const auto result = connection.fetch(endpoint);
        if (result)
            std::cout << result.value() << '\n';
        else
            std::cerr << "Error " << result.error().statusCode << ": " << result.error().message << '\n';
    }

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

#### cpp-httplib example:
```c++
#include <iostream>

#include <cpp_restapi/cpp-httplib_connection.hpp>
#include <cpp_restapi/github/connection_builder.hpp>
#include <cpp_restapi/github/request.hpp>


int main(int argc, char** argv)
{
    auto connection = cpp_restapi::GitHub::ConnectionBuilder().build<cpp_restapi::CppHttplibBackend::Connection>();
    cpp_restapi::GitHub::Request request(connection);

    std::cout << request.getRateLimit() << '\n';
    std::cout << request.getUserInfo("Kicer86") << '\n';

    return 0;
}
```

Also please look into 'examples' directory for details.

## Server-Sent Events (SSE)

In addition to regular REST requests, the library supports
[Server-Sent Events](https://html.spec.whatwg.org/multipage/server-sent-events.html) —
a standard mechanism for receiving a stream of events from a server over HTTP.

SSE support is available for all three backends via `Connection::subscribe()`.
The method connects to an SSE endpoint,
delivers parsed events through a callback and returns an `ISseConnection` handle.
The call is non-blocking — events are received on an internal
thread (or via the Qt event loop for the Qt backend). Use `close()` to stop.

### SSE with curl

```c++
#include <iostream>
#include <thread>
#include <chrono>

#include <cpp_restapi/curl_connection.hpp>


int main(int argc, char** argv)
{
    cpp_restapi::CurlBackend::Connection connection("http://localhost:8080", {});

    auto sse = connection.subscribe("events", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
    });

    // Do other work while events arrive in the background
    std::this_thread::sleep_for(std::chrono::seconds(30));

    sse->close();
    return 0;
}
```

### SSE with Qt

```c++
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/qt_connection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    cpp_restapi::QtBackend::Connection connection(manager, "http://localhost:8080", {});

    auto sse = connection.subscribe("events", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
    });

    return qapp.exec();
}
```

### SSE with cpp-httplib

```c++
#include <iostream>
#include <thread>
#include <chrono>

#include <cpp_restapi/cpp-httplib_connection.hpp>


int main(int argc, char** argv)
{
    cpp_restapi::CppHttplibBackend::Connection connection("http://localhost:8080", {});

    auto sse = connection.subscribe("events", [](const cpp_restapi::SseEvent& event)
    {
        std::cout << "Event: " << event.event << '\n';
        std::cout << "Data: " << event.data << '\n';
    });

    // Do other work while events arrive in the background
    std::this_thread::sleep_for(std::chrono::seconds(30));

    sse->close();
    return 0;
}
```

### SseEvent fields

The `SseEvent` struct exposes all standard SSE fields:

| Field   | Type          | Description                                              |
|---------|---------------|----------------------------------------------------------|
| `event` | `std::string` | Event type (from `event:` field, empty if not specified)  |
| `data`  | `std::string` | Event payload (from `data:` field(s), joined with `\n`)  |
| `id`    | `std::string` | Last event ID (from `id:` field)                         |
| `retry` | `int`         | Reconnection time in ms (from `retry:` field, -1 if N/A) |

## Asynchronous Requests

All three backends support non-blocking HTTP requests via a callback-based API.
`fetch()` returns immediately and delivers the result through `onSuccess` / `onError`
callbacks. It also returns a `CancellationToken` that can be used to suppress callbacks
before they fire.

For non-Qt backends (curl, cpp-httplib) callbacks run on a background `std::thread`.
For the Qt backend, callbacks are invoked on the Qt event-loop thread.

### Async with curl

```c++
#include <iostream>
#include <future>

#include <cpp_restapi/curl_connection.hpp>


int main()
{
    cpp_restapi::CurlBackend::Connection connection("https://swapi.dev/api", {});

    std::promise<void> done;
    auto future = done.get_future();

    auto cancel = connection.fetch("people/1",
        [&done](cpp_restapi::Response resp)
        {
            std::cout << "Status: " << resp.statusCode << '\n';
            std::cout << "Body:   " << resp.body << '\n';
            done.set_value();
        },
        [&done](cpp_restapi::HttpError err)
        {
            std::cerr << "Error " << err.statusCode << ": " << err.message << '\n';
            done.set_value();
        });

    // Do other work while the request runs in the background...
    std::cout << "Request in flight — doing other work...\n";

    future.wait();
    return 0;
}
```

### Async with Qt

```c++
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>

#include <cpp_restapi/qt_connection.hpp>


int main(int argc, char** argv)
{
    QCoreApplication qapp(argc, argv);
    QNetworkAccessManager manager;

    cpp_restapi::QtBackend::Connection connection(manager, "https://swapi.dev/api", {});

    auto cancel = connection.fetch("people/1",
        [&qapp](cpp_restapi::Response resp)
        {
            std::cout << "Status: " << resp.statusCode << '\n';
            std::cout << "Body:   " << resp.body << '\n';
            qapp.quit();
        },
        [&qapp](cpp_restapi::HttpError err)
        {
            std::cerr << "Error " << err.statusCode << ": " << err.message << '\n';
            qapp.quit();
        });

    return qapp.exec();
}
```

### Cancellation

The `CancellationToken` returned by async `fetch()` is a `std::shared_ptr<std::atomic<bool>>`.
Setting it to `true` suppresses further callbacks:

```c++
auto cancel = connection.fetch("slow/endpoint",
    [](cpp_restapi::Response) { /* ... */ },
    [](cpp_restapi::HttpError) { /* ... */ });

// Changed our mind — suppress callbacks
cancel->store(true);
```

### Async pagination

Paginated requests can also be performed asynchronously.
The merged result is delivered through a `StringCallback` once all pages have been collected:

```c++
cpp_restapi::LinkHeaderPaginationStrategy strategy;

auto cancel = connection.fetch("repos/owner/repo/issues", strategy,
    [](std::string mergedBody)
    {
        std::cout << "All pages: " << mergedBody << '\n';
    },
    [](cpp_restapi::HttpError err)
    {
        std::cerr << "Error on page: " << err.statusCode << '\n';
    });
```

## Building examples
Examples are located in the 'examples' directory of the project.
To build them set `CppRestAPI_Examples` CMake variable to `ON`.
It can be done when invoking `cmake` command by providing `-DCppRestAPI_Examples=ON` command line argument (see `Standalone build` section).
Or by modifying entry `CppRestAPI_Examples` in CMakeCache.txt file located in build directory of an already configured project.

Please mind that setting `CppRestAPI_Examples` to `ON` will force all backends to be used.

## Building unit tests
Unit tests are located in 'tests' directory of the project.
To build them set `CppRestAPI_Tests` CMake variable to `ON`.

Please mind that setting `CppRestAPI_Tests` to `ON` will force all backends to be used.

## Links

Code documentation available at https://kicer86.github.io/cpp_restapi/index.html
