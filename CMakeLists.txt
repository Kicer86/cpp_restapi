
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

add_library(cpp_restapi_core OBJECT
    src/base_connection.cpp
    src/threaded_connection.cpp
    src/sse_parser.cpp
)

set_target_properties(cpp_restapi_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(cpp_restapi_core
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(cpp_restapi_core PRIVATE cpp_restapi_EXPORTS)

generate_export_header(cpp_restapi_core
    BASE_NAME cpp_restapi
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/cpp_restapi_export.h
)

configure_file(
    ${PROJECT_SOURCE_DIR}/include/cpp_restapi/version.hpp.in
    ${PROJECT_BINARY_DIR}/cpp_restapi/version.hpp
    @ONLY
)

if(CppRestAPI_QtBackend)
    add_subdirectory(src/qt_backend)
endif()

if(CppRestAPI_CurlBackend)
    add_subdirectory(src/curl_backend)
endif()

if(CppRestAPI_CppHttplibBackend)
    add_subdirectory(src/cpp-httplib_backend)
endif()

if(CppRestAPI_JsonPagination)
    add_subdirectory(src/json_pagination)
endif()

if(CppRestAPI_GitHub)
    add_subdirectory(src/services/github)
endif()

# Umbrella library: the only artifact consumers link against. Composed
# entirely of OBJECT libraries; type follows BUILD_SHARED_LIBS.
set(_cpp_restapi_stub "${PROJECT_BINARY_DIR}/cpp_restapi_umbrella.cpp")
file(WRITE ${_cpp_restapi_stub} "// Auto-generated stub for cpp_restapi umbrella library.\n")

add_library(cpp_restapi ${_cpp_restapi_stub})

target_link_libraries(cpp_restapi PUBLIC cpp_restapi_core)

if(TARGET cpp_restapi_json_pagination)
    target_link_libraries(cpp_restapi PUBLIC cpp_restapi_json_pagination)
endif()

if(TARGET cpp_restapi_github)
    target_link_libraries(cpp_restapi PUBLIC cpp_restapi_github)
endif()

set_target_properties(cpp_restapi PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(CppRestAPI_Tests)
    enable_testing()
    add_subdirectory(tests)
endif()

if(CppRestAPI_Examples)
    add_subdirectory(examples)
endif()
