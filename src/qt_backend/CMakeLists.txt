
find_package(Qt5 5.12 REQUIRED COMPONENTS Core Network)

add_library(github_api_qt OBJECT
    connection.cpp
    connection.hpp
)

target_link_libraries(github_api_qt
    PUBLIC
        Qt5::Core
        Qt5::Network
)

target_include_directories(github_api_qt
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
)
