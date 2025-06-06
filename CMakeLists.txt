cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(
    beamburst 
    LANGUAGES CXX
)

add_compile_options(
    -g
    -O3
    -Wall
    -Werror
    -Wextra
)


find_package(Git REQUIRED)

if (${COMPILE_WITH_DOCTEST})
    include(FetchContent)

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
endif()

#### beamburst math
add_library(
    beamburst_linear_algebra
    src/linear_algebra/vector_ops.cc
    src/linear_algebra/matrix.cc
    src/linear_algebra/coordinates.cc
    src/linear_algebra/cmath_extention.cc
)
target_include_directories(
    beamburst_linear_algebra
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
)

#### beamburst geometry
add_library(
    beamburst_geometry
    src/geometry/primitives/sphere.cc
    src/geometry/primitives/plane.cc
    src/geometry/primitives/triangle.cc
    src/geometry/multitudes/bvh.cc
    src/geometry/line.cc
    src/geometry/aabb.cc
)

target_include_directories(
    beamburst_geometry
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
)

#### beamburst camera
add_library(
    camera
    src/camera.cc
    src/lighting.cc
    src/image.cc
    src/color.cc
    src/light_graph.cc
    src/texture.cc
)
target_link_libraries(
    camera
    beamburst_geometry
    beamburst_linear_algebra
)
target_include_directories(
    camera
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
)

#### beamburst parser 
add_library(
    parser
    src/parsing/parser.cc
)
target_include_directories(
    parser
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
)

add_subdirectory(tests)

