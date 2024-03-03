
execute_process(COMMAND git rev-parse --show-cdup
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  OUTPUT_VARIABLE relative_path_to_repository_root
  OUTPUT_STRIP_TRAILING_WHITESPACE)

get_filename_component(REPOSITORY_ROOT
  "${CMAKE_CURRENT_LIST_DIR}/${relative_path_to_repository_root}"
  ABSOLUTE)

message(STATUS "REPOSITORY_ROOT: ${REPOSITORY_ROOT}")

# https://stackoverflow.com/questions/48396863/how-to-include-external-shared-library-with-several-dlls-dylibs
add_library(angle::EGL SHARED IMPORTED)
set_target_properties(angle::EGL PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${REPOSITORY_ROOT}/third_parties/angle-lib/include/EGL
  IMPORTED_LOCATION ${REPOSITORY_ROOT}/third_parties/angle-lib/lib/macos/libEGL.dylib)

add_library(angle::GLES3 SHARED IMPORTED)
set_target_properties(angle::GLES3 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${REPOSITORY_ROOT}/third_parties/angle-lib/include/GLES3
  IMPORTED_LOCATION ${REPOSITORY_ROOT}/third_parties/angle-lib/lib/macos/libGLESv2.dylib)

add_library(angle::angle INTERFACE IMPORTED)
set_property(TARGET angle::angle PROPERTY
  INTERFACE_LINK_LIBRARIES angle::EGL angle::GLES3)  
target_include_directories(angle::angle INTERFACE
  ${REPOSITORY_ROOT}/third_parties/angle-lib/include)
