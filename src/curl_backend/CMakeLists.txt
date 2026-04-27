
find_package(CURL REQUIRED)

target_sources(cpp_restapi
    PRIVATE
        connection.cpp
        sse_connection.cpp
)

target_link_libraries(cpp_restapi
    PUBLIC
        CURL::libcurl
)
