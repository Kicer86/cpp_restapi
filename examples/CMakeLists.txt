
cmake_minimum_required(VERSION 3.10)
project(GitHubApiExample)

find_package(Qt6 COMPONENTS Network Core)
if (NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Network Core)
endif()

set(GitHubAPI_UseQt6 ${Qt6_FOUND})
set(GitHubAPI_QtBackend ON)
set(GitHubAPI_CurlBackend ON)
add_subdirectory(.. github_api_root)                #include directory with github_api

add_executable(qt_example qt_example.cpp)
add_executable(curl_example curl_example.cpp)

target_link_libraries(qt_example
    PRIVATE
        github_api
        Qt::Network
)

target_link_libraries(curl_example
    PRIVATE
        github_api
)
