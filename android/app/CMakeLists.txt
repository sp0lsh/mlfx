# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
project(mlfx-android-app)

set(CMAKE_VERBOSE_MAKEFILE on)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DVK_USE_PLATFORM_ANDROID_KHR -DVK_NO_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SRC_DIR src/main/cpp)


execute_process(COMMAND git rev-parse --show-cdup
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  OUTPUT_VARIABLE REL_PATH_TO_REPO_ROOT
  OUTPUT_STRIP_TRAILING_WHITESPACE)

get_filename_component(ROOT_DIR
  "${CMAKE_CURRENT_LIST_DIR}/${REL_PATH_TO_REPO_ROOT}"
  ABSOLUTE)

message(STATUS "ROOT_DIR: ${ROOT_DIR}")

include(${ROOT_DIR}/cmake/mlfx-lib.cmake)

include_directories(${WRAPPER_DIR})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( media-lib # Sets the name of the library.

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${SRC_DIR}/EngineContext.cpp
             ${SRC_DIR}/EngineJNI.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

find_library( # Sets the name of the path variable.
            GLESv3-lib

            # Specifies the name of the NDK library that
            # you want CMake to locate.
            GLESv3)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( media-lib # Specifies the target library.
                       android
                       ${log-lib}
                       ${GLESv3-lib}
                       mlfx-lib )

target_include_directories( media-lib
                            PUBLIC
                            ${MLFX_INCLUDE_DIR} )


