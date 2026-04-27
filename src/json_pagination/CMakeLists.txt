
find_package(jsoncpp CONFIG)

if(jsoncpp_FOUND)
    set(JSONCPP_LIBRARIES JsonCpp::JsonCpp)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(JSONCPP REQUIRED jsoncpp)
endif()

target_sources(cpp_restapi PRIVATE
    link_header_pagination_strategy.cpp
)

target_include_directories(cpp_restapi
    PRIVATE SYSTEM
        ${JSONCPP_INCLUDE_DIRS}
)

target_link_libraries(cpp_restapi
    PRIVATE
        ${JSONCPP_LIBRARIES}
)
