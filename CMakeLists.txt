cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(
    beamburst 
    LANGUAGES CXX
)

add_compile_options(
    -g
    -Wall
    -Werror
    -Wextra
)

include(FetchContent)

find_package(Git REQUIRED)

FetchContent_Declare(
    doctest
    SOURCE_DIR ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG origin/master 
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
FetchContent_MakeAvailable(doctest)
set(DOCTEST_INCLUDE_DIR ${CMAKE_BINARY_DIR}/doctest/doctest)
include(${CMAKE_BINARY_DIR}/doctest/scripts/cmake/doctest.cmake)

enable_testing()

#### beamburst math
add_library(
    beamburst_math
    src/math/vector_ops.cc
    src/math/matrix.cc
    src/math/coordinates.cc
    src/math/cmath_extention.cc
)
target_include_directories(
    beamburst_math
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
)

#### beamburst geometry
add_library(
    beamburst_geometry
    src/geometry/intersections.cc
    src/geometry/sphere.cc
    src/geometry/plane.cc
    src/geometry/triangle.cc
)
target_include_directories(
    beamburst_geometry
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
    ${CMAKE_SOURCE_DIR}/src/geometry
)

#### beamburst camera
add_library(
    camera
    src/camera.cc
    src/lighting.cc
    src/image.cc
    src/color.cc
)
target_link_libraries(
    camera
    beamburst_geometry
    beamburst_math
)
target_include_directories(
    camera
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
    ${CMAKE_SOURCE_DIR}/src/geometry
)

#### beamburst parser 
add_library(
    parser
    src/parsing/parser.cc
)
#### parser tests 
# add_executable(
# parser_test
# tests/parsing/basic_obj_parse.cc
# )
# target_link_libraries(
# parser_test
# parser
# )
# target_include_directories(
# parser_test
# PUBLIC 
# ${DOCTEST_INCLUDE_DIR} 
# ${CMAKE_SOURCE_DIR}/src/parsing
# )
# doctest_discover_tests(parser_test)

#### pngtest
add_executable(
    png_test
    tests/pngtest.cc
)
target_link_libraries(
    png_test
    camera
    png
)
target_include_directories(
    png_test 
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
)
doctest_discover_tests(png_test)


#### beamburst_math_test
add_executable(
    beamburst_math_test
    tests/math/matrix.cc
    tests/math/array_ops.cc
    tests/math/array_2d_ops.cc
    tests/math/vector_ops.cc
    tests/math/transformations.cc
)

target_link_libraries(
    beamburst_math_test
    beamburst_math
)

target_include_directories(
    beamburst_math_test
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
)
doctest_discover_tests(beamburst_math_test)


#### beamburst geometry test
add_executable(
    beamburst_geometry_test
    tests/geometry/intersections.cc
)
target_link_libraries(
    beamburst_geometry_test
    beamburst_geometry
)
target_include_directories(
    beamburst_geometry_test
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
    ${CMAKE_SOURCE_DIR}/src/geometry
)
doctest_discover_tests(beamburst_geometry_test)


# camera test
add_executable(
    camera_test 
    tests/camera_test.cc
)
target_link_libraries(
    camera_test 
    png
    beamburst_math
    beamburst_geometry
    camera
)
target_include_directories(
    camera_test 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
    ${CMAKE_SOURCE_DIR}/src/geometry
)
target_compile_options(
    camera_test
    PRIVATE
    -O3
)

# cube test
add_executable(
    cube_test 
    tests/cube_test.cc
)
target_link_libraries(
    cube_test 
    png
    beamburst_math
    beamburst_geometry
    camera
    parser
)
target_include_directories(
    cube_test 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/parsing
)
target_compile_options(
    cube_test 
    PRIVATE
    -O3
)

#### beamburst texture test
add_executable(
    texture_test
    tests/texture_test.cc
)
target_include_directories(
    texture_test
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/math
)
target_link_libraries(
    texture_test
    camera
    png
    beamburst_math
)
