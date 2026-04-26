
if(CppRestAPI_UseQt5)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
else()
    find_package(Qt6 COMPONENTS Core Network)

    if(NOT Qt6_FOUND)
        find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
    endif()
endif()

target_sources(cpp_restapi_core PRIVATE
    connection.cpp
    create_connection.cpp
    sse_connection.cpp
)

target_link_libraries(cpp_restapi_core
    PUBLIC
        Qt::Core
        Qt::Network
)

target_include_directories(cpp_restapi_core
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
