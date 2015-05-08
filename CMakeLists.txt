

find_package(Qt5Network REQUIRED)

set(SRCS
    connection.cpp
    github_api.cpp
    )

set(HEADERS
    connection.hpp
    github_api.hpp
    )

#set of file to be moced
set(TO_MOC
    
    )

qt5_wrap_cpp(MOCED ${TO_MOC})

add_library(github_api ${SRCS} ${MOCED} ${HEADERS})
