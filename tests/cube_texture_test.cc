#include <fstream>
#include <iostream>

#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "light_graph.h"
#include "lighting.h"
#include "linear_types.h"
#include "material.h"
#include "multitudes/bvh.h"
#include "parser.h"
#include "primitives/triangle.h"

using namespace linalg;

typedef std::tuple<
    std::size_t,
    std::size_t,
    vec3,
    vec3,
    vec2,
    Intersectable const* const>
    pixel_job_t;

int main()
{
    double constexpr axis_distance = 3.3;
    size_t constexpr img_size = 1024;
    Screen constexpr screen{
        .discretization = {img_size, img_size},
        .size = {1.0, 1.0}
    };

    Camera camera(
        screen,
        1.0,
        {axis_distance, axis_distance, axis_distance}, // pos
        {-1.0, -1.0, -1.0},                            // view
        {0.0, 1.0, 0.0}                                // up
    );
    Texture tiles_color("../tests/resources/Tiles_051_4K_basecolor.png");
    Texture tiles_normal("../tests/resources/Tiles_051_4K_normal.png");

    Material cube_material{};
    cube_material.set_index_of_refraction(1.0);
    cube_material.set_refract_precent(0);
    cube_material.set_reflect_precent(0);
    cube_material.set_base_ambient_color({0.9882, 0.4235, 0.52156});
    cube_material.set_diffuse_color({1, 1, 1});
    cube_material.set_specular_color({1, 1, 1});
    cube_material.set_coeffs({0.4, 0.4, 0.2});
    cube_material.set_specular_exponent(100);
    cube_material.set_color_texture(&tiles_color);
    cube_material.set_normal_texture(&tiles_normal);

    Material bg_material{};
    bg_material.set_index_of_refraction(1.0);
    bg_material.set_base_ambient_color({0.4, 0.4, 1.0});
    bg_material.set_diffuse_color({1, 1, 1});
    bg_material.set_specular_color({1, 1, 1});
    bg_material.set_coeffs({0.2, 0.3, 0.5});
    bg_material.set_specular_exponent(100);

    std::ifstream cube_file("../resources/objects/cube.obj", std::ifstream::in);
    // The vertex object owns the vertexes to create a transformation a copy of
    // the vertexes is made not just the triangles.
    VertexObject center_cube(cube_file);

    VertexObject left_cube = center_cube.copy_and_transform(
        {{{1, 0, 0, -3.0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}}
    );
    double constexpr right_cube_scale = 0.75;
    VertexObject right_cube = center_cube.copy_and_transform(
        {{{right_cube_scale, 0, 0, 0},
          {0, right_cube_scale, 0, 0},
          {0, 0, right_cube_scale, -4.0},
          {0, 0, 0, 1}}}
    );

    BVHNode bvh{};
    std::vector<Triangle> const triangles1 = center_cube.extract_triangles();
    for (auto const& triangle : triangles1) {
        bvh.add_primitive(&triangle);
    }
    std::vector<Triangle> const triangles2 = left_cube.extract_triangles();
    for (auto const& triangle : triangles2) {
        bvh.add_primitive(&triangle);
    }
    std::vector<Triangle> const triangles3 = right_cube.extract_triangles();
    for (auto const& triangle : triangles3) {
        bvh.add_primitive(&triangle);
    }
    bvh.construct_tree();

    std::vector<std::tuple<Intersectable const*, Material const*>> os{};
    os.push_back({&bvh, &cube_material});

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{{width, height}, {255, 255, 255, 255}};
    PointLight const light({0, 10, 0});

    size_t xi{0};
    size_t yi{0};

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&bg_material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(os, &bg_material);
            vec3 const vcol =
                root.calculate_color_v1(camera, light, root.sum_light_intensity());
            if (xi == x && yi == y) {
                std::cout << root.to_string();
                std::cout << vcol << "\n";
                std::cout << root.sum_light_intensity() << "\n";
            }
            img.set_color_at(x, y, to_color(vcol));
        }
    }

    img.save("cube_texture_test.png");
}
