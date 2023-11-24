
if(GitHubAPI_UseQt6)
    find_package(Qt6 REQUIRED COMPONENTS Core Network)
else()
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
endif()

target_sources(github_api PRIVATE
    ${PROJECT_SOURCE_DIR}/include/github_api/github_api_qt.hpp
    connection.cpp
    github_api.cpp
)

target_link_libraries(github_api
    PUBLIC
        Qt::Core
        Qt::Network
)

target_include_directories(github_api
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
