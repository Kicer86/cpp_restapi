
find_package(Qt5 REQUIRED COMPONENTS Core Network)

include(GenerateExportHeader)

qt5_wrap_cpp(MOCED
    connection.hpp
)

add_library(github_api
    iconnection.cpp
    iconnection.hpp
    connection.cpp
    connection.hpp
    github_api.cpp
    github_api.hpp
    request.cpp
    request.hpp
    ${MOCED}
)

target_link_libraries(github_api
    PUBLIC
        Qt::Core
        Qt::Network
)

target_include_directories(github_api
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
)

generate_export_header(github_api)

if(BUILD_SHARED_LIBS)
    install(TARGETS github_api RUNTIME DESTINATION ${PATH_LIBS}
                               LIBRARY DESTINATION ${PATH_LIBS})
endif()
