
if(CppRestAPI_UseQt5)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
else()
    find_package(Qt6 COMPONENTS Core Network)

    if(NOT Qt6_FOUND)
        find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
    endif()
endif()

add_executable(github_qt_example github_qt_example.cpp)
add_executable(github_curl_example github_curl_example.cpp)
add_executable(github_cpp-httplib_example github_cpp-httplib_example.cpp)
add_executable(bare_curl_connection_example bare_curl_connection_example.cpp)
add_executable(bare_qt_connection_example bare_qt_connection_example.cpp)
add_executable(bare_cpp-httplib_connection_example bare_cpp-httplib_connection_example.cpp)
add_executable(sse_qt_example sse_qt_example.cpp)
add_executable(sse_curl_example sse_curl_example.cpp)
add_executable(sse_cpp-httplib_example sse_cpp-httplib_example.cpp)
add_executable(async_curl_example async_curl_example.cpp)
add_executable(async_cpp-httplib_example async_cpp-httplib_example.cpp)
add_executable(async_qt_example async_qt_example.cpp)

target_link_libraries(github_qt_example
    PRIVATE
        cpp_restapi
        Qt::Network
)

target_link_libraries(github_curl_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(github_cpp-httplib_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(bare_curl_connection_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(bare_qt_connection_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(bare_cpp-httplib_connection_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(sse_qt_example
    PRIVATE
        cpp_restapi
        Qt::Network
)

target_link_libraries(sse_curl_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(sse_cpp-httplib_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(async_curl_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(async_cpp-httplib_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(async_qt_example
    PRIVATE
        cpp_restapi
        Qt::Network
)
