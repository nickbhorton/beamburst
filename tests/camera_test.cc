#include "array_ops.h"
#include "image.h"
#include "intersections.h"
#include "line.h"
#include "linear_types.h"
#include "plane.h"
#include "transformations.h"
#include <algorithm>
#include <limits>

using namespace linalg;
using namespace transformations;

int main()
{
    constexpr size_t img_width = 500;
    constexpr size_t img_height = 500;
    const double screen_width_samples = static_cast<double>(img_width);
    const double screen_height_samples = static_cast<double>(img_height);
    const double camera_screen_width = 1.0;
    const double camera_screen_height = 1.0;
    const double camera_distance_to_screen = 4.0;
    const vec4 camera_position = {0.0, 0.0, 0.0, 1.0};
    const vec3 camera_position3 =
        {camera_position[0], camera_position[1], camera_position[2]};
    const vec4 camera_up = {0.0, 1.0, 0.0, 1.0};
    const vec4 camera_right = {0.0, 0.0, 1.0, 1.0};
    const vec4 camera_direction = {1.0, 0.0, 0.0, 1.0};
    const vec4 screen_up =
        scaling_matrix(0.5 * camera_screen_height) * (camera_up);
    const vec4 screen_right =
        scaling_matrix(0.5 * camera_screen_width) * (camera_right);
    const vec4 screen_center =
        camera_position +
        scaling_matrix(camera_distance_to_screen) * (camera_direction);
    const vec4 screen_top_left = screen_center + screen_up - screen_right;
    const vec4 screen_delta_right =
        (2.0 * screen_right) * (1.0 / screen_width_samples);
    const vec4 screen_delta_down =
        (-2.0 * screen_up) * (1.0 / screen_height_samples);

    const Plane p{.position = {1.0, 0.0, 0.0}, .normal = {-1.0, 0.0, 0.0}};
    std::array<double, img_height * img_width> sol_mag{};
    for (size_t y = 0; y < img_height; y++) {
        for (size_t x = 0; x < img_width; x++) {
            const vec4 line_direction =
                (screen_top_left +
                 (screen_delta_right * static_cast<double>(x)) +
                 (screen_delta_down * static_cast<double>(y))) -
                camera_position;
            const vec3 line_direction3 =
                {line_direction[0], line_direction[1], line_direction[2]};
            const Line l{
                .position = camera_position3,
                .direction = line_direction3
            };
            const std::optional<double> t = find_intersection(l, p);
            if (t.has_value() && !std::signbit(t.value())) {
                const double positive_normal_t = t.value();
                const vec3 sol_position =
                    camera_position3 + positive_normal_t * l.direction;
                const double sol_position_distance = magnitude(sol_position);
                sol_mag[y * img_width + x] = sol_position_distance;
            } else {
                sol_mag[y * img_width + x] =
                    std::numeric_limits<double>::quiet_NaN();
            }
        }
    }
    Image img1{img_width, img_height};
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
    std::cout << distance_max << "\n";
    std::cout << distance_min << "\n";
    for (size_t y = 0; y < img_height; y++) {
        for (size_t x = 0; x < img_width; x++) {
            const double val = sol_mag[y * img_width + x];
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
