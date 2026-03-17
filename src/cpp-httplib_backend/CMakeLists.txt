
find_package(httplib QUIET)

if(NOT httplib_FOUND)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(HTTPLIB QUIET cpp-httplib)
    endif()
endif()

if(NOT httplib_FOUND AND NOT HTTPLIB_FOUND)
    include(FetchContent)
    FetchContent_Declare(httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.37.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(httplib)
endif()

if(TARGET httplib::httplib)
    set(HTTPLIB_LIBRARIES httplib::httplib)
endif()

target_sources(cpp_restapi
    PRIVATE
        connection.cpp
        sse_connection.cpp
)

target_link_libraries(cpp_restapi
    PUBLIC
        ${HTTPLIB_LIBRARIES}
)

target_include_directories(cpp_restapi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
