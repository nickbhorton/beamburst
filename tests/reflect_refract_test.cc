#include "array_ops.h"
#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "plane.h"
#include "sphere.h"
#include "vector_ops.h"

using namespace linalg;

typedef std::tuple<std::size_t, std::size_t, Line, intersection_t> pixel_job_t;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    constexpr size_t img_size = 1024;
    Screen const screen{
        .discretization = {img_size, img_size},
        .size = {1.0, 1.0}
    };

    Camera const camera(
        screen,
        1,
        {6, 2, 0},     // pos
        {-1, -0.2, 0}, // view
        {0, 1, 0}      // up
    );
    std::vector<Sphere> spheres{};
    spheres.push_back(Sphere({0, 0, 1}, 1));
    spheres.push_back(Sphere({0, 0, -1}, 1));
    spheres.push_back(Sphere({-1, 2, 1}, 1));
    spheres.push_back(Sphere({-1, 2, -1}, 1));
    spheres.push_back(Sphere({-2, 1, 1}, 1));
    spheres.push_back(Sphere({-2, 1, -1}, 1));
    BVHNode root_bvh{};
    for (auto const& sphere : spheres) {
        root_bvh.add_primitive(&sphere);
    }
    root_bvh.construct_tree();

    Plane ground({0, -1, 0}, {0, 1, 0});
    std::vector<Intersectable*> is{};
    is.push_back(&root_bvh);
    is.push_back(&ground);

    std::vector<pixel_job_t> pixel_jobs{};

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            Line const l = camera.get_line_at(x, y);
            auto intersection = intersect(is, l);

            if (intersection.has_value()) {
                pixel_jobs.push_back({x, y, l, intersection.value()});
            }
        }
    }

    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        Color(255, 255, 255, 255)
    };

    PointLight const light({10, 0, 0});
    for (const auto& [x, y, line, intersection] : pixel_jobs) {
        auto const& [t, normal, uv_opt, iptr] = intersection;
        vec3 const position = solve_line(line, t);
        vec3 const view = camera.get_position() - position;
        double const diffuse = phong_diffuse(light.position, position, normal);
        double const specular =
            blin_phong_specular(light.position, position, view, normal, 100.0);

        vec3 constexpr ambient_color = {1.0, 1.0, 1.0};
        double constexpr ambient_power = 0.1;
        double constexpr diffuse_power = 0.5;
        double constexpr specular_power = 0.4;

        Color const c1 = to_color(
            specular_power * specular * color::white +
            diffuse_power * diffuse * color::white +
            ambient_power * ambient_color
        );
        img.set_color_at(x, y, c1);
    }
    img.save("rr.png");
}
