
cmake_minimum_required(VERSION 3.16)
project(cppRestAPI VERSION 2.3.0)

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

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GenerateExportHeader)

option(CppRestAPI_QtBackend "Qt backend for CppRestAPI" OFF)
option(CppRestAPI_CurlBackend "libcurl backend for CppRestAPI" OFF)
option(CppRestAPI_CppHttplibBackend "cpp-httplib backend for CppRestAPI" OFF)
option(CppRestAPI_Tests "Enable CppRestAPI tests. Forces all backends to be ON" OFF)
option(CppRestAPI_Examples "Build CppRestAPI examples. Forces all backends to be ON" OFF)

if(CppRestAPI_Tests OR CppRestAPI_Examples)
    set(CppRestAPI_QtBackend ON)
    set(CppRestAPI_CurlBackend ON)
    set(CppRestAPI_CppHttplibBackend ON)
endif()

add_library(cpp_restapi
    src/base_connection.cpp
    src/threaded_connection.cpp
    src/header_utils.cpp
    src/sse_parser.cpp
    src/link_header_pagination_strategy.cpp
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

configure_file(
    ${PROJECT_SOURCE_DIR}/include/cpp_restapi/version.hpp.in
    ${PROJECT_BINARY_DIR}/cpp_restapi/version.hpp
    @ONLY
)

set_target_properties(cpp_restapi PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(NOT CppRestAPI_QtBackend AND NOT CppRestAPI_CurlBackend AND NOT CppRestAPI_CppHttplibBackend)
    message(FATAL_ERROR "No backend was chosen. Set CppRestAPI_QtBackend, CppRestAPI_CurlBackend or CppRestAPI_CppHttplibBackend variable to ON")
endif()

if(CppRestAPI_QtBackend)
    add_subdirectory(src/qt_backend)
endif()

if(CppRestAPI_CurlBackend)
    add_subdirectory(src/curl_backend)
endif()

if(CppRestAPI_CppHttplibBackend)
    add_subdirectory(src/cpp-httplib_backend)
endif()

if(CppRestAPI_Tests)
    enable_testing()
    add_subdirectory(tests)
endif()

if(CppRestAPI_Examples)
    add_subdirectory(examples)
endif()

