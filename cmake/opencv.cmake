
include(${PROJECT_SOURCE_DIR}/cmake/common.cmake)


get_repository_root(REPOSITORY_ROOT)
message(STATUS "REPOSITORY_ROOT: ${REPOSITORY_ROOT}")
message("cmake/openvc.cmake " ${MLFX_OS})

if( ${MLFX_OS} STREQUAL "android" )
    set(OpenCV_DIR "${REPOSITORY_ROOT}/third_parties/opencv-android-sdk/sdk/native/jni")
endif()
    
message(OpenCV_DIR "${OpenCV_DIR}")

find_package(OpenCV REQUIRED)

message("OpenCV_LIBS: ${OpenCV_LIBS}")
message("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
