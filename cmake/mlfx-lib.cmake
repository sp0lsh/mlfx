
add_library(
    mlfx-lib
    STATIC
    IMPORTED
)

MESSAGE("\n\nmlfx-lib.cmake")
MESSAGE("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
MESSAGE("ANDROID_ABI: ${ANDROID_ABI}")

execute_process(COMMAND git rev-parse --show-cdup
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  OUTPUT_VARIABLE relative_path_to_repository_root
  OUTPUT_STRIP_TRAILING_WHITESPACE)

get_filename_component(repository_root
  "${CMAKE_CURRENT_LIST_DIR}/${relative_path_to_repository_root}"
  ABSOLUTE)
 
message(STATUS "repository_root: ${repository_root}")

set_target_properties(mlfx-lib PROPERTIES IMPORTED_LOCATION
    ${repository_root}/release/lib/${ANDROID_ABI}/libmlfx-lib.a
)
set(MLFX_INCLUDE_DIR ${repository_root}/release/include)
