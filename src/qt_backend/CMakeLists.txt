
find_package(Qt5 5.12 REQUIRED COMPONENTS Core Network)

target_sources(github_api PRIVATE
    ${PROJECT_SOURCE_DIR}/include/github_api/github_api_qt.hpp
    connection.cpp
    connection.hpp
    github_api.cpp
)

target_link_libraries(github_api
    PUBLIC
        Qt5::Core
        Qt5::Network
)
