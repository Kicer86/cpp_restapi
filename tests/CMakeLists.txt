
find_package(GTest REQUIRED CONFIG)
find_package(httplib REQUIRED)

add_executable(api_tests
    connection_tests.cpp
    github_request_tests.cpp
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
