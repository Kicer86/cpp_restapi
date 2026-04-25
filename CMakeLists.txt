
cmake_minimum_required(VERSION 3.16)
project(cppRestAPI VERSION 2.3.0)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GenerateExportHeader)

option(CppRestAPI_QtBackend "Qt backend for CppRestAPI" OFF)
option(CppRestAPI_CurlBackend "libcurl backend for CppRestAPI" OFF)
option(CppRestAPI_CppHttplibBackend "cpp-httplib backend for CppRestAPI" OFF)
option(CppRestAPI_JsonPagination "JSON-aware Link-header pagination strategy (requires jsoncpp)" ON)
option(CppRestAPI_GitHub "GitHub API helpers (ConnectionBuilder, Request). Implies CppRestAPI_JsonPagination." ON)
option(CppRestAPI_Tests "Enable CppRestAPI tests. Forces all backends to be ON" OFF)
option(CppRestAPI_Examples "Build CppRestAPI examples. Forces all backends to be ON" OFF)

if(CppRestAPI_Tests OR CppRestAPI_Examples)
    set(CppRestAPI_QtBackend ON)
    set(CppRestAPI_CurlBackend ON)
    set(CppRestAPI_CppHttplibBackend ON)
    set(CppRestAPI_JsonPagination ON)
    set(CppRestAPI_GitHub ON)
endif()

if(CppRestAPI_GitHub AND NOT CppRestAPI_JsonPagination)
    message(STATUS "CppRestAPI_GitHub implies CppRestAPI_JsonPagination; enabling it.")
    set(CppRestAPI_JsonPagination ON)
endif()

if(NOT CppRestAPI_QtBackend AND NOT CppRestAPI_CurlBackend AND NOT CppRestAPI_CppHttplibBackend)
    message(FATAL_ERROR "No backend was chosen. Set CppRestAPI_QtBackend, CppRestAPI_CurlBackend or CppRestAPI_CppHttplibBackend variable to ON")
endif()

add_subdirectory(src)

if(CppRestAPI_Tests)
    enable_testing()
    add_subdirectory(tests)
endif()

if(CppRestAPI_Examples)
    add_subdirectory(examples)
endif()
