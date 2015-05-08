

find_package(Qt5Network REQUIRED)


include(GenerateExportHeader)

include_directories(SYSTEM ${Qt5Network_INCLUDE_DIRS})

set(SRCS
    connection.cpp
    github_api.cpp
    )

set(HEADERS
    aconnection.hpp
    connection.hpp
    github_api.hpp
    )

#set of file to be moced
set(TO_MOC
    
    )

qt5_wrap_cpp(MOCED ${TO_MOC})

add_library(github_api ${SRCS} ${MOCED} ${HEADERS})
target_link_libraries(github_api PRIVATE Qt5::Network)
