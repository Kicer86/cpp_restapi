
add_library(cpp_restapi_core OBJECT
    base_connection.cpp
    header_utils.cpp
    threaded_connection.cpp
    sse_parser.cpp
)

set_target_properties(cpp_restapi_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(cpp_restapi_core
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(cpp_restapi_core PRIVATE cpp_restapi_EXPORTS)

generate_export_header(cpp_restapi_core
    BASE_NAME cpp_restapi
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/cpp_restapi_export.h
)

configure_file(
    ${PROJECT_SOURCE_DIR}/include/cpp_restapi/version.hpp.in
    ${PROJECT_BINARY_DIR}/cpp_restapi/version.hpp
    @ONLY
)

if(CppRestAPI_QtBackend)
    add_subdirectory(qt_backend)
endif()

if(CppRestAPI_CurlBackend)
    add_subdirectory(curl_backend)
endif()

if(CppRestAPI_CppHttplibBackend)
    add_subdirectory(cpp-httplib_backend)
endif()

if(CppRestAPI_JsonPagination)
    add_subdirectory(json_pagination)
endif()

if(CppRestAPI_GitHub)
    add_subdirectory(services/github)
endif()

# Umbrella library: the only artifact consumers link against. Composed
# entirely of OBJECT libraries; type follows BUILD_SHARED_LIBS.
set(_cpp_restapi_stub "${PROJECT_BINARY_DIR}/cpp_restapi_umbrella.cpp")
file(WRITE ${_cpp_restapi_stub} "// Auto-generated stub for cpp_restapi umbrella library.\n")

add_library(cpp_restapi ${_cpp_restapi_stub})

target_link_libraries(cpp_restapi PUBLIC cpp_restapi_core)

if(TARGET cpp_restapi_json_pagination)
    target_link_libraries(cpp_restapi PUBLIC cpp_restapi_json_pagination)
endif()

if(TARGET cpp_restapi_github)
    target_link_libraries(cpp_restapi PUBLIC cpp_restapi_github)
endif()

set_target_properties(cpp_restapi PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
