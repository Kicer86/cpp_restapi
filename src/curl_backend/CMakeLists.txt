
find_package(CURL REQUIRED)

target_sources(cpp_restapi
    PRIVATE
        connection.cpp
)

target_link_libraries(cpp_restapi
    PUBLIC
        CURL::libcurl
)

target_include_directories(cpp_restapi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
