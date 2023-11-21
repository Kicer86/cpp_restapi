
cmake_minimum_required(VERSION 3.16)
project(GitHubAPI)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

find_package(jsoncpp CONFIG)

if(jsoncpp_FOUND)
    set(JSONCPP_LIBRARIES JsonCpp::JsonCpp)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(JSONCPP REQUIRED jsoncpp)
endif()

include(GenerateExportHeader)

option(GitHubAPI_QtBackend "Qt backend for GitHubApi" OFF)
option(GitHubAPI_CurlBackend "libcurl backend for GitHubApi" OFF)
option(GitHubAPI_Tests "Enable GitHubApi tests. Forces both GitHubAPI_QtBackend and GitHubAPI_CurlBackend to be ON" OFF)

if(GitHubAPI_Tests)
    set(GitHubAPI_QtBackend ON)
    set(GitHubAPI_CurlBackend ON)
endif()

add_library(github_api
    include/github_api/iconnection.hpp
    include/github_api/igithub_api.hpp
    include/github_api/request.hpp
    src/base_connection.cpp
    src/base_connection.hpp
    src/header_utils.cpp
    src/header_utils.hpp
    src/request.cpp
)

target_include_directories(github_api
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE SYSTEM
        ${JSONCPP_INCLUDE_DIRS}
)

target_link_libraries(github_api
    PRIVATE
        ${JSONCPP_LIBRARIES}
)

generate_export_header(github_api)

if(NOT GitHubAPI_QtBackend AND NOT GitHubAPI_CurlBackend)
    message(FATAL_ERROR "No backend was chosen. Set either GitHubAPI_QtBackend or GitHubAPI_CurlBackend variable to ON")
endif()

if(GitHubAPI_QtBackend)
    add_subdirectory(src/qt_backend)
endif()

if(GitHubAPI_CurlBackend)
    add_subdirectory(src/curl_backend)
endif()

if(GitHubAPI_Tests)
    enable_testing()
    add_subdirectory(tests)
endif()
