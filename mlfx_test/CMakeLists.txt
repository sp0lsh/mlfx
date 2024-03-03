
set(TARGET ${CMAKE_PROJECT_NAME}-test)

file(GLOB_RECURSE FILES_TESTS_SRC src/*.cpp)

# Storing tests in a common lib
# https://github.com/catchorg/Catch2/issues/720
# https://stackoverflow.com/a/72368810
add_library(${TARGET} OBJECT ${FILES_TESTS_SRC})

target_link_libraries(${TARGET} PUBLIC
    ${CMAKE_PROJECT_NAME}-lib
)

target_compile_definitions(${TARGET} PUBLIC
    CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}
)

target_include_directories(${TARGET} PUBLIC src)
