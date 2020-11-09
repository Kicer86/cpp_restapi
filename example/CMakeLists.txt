
project(GitHubApiExample)
cmake_minimum_required(VERSION 3.10)

add_subdirectory(.. github_api_root)                #include directory with github_api

find_package(Qt5 REQUIRED COMPONENTS Network Core)

add_executable(example main.cpp)

target_link_libraries(example
    PRIVATE
        github_api
        Qt5::Network
)
