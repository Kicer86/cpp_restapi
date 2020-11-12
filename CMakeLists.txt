
project(GitHubAPI)

include(GenerateExportHeader)

option(GitHubAPI_QtBackend "Qt backend for GitHubApi" OFF)
option(GitHubAPI_CurlBackend "libcurl backend for GitHubApi" OFF)

add_library(github_api
    include/github_api/iconnection.hpp
    include/github_api/request.hpp
    src/request.cpp
)

target_include_directories(github_api
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

generate_export_header(github_api)

if(NOT GitHubAPI_QtBackend AND NOT GitHubAPI_CurlBackend)
    message(FATAL_ERROR "No backend was chosen. Set either GitHubAPI_QtBackend or GitHubAPI_CurlBackend variable to ON")
endif()

if(GitHubAPI_QtBackend)
    add_subdirectory(src/qt_backend)
    target_sources(github_api PRIVATE include/github_api/github_api_qt.hpp)
    target_link_libraries(github_api PRIVATE github_api_qt)
endif()

if(GitHubAPI_CurlBackend)
    add_subdirectory(src/curl_backend)
endif()
