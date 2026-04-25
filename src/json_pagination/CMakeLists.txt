
find_package(jsoncpp CONFIG)

if(jsoncpp_FOUND)
    set(JSONCPP_LIBRARIES JsonCpp::JsonCpp)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(JSONCPP REQUIRED jsoncpp)
endif()

add_library(cpp_restapi_json_pagination OBJECT
    link_header_pagination_strategy.cpp
)

set_target_properties(cpp_restapi_json_pagination PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(cpp_restapi_json_pagination PRIVATE cpp_restapi_json_pagination_EXPORTS)

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
        cpp_restapi_core
    PRIVATE
        ${JSONCPP_LIBRARIES}
)

generate_export_header(cpp_restapi_json_pagination
    BASE_NAME cpp_restapi_json_pagination
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/cpp_restapi_json_pagination_export.h
)
