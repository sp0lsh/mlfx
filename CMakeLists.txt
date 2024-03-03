cmake_minimum_required(VERSION 3.18.1)

set(PROJECT "mlfx")
project(${PROJECT} LANGUAGES C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MLFX_OS "darwin" CACHE STRING "Target system/platform") # darwin, android
set(MLFX_IS_TEST False CACHE BOOL "Target system/platform") # true, false
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

message("MLFX_OS: " ${MLFX_OS})
message("MLFX_IS_TEST: " ${MLFX_IS_TEST})

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -g")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_subdirectory(mlfx)

if( ${MLFX_OS} STREQUAL "darwin" )

    #  https://github.com/google/sanitizers/wiki/AddressSanitizer
    #
    #  $ clang use-after-free.c -o test
    #  $ clang -fsanitize=address -O1 -fno-omit-frame-pointer -g use-after-free.c -o test
    #
    # add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    # add_link_options(-fsanitize=address -fno-omit-frame-pointer -g)
    # target_compile_options(asan-target PRIVATE -fsanitize=address)
    # target_link_options(asan-target PRIVATE -fsanitize=address)

    add_subdirectory(standalone)

    message("MLFX_IS_TEST: " ${MLFX_IS_TEST})
    if( ${MLFX_IS_TEST} )
        add_subdirectory(mlfx_test)
        add_subdirectory(standalone_test)
    endif()
endif()

if( ${MLFX_OS} STREQUAL "android" )
    if( ${MLFX_IS_TEST} )
        add_subdirectory(mlfx_test)
        add_subdirectory(android_test)
    endif()
endif()
