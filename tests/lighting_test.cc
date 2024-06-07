#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "light_graph.h"
#include "material.h"
#include "primitives/sphere.h"

int main()
{
    size_t constexpr img_width = 1200;
    Screen constexpr screen{
        .discretization = {img_width, img_width},
        .size = {1.0, 1.0}
    };

    Camera camera(
        screen,
        1.0,
        {0, 0, -4}, // pos
        {0, 0, 1},  // view
        {0, 1, 0}   // up
    );

    Material sphere_mat{};
    sphere_mat.set_index_of_refraction(1.0);
    sphere_mat.set_base_ambient_color({1, 1, 1});
    sphere_mat.set_diffuse_color({1, 1, 1});
    sphere_mat.set_specular_color({1, 1, 1});
    sphere_mat.set_coeffs({0, 1, 0});
    sphere_mat.set_specular_exponent(1);

    Material background_mat{};
    background_mat.set_index_of_refraction(1.0);
    background_mat.set_base_ambient_color({0, 0, 0});
    background_mat.set_diffuse_color({1, 1, 1});
    background_mat.set_specular_color({1, 1, 1});
    background_mat.set_coeffs({1, 0, 0});
    background_mat.set_specular_exponent(1);

    std::vector<std::tuple<Intersectable const*, Material const*>> group{};
    std::vector<std::tuple<Sphere, Material*>> spheres;
    spheres.push_back({Sphere({0, 0, 0}, 1), &sphere_mat});

    for (auto& [sphere, mat_ptr] : spheres) {
        group.push_back({&sphere, mat_ptr});
    }

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{{width, height}, {255, 255, 255, 255}};
    PointLight const light(
        camera.get_position() + std::array<double, 3>{0, 4, 0}
    );

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&background_mat, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(group, &background_mat);
            auto const vcol = root.light_pixel(camera, light);
            img.set_color_at(x, y, to_color(vcol));
        }
    }
    img.save("lighting_test.png");
}
