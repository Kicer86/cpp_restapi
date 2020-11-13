
add_library(github_api_curl OBJECT
    connection.cpp
    connection.hpp
    github_api.cpp
)

target_link_libraries(github_api_curl
    PUBLIC
)

target_include_directories(github_api_curl
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
)
