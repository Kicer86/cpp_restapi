
project(GitHubApiExample)
cmake_minimum_required(VERSION 3.10)

set(GitHubAPI_QtBackend ON)
add_subdirectory(.. github_api_root)                #include directory with github_api

find_package(Qt5 REQUIRED COMPONENTS Network Core)

add_executable(qt_example qt_example.cpp)

target_link_libraries(qt_example
    PRIVATE
        github_api
        Qt5::Network
)
