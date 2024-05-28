#include <fstream>
#include <iostream>

#include "array_ops.h"
#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "material.h"
#include "parser.h"
#include "path_tree.h"
#include "plane.h"
#include "sphere.h"
#include "vector_ops.h"

using namespace linalg;

typedef std::tuple<double, Line, intersection_t> ray_path_segment_t;
typedef std::vector<std::tuple<double, Line, intersection_t>> ray_path_t;
typedef std::tuple<std::size_t, std::size_t, LightGraphNode> pixel_job_t;

int main()
{
    constexpr size_t img_size = 512;
    Screen const screen{
        .discretization = {img_size, img_size},
        .size = {1.0, 1.0}
    };

    Camera const camera(
        screen,
        1,
        {15, 2, 15},    // pos
        {-1, -0.1, -1}, // view
        {0, 1, 0}       // up
    );

    Material cow_material{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 1.0,
        .ambient_color = {0, 0, 1},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.5,
        .specular_exponent = 100.0
    };

    Material ground_material{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {1, 1, 1},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.5,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.2,
        .specular_exponent = 100.0
    };

    std::vector<std::tuple<Intersectable*, Material*>> os{};

    Plane ground({0, -4, 0}, {0, 1, 0});
    os.push_back({&ground, &ground_material});

    std::vector<Sphere> spheres{};
    spheres.push_back(Sphere({-17, 5, -3}, 10));
    spheres.push_back(Sphere({-3, 5, -17}, 10));
    for (auto& sphere : spheres) {
        os.push_back({&sphere, &ground_material});
    }

    std::ifstream cow_file("../resources/objects/cow.obj", std::ifstream::in);
    VertexObject const cow_vo(cow_file);
    BVHNode cow_bvh{};
    std::vector<Triangle> const triangles = cow_vo.extract_triangles();
    for (auto const& triangle : triangles) {
        cow_bvh.add_primitive(&triangle);
    }
    cow_bvh.construct_tree();
    os.push_back({&cow_bvh, &cow_material});

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        Color(0, 0, 0, 0)
    };
    PointLight const light(
        camera.get_position() + std::array<double, 3>({0, 10, 0})
    );

    for (size_t y = 0; y < height; y++) {
        std::cout << y << " ";
        std::flush(std::cout);
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&cow_material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(os, &ground_material);
            double const total_intensity = root.sum_light_intensity();
            vec3 const vcol =
                root.calculate_color(camera, light, total_intensity);
            img.set_color_at(x, y, to_color(vcol));
        }
    }

    img.save("cow_rr.png");
}
