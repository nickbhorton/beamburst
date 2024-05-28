#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "linear_types.h"
#include "material.h"
#include "path_tree.h"
#include "sphere.h"
#include "texture.h"
#include "vector_ops.h"

using namespace linalg;

int main()
{

    size_t constexpr img_width = 1200;
    Screen constexpr screen{
        .discretization = {img_width, img_width},
        .size = {1.0, 1.0}
    };
    Texture tiles(
        "../tests/resources/Tiles_051_4K_basecolor.png",
        "../tests/resources/Tiles_051_4K_normal.png"
    );

    Camera camera(
        screen,
        1.0,
        {0, 0, -3}, // pos
        {0, 0, 1},  // view
        {0, 1, 0}   // up
    );

    Material r_sphere_mat{};
    r_sphere_mat.set_index_of_refraction(1.0);
    r_sphere_mat.set_base_ambient_color({0, 0, 0});
    r_sphere_mat.set_diffuse_color({1, 1, 1});
    r_sphere_mat.set_specular_color({1, 1, 1});
    r_sphere_mat.set_coeffs({0.7, 0.2, 0.1});
    r_sphere_mat.set_specular_exponent(100);
    r_sphere_mat.set_texture(&tiles);

    Material bg_material{};
    bg_material.set_index_of_refraction(1.0);
    bg_material.set_base_ambient_color({0.1, 0.1, 0.1});
    bg_material.set_diffuse_color({1, 1, 1});
    bg_material.set_specular_color({1, 1, 1});
    bg_material.set_coeffs({0.2, 0.3, 0.5});
    bg_material.set_specular_exponent(1);

    std::vector<std::tuple<Sphere, Material*>> spheres;
    spheres.push_back({Sphere({0, 0, 0}, 1), &r_sphere_mat});

    std::vector<std::tuple<Intersectable*, Material*>> os{};
    for (auto& [sphere, mat_ptr] : spheres) {
        os.push_back({&sphere, mat_ptr});
    }

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{{width, height}, {255, 255, 255, 255}};
    PointLight const light({0, 10, 0});

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&bg_material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(os, &bg_material);
            vec3 const vcol =
                root.calculate_color(camera, light, root.sum_light_intensity());
            img.set_color_at(x, y, to_color(vcol));
        }
    }

    img.save("sphere_texture_test.png");
}
