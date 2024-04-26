#include "array_ops.h"
#include "camera.h"
#include "image.h"
#include "intersections.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "sphere.h"
#include "vector_ops.h"

using namespace linalg;

int main()
{
    typedef std::tuple<std::size_t, std::size_t, vec3, vec3, vec3> pixel_job;

    constexpr double user_distance_to_screen = 1.0;
    constexpr vec3 user_camera_position = {0.0, 0.0, 0.0};
    constexpr vec3 user_camera_up = {0.0, 1.0, 0.0};
    constexpr vec3 user_camera_direction = {1.0, 0.0, 0.0};
    constexpr Screen screen{.discretization = {1024, 1024}, .size = {1.0, 1.0}};
    const Camera camera{
        screen,
        user_distance_to_screen,
        user_camera_position,
        user_camera_direction,
        user_camera_up
    };

    constexpr Sphere sphere{.position = {4.0, 0.0, 0.0}, .radius = 1.0};
    std::vector<pixel_job> pixel_jobs{};
    for (size_t y = 0; y < screen.get_vertical_discretization(); y++) {
        for (size_t x = 0; x < screen.get_horizontal_discretization(); x++) {
            Line line = camera.get_line_at(x, y);
            const auto t_opt = find_intersection(line, sphere);
            if (t_opt.has_value()) {
                const vec3 solution_position =
                    camera.get_position() + t_opt.value() * line.direction;
                const vec3 view_direction =
                    camera.get_position() - solution_position;
                vec3 solution_normal =
                    solution_position - sphere.position;
                pixel_jobs.push_back(
                    {x, y, solution_position, solution_normal, view_direction}
                );
            }
        }
    }
    Color bg_color{0, 0, 0, 128};
    Image img1{screen.get_discretization(), bg_color};

    vec3 light_position = {0.0, 0.0, 0.0};
    vec3 specular_color = {1.0, 1.0, 1.0};
    double specular_power = 0.5;
    vec3 diffuse_color = {1.0, 1.0, 1.0};
    double diffuse_power = 0.4;
    vec3 ambient_color = {1.0, 1.0, 1.0};
    double ambient_power = 0.1;
    for (const auto& [x, y, position, normal, view] : pixel_jobs) {
        auto const& [diffuse, specular] =
            blin_phong(light_position, position, view, normal, 100.0);
        Color c1 = to_color(
            specular_power * specular * specular_color +
            diffuse_power * diffuse * diffuse_color +
            ambient_power * ambient_color
        );
        img1.set_color_at(x, y, c1);
    }
    img1.save("test1.png");
}
