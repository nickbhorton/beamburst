#include "array_ops.h"
#include "camera.h"
#include "image.h"
#include "intersections.h"
#include "line.h"
#include "linear_types.h"
#include "plane.h"
#include "vector_ops.h"

using namespace linalg;

double saturate(double in)
{
    in = std::abs(in);
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
    const vec3& light_position,
    const vec3& position,
    const vec3& view_direction,
    const vec3& normal,
    double specular_hardness
) -> std::tuple<double, double>
{
    const vec3 light_direction = normalize(light_position - position);
    const vec3 halfway = normalize(light_direction + view_direction);
    double specular =
        std::pow(std::max(dot(normal, halfway), 0.0), specular_hardness);
    double diffuse = dot(light_direction, normalize(normal));
    return {diffuse, specular};
}

int main()
{
    typedef std::tuple<std::size_t, std::size_t, vec3, vec3, vec3> pixel_job;

    constexpr double user_distance_to_screen = 1.0;
    constexpr vec3 user_camera_position = {0.0, 0.0, 0.0};
    constexpr vec3 user_camera_up = {0.0, 1.0, 0.0};
    constexpr vec3 user_camera_direction = {1.0, 0.0, 0.0};
    constexpr Screen screen{.discretization = {512, 512}, .size = {1.0, 1.0}};
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
                const vec3 solution_normal =
                    solution_position - sphere.position;
                pixel_jobs.push_back(
                    {x, y, solution_position, solution_normal, view_direction}
                );
            }
        }
    }
    Color bg_color{128, 0, 128, 128};
    Image img1{
        screen.get_horizontal_discretization(),
        screen.get_vertical_discretization()
    };
    img1.fill(bg_color);
    Image img2{
        screen.get_horizontal_discretization(),
        screen.get_vertical_discretization()
    };
    img2.fill(bg_color);
    vec3 light_position = {0.0, 0.0, 0.0};
    vec3 specular_color = {1.0, 0.0, 0.0};
    vec3 diffuse_color = {0.0, 1.0, 0.0};
    [[maybe_unused]] vec3 ambient_color = {0.0, 0.0, 1.0};
    for (const auto& [x, y, position, normal, view] : pixel_jobs) {
        auto const& [diffuse, specular] =
            blin_phong(light_position, position, view, normal, 100.0);
        Color c1 = to_color(specular * specular_color);
        Color c2 = to_color(diffuse * diffuse_color);
        img1.set_color_at(x, y, c1);
        img2.set_color_at(x, y, c2);
    }
    img1.save("test1.png");
    img2.save("test2.png");
}
