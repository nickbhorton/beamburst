#include "array_ops.h"
#include "camera.h"
#include "image.h"
#include "intersections.h"
#include "line.h"
#include "linear_types.h"
#include "plane.h"
#include "vector_ops.h"

using namespace linalg;

struct PointLight {
    vec3 position;
    vec3 diffuse_color;
    double diffuse_power;
    vec3 specular_color;
    double specular_power;
};

double saturate(double in)
{
    if (in > 1.0) {
        return 1.0;
    } else if (in < 0.0) {
        return 0.0;
    }
    return in;
}

auto to_color(const vec3& vec_color) -> Color
{
    uint8_t r = std::floor(saturate(vec_color[0]) * 255.0);
    uint8_t g = std::floor(saturate(vec_color[1]) * 255.0);
    uint8_t b = std::floor(saturate(vec_color[2]) * 255.0);
    return Color{r, g, b, 255};
}

auto blin_phong(
    const PointLight& light,
    const vec3& position,
    const vec3& view_direction,
    const vec3& normal
) -> std::tuple<vec3, vec3>
{
    vec3 light_direction = light.position - position;
    double distance = magnitude(light_direction);
    light_direction = light_direction * (1.0 / distance);
    distance = distance * distance;
    double n_dot_l = dot(normal, light_direction);
    double intensity = saturate(n_dot_l);
    vec3 diffuse = intensity * light.diffuse_color * light.diffuse_power *
                   (1.0 / distance);
    vec3 h = normalize(light_direction + view_direction);
    double n_dot_h = dot(normal, h);
    constexpr double specular_hardness = 10.0;
    intensity = std::pow(saturate(n_dot_h), specular_hardness);
    vec3 specular = intensity * light.specular_color * light.specular_power *
                    (1.0 / distance);
    return {diffuse, specular};
}

int main()
{
    typedef std::tuple<std::size_t, std::size_t, vec3, vec3> pixel_job;

    constexpr double user_distance_to_screen = 4.0;
    constexpr vec3 user_camera_position = {0.0, 0.0, 0.0};
    constexpr vec3 user_camera_up = {0.0, 1.0, 0.0};
    constexpr vec3 user_camera_direction = {1.0, 0.0, 0.0};
    constexpr Screen screen{.discretization = {1000, 1000}, .size = {1.0, 1.0}};
    const Camera camera{
        screen,
        user_distance_to_screen,
        user_camera_position,
        user_camera_direction,
        user_camera_up
    };

    constexpr Plane p{.position = {0.0, -0.1, 0.0}, .normal = {0.0, 1.0, 0.0}};
    std::vector<pixel_job> pixel_jobs{};
    for (size_t y = 0; y < screen.get_vertical_discretization(); y++) {
        for (size_t x = 0; x < screen.get_horizontal_discretization(); x++) {
            Line line = camera.get_line_at(x, y);
            const auto t_opt = find_intersection(line, p);
            if (t_opt.has_value()) {
                pixel_jobs.push_back(
                    {x,
                     y,
                     camera.get_position() + t_opt.value() * line.direction,
                     line.direction}
                );
            }
        }
    }
    Image img1{
        screen.get_horizontal_discretization(),
        screen.get_vertical_discretization()
    };
    for (size_t i = 0; i < screen.get_vertical_discretization(); i++) {
    for (size_t j = 0; j < screen.get_horizontal_discretization(); j++) {
            img1.set_color_at(j, i, Color{255, 255, 255, 255});
    }
    }
    PointLight light{
        .position = {3.0, 1.0, 0.0},
        .diffuse_color = {1.0, 0.0, 0.0},
        .diffuse_power = 1.0,
        .specular_color = {0.0, 0.0, 1.0},
        .specular_power = 1.0
    };
    for (const auto& [x, y, sol_pos, view_dir] : pixel_jobs) {
        auto const& [diffuse, specular] =
            blin_phong(light, sol_pos, view_dir, p.normal);
        // std::cout << diffuse << "\n";
        // std::cout << specular << "\n\n";
        Color c = to_color(diffuse + specular);
        img1.set_color_at(x, y, c);
    }
    img1.save("test.png");
}
