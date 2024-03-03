# dependecies
include(${PROJECT_SOURCE_DIR}/cmake/angle.cmake)


# target
set(TARGET ${CMAKE_PROJECT_NAME}-standalone-test)

file(GLOB_RECURSE FILES_TESTS_SRC src/*.cpp)
add_executable(${TARGET} ${FILES_TESTS_SRC})

target_link_libraries(${TARGET} PRIVATE
    ${CMAKE_PROJECT_NAME}-test
    angle::angle
)
