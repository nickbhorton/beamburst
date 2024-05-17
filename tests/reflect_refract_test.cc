#include <stack>

#include "array_ops.h"
#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "path_tree.h"
#include "plane.h"
#include "sphere.h"
#include "vector_ops.h"

using namespace linalg;

typedef std::tuple<double, Line, intersection_t> ray_path_segment_t;
typedef std::vector<std::tuple<double, Line, intersection_t>> ray_path_t;
typedef std::tuple<std::size_t, std::size_t, ray_path_t> pixel_job_t;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
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
    std::vector<Sphere> spheres{};
    spheres.push_back(Sphere({0, 0, 1}, 1));
    spheres.push_back(Sphere({0, 0, -1}, 1));
    spheres.push_back(Sphere({-1, 2, 1}, 1));
    spheres.push_back(Sphere({-1, 2, -1}, 1));
    spheres.push_back(Sphere({-2, 1, 1}, 1));
    spheres.push_back(Sphere({-2, 1, -1}, 1));
    Plane ground({0, -1, 0}, {0, 1, 0});

    std::vector<Intersectable*> is{};
    is.push_back(&ground);
    for (auto& sphere : spheres) {
        is.push_back(&sphere);
    }

    std::vector<pixel_job_t> pixel_jobs{};

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    double const light_reflected = 0.5;
    Material water_material{.index_of_refraction = 1.5};

    size_t constexpr max_tree_depth = 10;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode root{water_material, 0, 1, camera.get_line_at(x, y)};
            ray_path_t path{};
            Line const cam_line = camera.get_line_at(x, y);
            std::stack<std::tuple<size_t, Line, Intersectable const*>> s{};
            s.push({0, cam_line, nullptr});
            while (!s.empty()) {
                auto const& [depth, line, remove_ptr] = s.top();
                s.pop();
                auto const intersection = intersect(is, line, remove_ptr);

                if (intersection.has_value() && depth < max_tree_depth) {
                    ray_path_segment_t new_segment = {
                        std::pow(light_reflected, depth),
                        line,
                        intersection.value()
                    };
                    path.push_back(new_segment);
                    Line reflected_line = Line(
                        solve_line(line, std::get<0>(intersection.value())),
                        reflected_direction(
                            line.direction,
                            std::get<1>(intersection.value())
                        )
                    );
                    s.push(
                        {depth + 1,
                         reflected_line,
                         std::get<3>(intersection.value())}
                    );
                }
            }
            pixel_jobs.push_back({x, y, path});
        }
    }

    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        Color(0, 0, 0, 0)
    };

    PointLight const light({-1, 1.5, 0});
    for (const auto& [x, y, path] : pixel_jobs) {
        vec3 vcol{0, 0, 0};
        for (auto const& seg : path) {
            auto const& [intensity, line, intersection] = seg;
            auto const& [t, normal, uv_opt, iptr] = intersection;
            vec3 const position = solve_line(line, t);
            vec3 const view = camera.get_position() - position;
            double const diffuse =
                phong_diffuse(light.position, position, normal);
            double const specular = blin_phong_specular(
                light.position,
                position,
                view,
                normal,
                100.0
            );

            double in_shadow = 1.0;
            double const distance_to_light =
                magnitude(light.position - position);

            auto const in_shadow_i = intersect(
                is,
                Line(position, normalize(light.position - position)),
                std::get<3>(intersection)
            );
            if (in_shadow_i.has_value() &&
                std::get<0>(in_shadow_i.value()) > distance_to_light) {
                in_shadow = 0.0;
            }

            vec3 constexpr ambient_color = {0.0, 0.0, 1.0};
            double constexpr ambient_power = 0.1;
            double constexpr diffuse_power = 0.5;
            double constexpr specular_power = 0.4;

            vcol = vcol +
                   intensity *
                       (in_shadow * specular_power * specular * color::white +
                        in_shadow * diffuse_power * diffuse * color::white +
                        ambient_power * ambient_color);
        }
        img.set_color_at(x, y, to_color(vcol));
    }
    img.save("rr.png");
}
