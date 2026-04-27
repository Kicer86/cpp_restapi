
# Map CMake options to cmakedefine01 inputs consumed by version.hpp.in.
set(CPP_RESTAPI_HAS_JSON_PAGINATION ${CppRestAPI_JsonPagination})
set(CPP_RESTAPI_HAS_GITHUB ${CppRestAPI_GitHub})

configure_file(
    ${PROJECT_SOURCE_DIR}/include/cpp_restapi/version.hpp.in
    ${PROJECT_BINARY_DIR}/cpp_restapi/version.hpp
    @ONLY
)

add_library(cpp_restapi version.cpp)

target_include_directories(cpp_restapi
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

generate_export_header(cpp_restapi)

set_target_properties(cpp_restapi PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_sources(cpp_restapi PRIVATE
    base_connection.cpp
    header_utils.cpp
    threaded_connection.cpp
    sse_parser.cpp
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
