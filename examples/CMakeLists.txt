
if(CppRestAPI_UseQt6)
    find_package(Qt6 REQUIRED COMPONENTS Core Network)
else()
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
endif()

add_executable(qt_example qt_example.cpp)
add_executable(curl_example curl_example.cpp)
add_executable(cpp-httplib_example cpp-httplib_example.cpp)
add_executable(bare_curl_connection_example bare_curl_connection_example.cpp)
add_executable(bare_qt_connection_example bare_qt_connection_example.cpp)
add_executable(bare_cpp-httplib_connection_example bare_cpp-httplib_connection_example.cpp)

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
