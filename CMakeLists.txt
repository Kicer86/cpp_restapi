
project(GitHubAPI)

include(GenerateExportHeader)

add_subdirectory(src/qt_backend)

add_library(github_api
    include/github_api/github_api.hpp
    include/github_api/iconnection.hpp
    include/github_api/request.hpp
    src/request.cpp
)

target_link_libraries(github_api
    PUBLIC
        github_api_qt
)

target_include_directories(github_api
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

generate_export_header(github_api)
