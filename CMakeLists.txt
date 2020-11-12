
find_package(Qt5 5.12 REQUIRED COMPONENTS Core Network)

include(GenerateExportHeader)

add_library(github_api
    include/github_api/github_api.hpp
    include/github_api/iconnection.hpp
    include/github_api/request.hpp
    src/qt_backend/connection.cpp
    src/qt_backend/connection.hpp
    src/qt_backend/github_api.cpp
    src/request.cpp
)

target_link_libraries(github_api
    PUBLIC
        Qt5::Core
        Qt5::Network
)

target_include_directories(github_api
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

generate_export_header(github_api)
