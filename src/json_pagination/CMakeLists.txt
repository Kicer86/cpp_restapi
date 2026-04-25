
# Optional component pulling in the jsoncpp dependency. Contains the
# Link-header based pagination strategy. Users that do not need
# JSON-aware multi-page merging do not have to link against it
# (and do not pay the jsoncpp dependency cost).
find_package(jsoncpp CONFIG)

if(jsoncpp_FOUND)
    set(JSONCPP_LIBRARIES JsonCpp::JsonCpp)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(JSONCPP REQUIRED jsoncpp)
endif()

add_library(cpp_restapi_json_pagination
    link_header_pagination_strategy.cpp
)

target_include_directories(cpp_restapi_json_pagination
    PRIVATE SYSTEM
        ${JSONCPP_INCLUDE_DIRS}
)

target_include_directories(cpp_restapi_json_pagination
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(cpp_restapi_json_pagination
    PUBLIC
        cpp_restapi
    PRIVATE
        ${JSONCPP_LIBRARIES}
)

generate_export_header(cpp_restapi_json_pagination
    BASE_NAME cpp_restapi_json_pagination
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/cpp_restapi_json_pagination_export.h
)

set_target_properties(cpp_restapi_json_pagination PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Re-export via cpp_restapi so downstream users only have to link
# against cpp_restapi to get access to the optional component.
target_link_libraries(cpp_restapi INTERFACE cpp_restapi_json_pagination)
