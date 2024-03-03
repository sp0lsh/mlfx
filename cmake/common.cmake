
# https://cmake.org/cmake/help/latest/command/return.html#command:return
function(get_repository_root REPOSITORY_ROOT)
  # This propagates the variables through the enclosing block and
  # out to the caller of the function.
  execute_process(COMMAND git rev-parse --show-cdup
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    OUTPUT_VARIABLE relative_path_to_repository_root
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  
  get_filename_component(REPOSITORY_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/${relative_path_to_repository_root}"
    ABSOLUTE)
  
  set(REPOSITORY_ROOT ${REPOSITORY_ROOT} PARENT_SCOPE)
endfunction()
