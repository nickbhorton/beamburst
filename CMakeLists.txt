cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(
    beamburst 
    LANGUAGES CXX
)

add_compile_options(
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Debug>:-Wall>
    $<$<CONFIG:Debug>:-Werror>
    $<$<CONFIG:Debug>:-Wextra>
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

#### gen 
add_library(
    gen
    src/image.cc
    src/color.cc
)
####

#### pngtest
add_executable(
    png_test
    tests/pngtest.cc
)

target_link_libraries(
    png_test
    gen
    png
)

target_include_directories(
    png_test 
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
)
doctest_discover_tests(png_test)
####

#### matrix
add_library(
    matrix
    src/vector_ops.cc
    src/matrix.cc
)
####

#### matrix test
add_executable(
    matrix_test
    tests/matrix.cc
)

target_link_libraries(
    matrix_test
    matrix
)

target_include_directories(
    matrix_test 
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
)
doctest_discover_tests(matrix_test)
####

#### vector ops test
add_executable(
    vector_ops_test
    tests/vector_ops.cc
)

target_link_libraries(
    vector_ops_test
    matrix
)

target_include_directories(
    vector_ops_test
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
)
doctest_discover_tests(vector_ops_test)
####

#### array ops test
add_executable(
    array_ops_test
    tests/array_ops.cc
)

target_include_directories(
    array_ops_test
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
)
doctest_discover_tests(array_ops_test)
####

#### array 2d ops test
add_executable(
    array_2d_ops_test
    tests/array_2d_ops.cc
)

target_include_directories(
    array_2d_ops_test
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
)
doctest_discover_tests(array_2d_ops_test)
####

