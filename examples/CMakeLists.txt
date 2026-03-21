
if(CppRestAPI_UseQt5)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
else()
    find_package(Qt6 COMPONENTS Core Network)

    if(NOT Qt6_FOUND)
        find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
    endif()
endif()

add_executable(qt_example qt_example.cpp)
add_executable(curl_example curl_example.cpp)
add_executable(cpp-httplib_example cpp-httplib_example.cpp)
add_executable(bare_curl_connection_example bare_curl_connection_example.cpp)
add_executable(bare_qt_connection_example bare_qt_connection_example.cpp)
add_executable(bare_cpp-httplib_connection_example bare_cpp-httplib_connection_example.cpp)
add_executable(qt_sse_example qt_sse_example.cpp)
add_executable(curl_sse_example curl_sse_example.cpp)
add_executable(cpp-httplib_sse_example cpp-httplib_sse_example.cpp)
add_executable(curl_async_example curl_async_example.cpp)
add_executable(cpp-httplib_async_example cpp-httplib_async_example.cpp)
add_executable(qt_async_example qt_async_example.cpp)
add_executable(curl_coroutine_example curl_coroutine_example.cpp)

target_link_libraries(qt_example
    PRIVATE
        cpp_restapi
        Qt::Network
)

target_link_libraries(curl_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(cpp-httplib_example
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

target_link_libraries(qt_sse_example
    PRIVATE
        cpp_restapi
        Qt::Network
)

target_link_libraries(curl_sse_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(cpp-httplib_sse_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(curl_async_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(cpp-httplib_async_example
    PRIVATE
        cpp_restapi
)

target_link_libraries(qt_async_example
    PRIVATE
        cpp_restapi
        Qt::Network
)

target_link_libraries(curl_coroutine_example
    PRIVATE
        cpp_restapi
)
