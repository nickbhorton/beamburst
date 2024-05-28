#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "material.h"
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
    constexpr size_t img_size = 2048;
    Screen const screen{
        .discretization = {img_size, img_size},
        .size = {1.0, 1.0}
    };

    Camera const camera(
        screen,
        1,
        {5, 1.75, 0},  // pos
        {-1, -0.2, 0}, // view
        {0, 1, 0}      // up
    );

    Material material{
        .index_of_refraction = 1.5,
        .reflect_precent = 0.5,
        .refract_precent = 0.5,
        .ambient_color = {0, 1, 0},
        .diffuse_color = {1, 0, 0},
        .specular_color = {0, 0, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 1.0,
        .specular_coeff = 1.0,
        .specular_exponent = 100.0
    };

    std::vector<Sphere> spheres{};
    spheres.push_back(Sphere({0, 0, 1}, 1));
    spheres.push_back(Sphere({0, 0, -1}, 1));
    spheres.push_back(Sphere({-1, 2, 1}, 1));
    spheres.push_back(Sphere({-1, 2, -1}, 1));
    spheres.push_back(Sphere({-2, 1, 1}, 1));
    spheres.push_back(Sphere({-2, 1, -1}, 1));
    Plane ground({0, -1, 0}, {0, 1, 0});

    std::vector<Intersectable*> is{};
    BVHNode bvh{};
    is.push_back(&ground);
    for (auto& sphere : spheres) {
        bvh.add_primitive(&sphere);
    }
    bvh.construct_tree();
    is.push_back(&bvh);

    std::vector<pixel_job_t> pixel_jobs{};

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct(is);
            pixel_jobs.push_back({x, y, std::move(root)});
        }
    }

    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        Color(0, 0, 0, 0)
    };

    PointLight const light({-1, 1.5, 0});
    for (const auto& [x, y, light_graph] : pixel_jobs) {
        // we walkin the tree twice
        double const total_intensity = light_graph.sum_light_intensity();
        vec3 vcol = light_graph.calculate_color(camera, light, total_intensity);
        img.set_color_at(x, y, to_color(vcol));
    }
    img.save("rr.png");
}
