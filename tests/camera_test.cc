#include "array_ops.h"
#include "image.h"
#include "intersections.h"
#include "line.h"
#include "linear_types.h"
#include "plane.h"
#include "transformations.h"
#include "vector_ops.h"
#include <algorithm>
#include <limits>

using namespace linalg;
using namespace transformations;

int main()
{
    constexpr Plane p{.position = {0.0, -1.0, 0.0}, .normal = {0.0, 1.0, 0.0}};
    constexpr std::tuple<size_t, size_t> image_size = {1024, 1024};
    constexpr std::tuple<double, double> camera_screen_size = {1.0, 1.0};
    constexpr double camera_distance_to_screen = 4.0;
    constexpr vec3 camera_position = {0.0, 0.0, 0.0};
    constexpr vec3 user_camera_up = {0.0, 1.0, 0.1};
    constexpr vec3 user_camera_direction = {1.0, 0.0, 0.0};

    const vec3 camera_direction = normalize(user_camera_direction);
    const vec3 camera_up = normalize(user_camera_up);
    const vec3 camera_right = normalize(cross(camera_direction, camera_up));
    const double camera_screen_width = std::get<0>(camera_screen_size);
    const double camera_screen_height = std::get<1>(camera_screen_size);
    const vec3 screen_up =
        transform(scaling_matrix(0.5 * camera_screen_height), camera_up);
    const vec3 screen_right =
        transform(scaling_matrix(0.5 * camera_screen_width), camera_right);
    const size_t img_width = std::get<0>(image_size);
    const size_t img_height = std::get<1>(image_size);
    const double screen_horizontal_samples = static_cast<double>(img_width);
    const double screen_vertical_samples = static_cast<double>(img_height);
    const vec3 screen_delta_right =
        (2.0 * screen_right) * (1.0 / screen_horizontal_samples);
    const vec3 screen_delta_down =
        (-2.0 * screen_up) * (1.0 / screen_vertical_samples);
    const vec3 screen_center =
        camera_position + transform(
                              scaling_matrix(camera_distance_to_screen),
                              (camera_direction)
                          );
    const vec3 screen_top_left = screen_center + screen_up - screen_right;
    std::vector<double> sol_mag{};
    for (size_t y = 0; y < img_height; y++) {
        for (size_t x = 0; x < img_width; x++) {
            const vec3 line_direction =
                (screen_top_left +
                 (screen_delta_right * static_cast<double>(x)) +
                 (screen_delta_down * static_cast<double>(y))) -
                camera_position;
            const vec3 line_direction3 =
                {line_direction[0], line_direction[1], line_direction[2]};
            const Line l{
                .position = camera_position,
                .direction = line_direction3
            };
            const std::optional<double> t = find_intersection(l, p);
            if (t.has_value() && !std::signbit(t.value())) {
                const double positive_normal_t = t.value();
                const vec3 sol_position =
                    camera_position + positive_normal_t * l.direction;
                const double sol_position_distance = magnitude(sol_position);
                sol_mag.push_back(sol_position_distance);
            } else {
                sol_mag.push_back(std::numeric_limits<double>::quiet_NaN());
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
