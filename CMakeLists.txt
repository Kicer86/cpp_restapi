
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)


include(GenerateExportHeader)

include_directories(SYSTEM
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Network_INCLUDE_DIRS}
                   )

set(SRCS
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
                                 Qt5::Core
                                 Qt5::Network
                     )
target_include_directories(github_api PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

generate_export_header(github_api)
hideSymbols(github_api)
