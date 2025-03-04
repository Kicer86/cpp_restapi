
if(CppRestAPI_UseQt5)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
else()
    find_package(Qt6 COMPONENTS Core Network)

    if(NOT Qt6_FOUND)
        find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
    endif()
endif()

target_sources(cpp_restapi PRIVATE
    connection.cpp
)

target_link_libraries(cpp_restapi
    PUBLIC
        Qt::Core
        Qt::Network
)

target_include_directories(cpp_restapi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
