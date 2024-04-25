#include <algorithm>
#include <limits>

#include "array_ops.h"
#include "camera.h"
#include "image.h"
#include "intersections.h"
#include "line.h"
#include "linear_types.h"
#include "plane.h"
#include "vector_ops.h"

using namespace linalg;

int main()
{
    constexpr Plane p{.position = {0.0, -1.0, 0.0}, .normal = {0.0, 1.0, 0.0}};
    constexpr double user_distance_to_screen = 4.0;
    constexpr vec3 user_camera_position = {0.0, 0.0, 0.0};
    constexpr vec3 user_camera_up = {0.0, 1.0, 0.1};
    constexpr vec3 user_camera_direction = {1.0, 0.0, 0.0};
    constexpr Screen screen{.discretization = {1024, 1024}, .size = {1.0, 1.0}};
    const Camera camera{
        screen,
        user_distance_to_screen,
        user_camera_position,
        user_camera_direction,
        user_camera_up
    };

    std::vector<double> sol_mag{};
    for (size_t y = 0; y < screen.get_vertical_discretization(); y++) {
        for (size_t x = 0; x < screen.get_horizontal_discretization(); x++) {
            Line line = camera.get_line_at(x, y);
            const auto t_opt = find_intersection(line, p);
            if (t_opt.has_value() && !std::signbit(t_opt.value())) {
                const vec3 sol_position =
                    camera.get_position() + t_opt.value() * line.direction;
                const double sol_position_distance = magnitude(sol_position);
                sol_mag.push_back(sol_position_distance);
            } else {
                sol_mag.push_back(std::numeric_limits<double>::quiet_NaN());
            }
        }
    }
    Image img1{
        screen.get_horizontal_discretization(),
        screen.get_vertical_discretization()
    };
    double distance_max = std::numeric_limits<double>::quiet_NaN();
    double distance_min = std::numeric_limits<double>::quiet_NaN();
    std::ranges::for_each(sol_mag, [&distance_max](double e) {
        if (std::isnan(e)) {
            return;
        } else if (std::isnan(distance_max)) {
            distance_max = e;
        }
        if (e > distance_max) {
            distance_max = e;
        }
    });
    std::ranges::for_each(sol_mag, [&distance_min](double e) {
        if (std::isnan(e)) {
            return;
        } else if (std::isnan(distance_min)) {
            distance_min = e;
        }
        if (e < distance_min) {
            distance_min = e;
        }
    });
    const double distance_range = distance_max - distance_min;
    for (size_t y = 0; y < img1.get_height(); y++) {
        for (size_t x = 0; x < img1.get_width(); x++) {
            const double val = sol_mag[y * img1.get_width() + x];
            if (val == std::numeric_limits<double>::quiet_NaN()) {
                img1.set_color_at(x, y, Color{0, 0, 0, 255});
            } else {
                double unity = (val - distance_min) / distance_range;
                uint8_t color_unity = std::floor((1.0 - unity) * 255.0);
                img1.set_color_at(
                    x,
                    y,
                    Color{color_unity, color_unity, color_unity, 255}
                );
            }
        }
    }
    img1.save("test.png");
}
