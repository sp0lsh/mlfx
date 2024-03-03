
# dependecies
# TODO: On macos disguise angle as gles3 (libGLEv3)
# there are differeces in core gl on mac vs gles3
# https://stackoverflow.com/questions/22118518/use-of-undeclared-identifier-glgenvertexarrays-error-even-after-including-open
include(${PROJECT_SOURCE_DIR}/cmake/opencv.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/angle.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/stb.cmake)


# target 
set(TARGET ${CMAKE_PROJECT_NAME}-lib)
set(LIBRARY_NAME ${CMAKE_PROJECT_NAME})

set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
)

file(GLOB_RECURSE FILES_SRC src/*.cpp)
file(GLOB_RECURSE FILES_HEADERS ${LIBRARY_HEADERS_DIR}/*.h)
add_library(${TARGET} STATIC
    ${FILES_SRC}
    ${FILES_HEADERS}
)

target_link_libraries(${TARGET} PUBLIC
    ${OpenCV_LIBS}
)

target_include_directories(${TARGET} PUBLIC
    ${STB_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_include_directories(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${TARGET} PUBLIC
    GL_SILENCE_DEPRECATION
)


if( ${MLFX_OS} STREQUAL "darwin" )
    target_link_libraries(${TARGET} PUBLIC
        angle::angle
    )
endif()

if( ${MLFX_OS} STREQUAL "android" )

endif()
