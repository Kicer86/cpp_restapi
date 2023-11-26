
# For compatibility. TODO: remove
if(GitHubAPI_UseQt6)
    set(CppRestAPI_UseQt6 ${GitHubAPI_UseQt6})
    message(WARNING "GitHubAPI_UseQt6 is deprecated, please use CppRestAPI_UseQt6")
endif()

if(CppRestAPI_UseQt6)
    find_package(Qt6 REQUIRED COMPONENTS Core Network)
else()
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
endif()

target_sources(cpp_restapi PRIVATE
    ${PROJECT_SOURCE_DIR}/include/github_api/github_api_qt.hpp
    connection.cpp
    github_api.cpp
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
