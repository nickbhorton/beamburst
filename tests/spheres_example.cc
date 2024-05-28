#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "linear_types.h"
#include "material.h"
#include "path_tree.h"
#include "plane.h"
#include "sphere.h"
#include "vector_ops.h"

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
    constexpr Screen screen{.discretization = {1024, 1024}, .size = {1.0, 1.0}};

    Camera camera(
        screen,
        1.0,
        {0, 2, 0},      // pos
        {0, -0.2, 1},   // view
        {0.0, 1.0, 0.0} // up
    );

    Material sphere_mat{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {1, 0, 0},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.79,
        .diffuse_coeff = 0.2,
        .specular_coeff = 0.01,
        .specular_exponent = 1.0
    };
    Material r_sphere_mat{
        .index_of_refraction = 1.0,
        .reflect_precent = 1.0,
        .refract_precent = 0.0,
        .ambient_color = {1, 1, 1},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.79,
        .diffuse_coeff = 0.2,
        .specular_coeff = 0.01,
        .specular_exponent = 1.0
    };
    Material ground_mat{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {0.5, 0.5, 0.5},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.5,
        .specular_exponent = 1.0
    };
    Material bg_material{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {1, 1, 1},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.5,
        .specular_exponent = 1.0
    };

    std::vector<std::tuple<Intersectable*, Material*>> os{};
    std::vector<std::tuple<Sphere, Material*>> spheres;
    spheres.push_back({Sphere({-1, 1, 4}, 1), &r_sphere_mat});
    spheres.push_back({Sphere({0, 1, 7}, 1), &r_sphere_mat});
    spheres.push_back({Sphere({0, 0.3, 2}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({0.4, 0.3, 2.5}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({0.2, 0.3, -4}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({-0.5, 0.3, -1}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({0.1, 0.3, 4.2}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({0.7, 0.3, 1.2}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({0.3, 0.3, -6}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({-1, 0.3, 3}, 0.3), &sphere_mat});
    spheres.push_back({Sphere({-1.4, 0.3, -1.8}, 0.3), &sphere_mat});
    for (auto& [sphere, mat_ptr] : spheres) {
        os.push_back({&sphere, mat_ptr});
    }
    Plane ground({0, 0, 0}, {0, 1, 0});
    os.push_back({&ground, &ground_mat});

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{{width, height}, {255, 255, 255, 255}};
    PointLight const light(camera.get_position());

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&bg_material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(os);
            vec3 const vcol =
                root.calculate_color(camera, light, root.sum_light_intensity());
            img.set_color_at(x, y, to_color(vcol));
        }
    }

    img.save("spheres_example.png");
}
