
cmake_minimum_required(VERSION 3.10)
project(GitHubApiExample)

find_package(Qt6 COMPONENTS Network Core)
if (NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Network Core)
endif()

set(CppRestAPI_UseQt6 ${Qt6_FOUND})
set(CppRestAPI_QtBackend ON)
set(CppRestAPI_CurlBackend ON)
add_subdirectory(.. cpp_restapi_root)                #include directory with github_api

add_executable(qt_example qt_example.cpp)
add_executable(curl_example curl_example.cpp)
add_executable(bare_curl_connection_example bare_curl_connection_example.cpp)
add_executable(bare_qt_connection_example bare_qt_connection_example.cpp)

target_link_libraries(qt_example
    PRIVATE
        github_api
        Qt::Network
)

target_link_libraries(curl_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(bare_curl_connection_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(bare_qt_connection_example
    PRIVATE
        cpp_restapi
)
