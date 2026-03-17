
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

find_package(httplib)

if(httplib_FOUND)
    set(HTTPLIB_LIBRARIES httplib::httplib)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(HTTPLIB REQUIRED cpp-httplib)
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
        ${HTTPLIB_LIBRARIES}

        cpp_restapi
)

add_test(NAME api_tests COMMAND api_tests)
