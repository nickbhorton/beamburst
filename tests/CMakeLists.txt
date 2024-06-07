#### pngtest
add_executable(
    png_test
    pngtest.cc
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

#### beamburst_linear_algebra_test
add_executable(
    beamburst_linear_algebra_test
    linear_algebra/matrix.cc
    linear_algebra/array_ops.cc
    linear_algebra/array_2d_ops.cc
    linear_algebra/vector_ops.cc
    linear_algebra/transformations.cc
)

target_link_libraries(
    beamburst_linear_algebra_test
    beamburst_linear_algebra
)

target_include_directories(
    beamburst_linear_algebra_test
    PUBLIC 
    ${DOCTEST_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
)
doctest_discover_tests(beamburst_linear_algebra_test)


# cube texture test
add_executable(
    cube_texture_test 
    cube_texture_test.cc
)
target_link_libraries(
    cube_texture_test 
    png
    beamburst_linear_algebra
    beamburst_geometry
    camera
    parser
)
target_include_directories(
    cube_texture_test 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/parsing
)

# sphere texture test
add_executable(
    sphere_texture_test 
    sphere_texture_test.cc
)
target_link_libraries(
    sphere_texture_test 
    png
    beamburst_linear_algebra
    beamburst_geometry
    camera
    parser
)
target_include_directories(
    sphere_texture_test 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/parsing
)

# bubble test
add_executable(
    bubble_test 
    bubble_test.cc
)
target_link_libraries(
    bubble_test  
    png
    beamburst_linear_algebra
    beamburst_geometry
    camera
    parser
)
target_include_directories(
    bubble_test   
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/parsing
)

# lighting test
add_executable(
    lighting_test 
    lighting_test.cc
)
target_link_libraries(
    lighting_test  
    png
    beamburst_linear_algebra
    beamburst_geometry
    camera
    parser
)
target_include_directories(
    lighting_test  
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/parsing
)

# cow reflect refract test
add_executable(
    cow_rr_test
    cow_reflect_refract_test.cc
)
target_link_libraries(
    cow_rr_test
    png
    beamburst_linear_algebra
    beamburst_geometry
    camera
    parser
)
target_include_directories(
    cow_rr_test
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/linear_algebra
    ${CMAKE_SOURCE_DIR}/src/geometry
    ${CMAKE_SOURCE_DIR}/src/parsing
)

