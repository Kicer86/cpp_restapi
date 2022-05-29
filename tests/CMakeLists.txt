
find_package(GTest REQUIRED CONFIG)

include(FetchContent)

FetchContent_Declare(httpmockserver
    GIT_REPOSITORY https://github.com/seznam/httpmockserver.git
    GIT_TAG 38d4425470aaed56a9d2a4c736500d405e74d2e1
)

set(USE_SYSTEM_CPR   FALSE)
set(USE_SYSTEM_GTEST TRUE)
FetchContent_MakeAvailable(httpmockserver)

add_executable(api_tests
    api_tests.cpp
    main.cpp
)

target_link_libraries(api_tests
    PRIVATE
        GTest::gtest
        GTest::gmock
        httpmockserver
        microhttpd

        github_api
)

target_include_directories(api_tests
    PRIVATE
        ${httpmockserver_SOURCE_DIR}/include
)

add_test(NAME api_tests COMMAND api_tests)
