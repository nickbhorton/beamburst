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

#### pngtest
add_executable(
    pngtest
    src/pngtest.cc
    src/image.cc
    src/color.cc
)

target_link_libraries(
    pngtest
    png
)
####

#### matrix
add_library(
    matrix
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



