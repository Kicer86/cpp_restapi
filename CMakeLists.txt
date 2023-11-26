
cmake_minimum_required(VERSION 3.16)
project(cppRestAPI)

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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GenerateExportHeader)

option(CppRestAPI_QtBackend "Qt backend for GitHubApi" OFF)
option(CppRestAPI_CurlBackend "libcurl backend for GitHubApi" OFF)
option(CppRestAPI_Tests "Enable GitHubApi tests. Forces both CppRestAPI_QtBackend and CppRestAPI_CurlBackend to be ON" OFF)

if(CppRestAPI_Tests)
    set(CppRestAPI_QtBackend ON)
    set(CppRestAPI_CurlBackend ON)
endif()

add_library(cpp_restapi
    src/base_connection.cpp
    src/header_utils.cpp
    src/services/github/request.cpp
)

target_include_directories(cpp_restapi
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE SYSTEM
        ${JSONCPP_INCLUDE_DIRS}
)

target_link_libraries(cpp_restapi
    PRIVATE
        ${JSONCPP_LIBRARIES}
)

generate_export_header(cpp_restapi)

add_library(github_api ALIAS cpp_restapi)

if(NOT CppRestAPI_QtBackend AND NOT CppRestAPI_CurlBackend)
    message(FATAL_ERROR "No backend was chosen. Set either CppRestAPI_QtBackend or CppRestAPI_CurlBackend variable to ON")
endif()

if(CppRestAPI_QtBackend)
    add_subdirectory(src/qt_backend)
endif()

if(CppRestAPI_CurlBackend)
    add_subdirectory(src/curl_backend)
endif()

if(CppRestAPI_Tests)
    enable_testing()
    add_subdirectory(tests)
endif()
