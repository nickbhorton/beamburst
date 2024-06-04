#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "light_graph.h"
#include "lighting.h"
#include "linear_types.h"
#include "material.h"
#include "parsing/parser.h"
#include "sphere.h"
#include "texture.h"

#include <fstream>

using namespace linalg;

int main()
{
    size_t constexpr img_width = 2048;
    Screen constexpr screen{
        .discretization = {img_width, img_width},
        .size = {1.0, 1.0}
    };

    Camera camera(
        screen,
        1.0,
        {0, 0, -5}, // pos
        {0, 0, 1},  // view
        {0, 1, 0}   // up
    );

    Material tile_material{};
    tile_material.set_index_of_refraction(1.0);
    tile_material.set_base_ambient_color({1, 0, 0});
    tile_material.set_diffuse_color({1, 1, 1});
    tile_material.set_specular_color({1, 1, 1});
    tile_material.set_coeffs({0.2, 0.4, 0.4});
    tile_material.set_specular_exponent(100);
    Texture tiles_color("../tests/resources/Tiles_051_4K_basecolor.png");
    tile_material.set_color_texture(&tiles_color);
    Texture tiles_normal("../tests/resources/Tiles_051_4K_normal.png");
    tile_material.set_normal_texture(&tiles_normal);

    Material abstract_material{};
    abstract_material.set_index_of_refraction(1.0);
    abstract_material.set_reflect_precent(1.0);
    abstract_material.set_diffuse_color({1, 1, 1});
    abstract_material.set_specular_color({1, 1, 1});
    abstract_material.set_coeffs({0.2, 0.4, 0.4});
    abstract_material.set_specular_exponent(100);
    Texture abstract_color("../tests/resources/Abstract_011_basecolor.png");
    abstract_material.set_color_texture(&abstract_color);
    Texture abstract_normal("../tests/resources/Abstract_011_normal.png");
    abstract_material.set_normal_texture(&abstract_normal);

    Material blue_material{};
    blue_material.set_index_of_refraction(1.0);
    blue_material.set_refract_precent(1.0);
    blue_material.set_base_ambient_color({0, 0, 1});
    blue_material.set_diffuse_color({1, 1, 1});
    blue_material.set_specular_color({1, 1, 1});
    blue_material.set_coeffs({0.2, 0.4, 0.4});
    blue_material.set_specular_exponent(100);

    Material bg_material{};
    bg_material.set_index_of_refraction(1.0);
    bg_material.set_refract_precent(1.0);
    bg_material.set_base_ambient_color({0.1, 0.1, 0.1});
    bg_material.set_diffuse_color({1, 1, 1});
    bg_material.set_specular_color({1, 1, 1});
    bg_material.set_coeffs({0.2, 0.3, 0.5});
    bg_material.set_specular_exponent(1);

    std::ifstream cube_file("../resources/objects/cube.obj", std::ifstream::in);
    VertexObject cube_obj(cube_file);
    double const scale{2};
    VertexObject cube = cube_obj.copy_and_transform(
        {{{scale, 0, 0, 0}, {0, scale, 0, 0}, {0, 0, scale, 6}, {0, 0, 0, 1}}}
    );

    BVHNode bvh{};
    std::vector<Triangle> const triangles = cube.extract_triangles();
    for (auto const& triangle : triangles) {
        bvh.add_primitive(&triangle);
    }
    bvh.construct_tree();
    std::vector<std::tuple<Intersectable*, Material*>> group{};
    group.push_back({&bvh, &abstract_material});

    std::vector<std::tuple<Sphere, Material*>> spheres;
    spheres.push_back({Sphere({0, 0, 0}, 1), &blue_material});
    spheres.push_back({Sphere({0, 0, 0}, 0.9), &bg_material});

    for (auto& [sphere, mat_ptr] : spheres) {
        group.push_back({&sphere, mat_ptr});
    }

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{{width, height}, {255, 255, 255, 255}};
    PointLight const light(camera.get_position());

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&bg_material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(group, &bg_material);
            vec3 const vcol =
                root.calculate_color(camera, light, root.sum_light_intensity());
            img.set_color_at(x, y, to_color(vcol));
        }
    }

    img.save("bubble_test.png");
}
