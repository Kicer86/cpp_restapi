
find_package(GTest REQUIRED CONFIG)

add_executable(api_tests
    qtbackend_tests.cpp
)

target_link_libraries(api_tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main

        github_api
)
