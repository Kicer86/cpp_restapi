
find_package(Qt5 REQUIRED COMPONENTS Core Network)

include(GenerateExportHeader)

qt5_wrap_cpp(MOCED
    src/connection.hpp
)

add_library(github_api
    include/github_api/github_api.hpp
    include/github_api/iconnection.hpp
    include/github_api/request.hpp
    src/connection.cpp
    src/connection.hpp
    src/github_api.cpp
    src/request.cpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

generate_export_header(github_api)

if(BUILD_SHARED_LIBS)
    install(TARGETS github_api RUNTIME DESTINATION ${PATH_LIBS}
                               LIBRARY DESTINATION ${PATH_LIBS})
endif()
