
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP REQUIRED jsoncpp)

target_sources(github_api
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/github_api/github_api_curl.hpp
        connection.cpp
        connection.hpp
        github_api.cpp
)

target_link_libraries(github_api
    PUBLIC
        CURL::libcurl
        ${JSONCPP_LIBRARIES}
)

target_include_directories(github_api
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
