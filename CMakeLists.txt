
find_package(Qt5 REQUIRED COMPONENTS Core Network)

include(GenerateExportHeader)

set(SRCS
    iconnection.cpp
    connection.cpp
    github_api.cpp
    request.cpp
    )

set(HEADERS
    iconnection.hpp
    connection.hpp
    github_api.hpp
    request.hpp
    )

#set of file to be moced
set(TO_MOC
    connection.hpp
    )

qt5_wrap_cpp(MOCED ${TO_MOC})

add_library(github_api ${SRCS} ${MOCED} ${HEADERS})
target_link_libraries(github_api PRIVATE
                                 Qt::Core
                                 Qt::Network
                     )
target_include_directories(github_api PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

generate_export_header(github_api)
hideSymbols(github_api)

if(BUILD_SHARED_LIBS)
    install(TARGETS github_api RUNTIME DESTINATION ${PATH_LIBS}
                               LIBRARY DESTINATION ${PATH_LIBS})
endif()

