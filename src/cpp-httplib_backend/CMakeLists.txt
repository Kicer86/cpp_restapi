
find_package(httplib)

if(httplib_FOUND)
    set(HTTPLIB_LIBRARIES httplib::httplib)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(HTTPLIB REQUIRED cpp-httplib)
endif()


target_sources(cpp_restapi
    PRIVATE
        connection.cpp
)

target_link_libraries(cpp_restapi
    PUBLIC
        ${HTTPLIB_LIBRARIES}
)

target_include_directories(cpp_restapi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
