
# Optional component with GitHub API helpers (ConnectionBuilder, Request).
# Relies on LinkHeaderPaginationStrategy from cpp_restapi_json_pagination
# to merge paginated responses.
add_library(cpp_restapi_github
    request.cpp
)

target_link_libraries(cpp_restapi_github
    PUBLIC
        cpp_restapi
        cpp_restapi_json_pagination
)

generate_export_header(cpp_restapi_github
    BASE_NAME cpp_restapi_github
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/cpp_restapi_github_export.h
)

set_target_properties(cpp_restapi_github PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Re-export via cpp_restapi so downstream users only have to link
# against cpp_restapi to get the GitHub helpers.
target_link_libraries(cpp_restapi INTERFACE cpp_restapi_github)
