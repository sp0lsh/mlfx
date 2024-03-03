
include(${PROJECT_SOURCE_DIR}/cmake/stb.cmake)

set(TARGET ${CMAKE_PROJECT_NAME}-android-test)
# file(GLOB_RECURSE FILES_TESTS_SRC src/*.cpp)
# add_executable(${TARGET} ${FILES_TESTS_SRC})
add_executable(${TARGET}
    src/main.cpp
)

target_link_libraries(${TARGET} PRIVATE
    log
    android
    mediandk
    EGL
    GLESv3
    mlfx-lib
)

target_link_libraries(${TARGET} PRIVATE
    ${CMAKE_PROJECT_NAME}-test
)

target_include_directories(${TARGET} PRIVATE
    ${STB_INCLUDE_DIR}
)

