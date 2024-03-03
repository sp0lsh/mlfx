
# dependecies
include(${PROJECT_SOURCE_DIR}/cmake/angle.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

# target
set(TARGET ${CMAKE_PROJECT_NAME}-standalone)

file(GLOB_RECURSE FILES_SRC src/*.cpp)
add_executable(${TARGET} ${FILES_SRC})

target_link_libraries(${TARGET}
  mlfx-lib
  angle::angle
  ${SDL2_LIBRARY}
)

target_include_directories(${TARGET} PRIVATE
  ${SDL2_INCLUDE_DIR}
)
