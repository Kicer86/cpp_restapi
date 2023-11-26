
find_package(CURL REQUIRED)

target_sources(cpp_restapi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/github_api/github_api_curl.hpp
        connection.cpp
        github_api.cpp
)

target_link_libraries(cpp_restapi
    PUBLIC
        CURL::libcurl
)

target_include_directories(cpp_restapi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
