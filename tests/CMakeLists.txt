
find_package(GTest REQUIRED CONFIG)

include(FetchContent)

FetchContent_Declare(httpmockserver
    GIT_REPOSITORY https://github.com/seznam/httpmockserver.git
    GIT_TAG 38d4425470aaed56a9d2a4c736500d405e74d2e1
)

FetchContent_Populate(httpmockserver)

set(USE_SYSTEM_CPR   FALSE)
set(USE_SYSTEM_GTEST TRUE)
add_subdirectory(${httpmockserver_SOURCE_DIR} ${httpmockserver_BINARY_DIR})

add_executable(api_tests
    qtbackend_tests.cpp
)

target_link_libraries(api_tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main

        github_api
)
