
add_library(cpp_restapi_github OBJECT
    request.cpp
)

set_target_properties(cpp_restapi_github PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(cpp_restapi_github PRIVATE cpp_restapi_github_EXPORTS)

target_link_libraries(cpp_restapi_github
    PUBLIC
        cpp_restapi_core
        cpp_restapi_json_pagination
)

generate_export_header(cpp_restapi_github
    BASE_NAME cpp_restapi_github
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/cpp_restapi_github_export.h
)
