#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "coordinates.h"
#include "image.h"
#include "intersections.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "sphere.h"
#include "vector_ops.h"
#include <cmath>

using namespace linalg;

int main()
{
    typedef std::tuple<std::size_t, std::size_t, vec3, vec3, vec3> pixel_job;

    constexpr double user_distance_to_screen = 1.0;
    constexpr vec3 user_camera_position1 = {0.0, 0.0, 0.0};
    constexpr vec3 user_camera_up1 = {0.0, 0.0, 1.0};
    constexpr vec3 user_camera_direction1 = {0.0, -1.0, 0.0};
    constexpr vec3 user_camera_position2 = {0.0, -8.0, 0.0};
    constexpr vec3 user_camera_up2 = {0.0, 0.0, -1.0};
    constexpr vec3 user_camera_direction2 = {0.0, 1.0, 0.0};
    constexpr Screen screen{.discretization = {1024, 1024}, .size = {1.0, 1.0}};
    const Camera camera1{
        screen,
        user_distance_to_screen,
        user_camera_position1,
        user_camera_direction1,
        user_camera_up1
    };
    const Camera camera2{
        screen,
        user_distance_to_screen,
        user_camera_position2,
        user_camera_direction2,
        user_camera_up2
    };

    constexpr Sphere sphere{.position = {0.0, -4.0, 0.0}, .radius = 1.0};
    std::vector<pixel_job> pixel_jobs1{};
    std::vector<pixel_job> pixel_jobs2{};
    for (size_t y = 0; y < screen.get_vertical_discretization(); y++) {
        for (size_t x = 0; x < screen.get_horizontal_discretization(); x++) {
            {
                Line line = camera1.get_line_at(x, y);
                const auto t_opt = find_intersection(line, sphere);
                if (t_opt.has_value()) {
                    const vec3 solution_position =
                        camera1.get_position() + t_opt.value() * line.direction;
                    const vec3 view_direction =
                        camera1.get_position() - solution_position;
                    vec3 solution_normal = solution_position - sphere.position;
                    pixel_jobs1.push_back(
                        {x,
                         y,
                         solution_position,
                         solution_normal,
                         view_direction}
                    );
                }
            }
            {
                Line line = camera2.get_line_at(x, y);
                const auto t_opt = find_intersection(line, sphere);
                if (t_opt.has_value()) {
                    const vec3 solution_position =
                        camera2.get_position() + t_opt.value() * line.direction;
                    const vec3 view_direction =
                        camera2.get_position() - solution_position;
                    vec3 solution_normal = solution_position - sphere.position;
                    pixel_jobs2.push_back(
                        {x,
                         y,
                         solution_position,
                         solution_normal,
                         view_direction}
                    );
                }
            }
        }
    }
    Color bg_color{0, 0, 0, 64};
    Image img1{screen.get_discretization(), bg_color};
    Image img2{screen.get_discretization(), bg_color};

    vec3 light_position = {0.0, 0.0, 0.0};
    vec3 specular_color = {1.0, 1.0, 1.0};
    double specular_power = 0.1;
    vec3 diffuse_color = {1.0, 1.0, 1.0};
    double diffuse_power = 0.1;
    double ambient_power = 0.8;
    for (const auto& [x, y, position, normal, view] : pixel_jobs1) {
        auto const& [diffuse, specular] =
            blin_phong(light_position, position, view, normal, 100.0);
        const vec3 spherical_normal = cartesian_to_spherical(normal);
        double phi = (spherical_normal[2] + M_PI) / (2.0 * M_PI);
        double theta = spherical_normal[1] / M_PI;
        vec3 ambient_color = {saturate(phi), saturate(theta), 0.0};
        Color c1 = to_color(
            specular_power * specular * specular_color +
            diffuse_power * diffuse * diffuse_color +
            ambient_power * ambient_color
        );
        img1.set_color_at(x, y, c1);
    }
    img1.save("top.png");
    for (const auto& [x, y, position, normal, view] : pixel_jobs2) {
        auto const& [diffuse, specular] =
            blin_phong(light_position, position, view, normal, 100.0);
        const vec3 spherical_normal = cartesian_to_spherical(normal);
        double phi = (spherical_normal[2] + M_PI) / (2.0 * M_PI);
        double theta = spherical_normal[1] / M_PI;
        vec3 ambient_color = {saturate(phi), saturate(theta), 0.0};
        Color c1 = to_color(
            specular_power * specular * specular_color +
            diffuse_power * diffuse * diffuse_color +
            ambient_power * ambient_color
        );
        img2.set_color_at(x, y, c1);
    }
    img2.save("bot.png");
}
