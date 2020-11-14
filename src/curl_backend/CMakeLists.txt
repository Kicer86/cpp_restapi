
find_package(CURL REQUIRED)

target_sources(github_api PRIVATE
    ${PROJECT_SOURCE_DIR}/include/github_api/github_api_curl.hpp
    connection.cpp
    connection.hpp
    github_api.cpp
)

target_link_libraries(github_api
    PUBLIC
        CURL::libcurl
)
