
find_package(httplib 0.14.0 REQUIRED)

target_sources(cpp_restapi
    PRIVATE
        connection.cpp
)

target_link_libraries(cpp_restapi
    PUBLIC
        httplib::httplib
)

target_include_directories(cpp_restapi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
