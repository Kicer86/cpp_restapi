
find_package(GTest CONFIG)

if(NOT GTest_FOUND)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )

    FetchContent_MakeAvailable(googletest)
endif()

find_package(httplib QUIET)

if(NOT httplib_FOUND AND NOT TARGET httplib::httplib)
    include(FetchContent)
    FetchContent_Declare(httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.37.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(httplib)
endif()


add_executable(api_tests
    connection_tests.cpp
    github_request_tests.cpp
    sse_parser_tests.cpp
    main.cpp
)

target_link_libraries(api_tests
    PRIVATE
        GTest::gtest
        GTest::gmock
        httplib::httplib

        cpp_restapi
)

add_test(NAME api_tests COMMAND api_tests)
