#include "camera.h"
#include "array_ops.h"
#include "transformations.h"

using namespace transformations;
using namespace linalg;

Camera::Camera(
    const Screen& screen,
    double distance_to_screen,
    const std::array<double, 3>& position,
    const std::array<double, 3>& user_front_direction,
    const std::array<double, 3>& user_up_direction
)
    : screen(screen), distance_to_screen(distance_to_screen),
      position(position), front_direction(normalize(user_front_direction)),
      up_direction(normalize(user_up_direction))
{
    right_direction = normalize(cross(front_direction, up_direction));
    screen_up =
        transform(scaling_matrix(0.5 * screen.get_height()), up_direction);
    screen_right =
        transform(scaling_matrix(0.5 * screen.get_width()), right_direction);
    screen_delta_right = (2.0 * screen_right) * screen.get_horizontal_delta();
    screen_delta_down = (-2.0 * screen_up) * screen.get_vertical_delta();
    screen_center =
        position +
        transform(scaling_matrix(distance_to_screen), (front_direction));
    screen_top_left = screen_center + screen_up - screen_right;
}

std::array<double, 3> Camera::get_position() const { return position; }
Line Camera::get_line_at(std::size_t x, std::size_t y) const
{
    const std::array<double, 3> line_direction =
        (screen_top_left + (screen_delta_right * static_cast<double>(x)) +
         (screen_delta_down * static_cast<double>(y))) -
        position;
    Line l{.position = position, .direction = normalize(line_direction)};
    return l;
}

auto axis_aligned_camera_px(const Screen& screen, double distance) -> Camera
{
    const vec3 camera_position_px = {-distance, 0.0, 0.0};
    const vec3 camera_up_px = {0.0, 1.0, 0.0};
    const vec3 camera_direction_px = {1.0, 0.0, 0.0};
    return Camera{
        screen,
        (std::get<0>(screen.size) + std::get<1>(screen.size)) / 2.0,
        camera_position_px,
        camera_direction_px,
        camera_up_px
    };
}

auto axis_aligned_camera_nx(const Screen& screen, double distance) -> Camera
{
    const vec3 camera_position_nx = {distance, 0.0, 0.0};
    const vec3 camera_up_nx = {0.0, 1.0, 0.0};
    const vec3 camera_direction_nx = {-1.0, 0.0, 0.0};
    return Camera{
        screen,
        (std::get<0>(screen.size) + std::get<1>(screen.size)) / 2.0,
        camera_position_nx,
        camera_direction_nx,
        camera_up_nx
    };
}

auto axis_aligned_camera_py(const Screen& screen, double distance) -> Camera
{
    const vec3 camera_position_py = {0.0, -distance, 0.0};
    const vec3 camera_up_py = {1.0, 0.0, 0.0};
    const vec3 camera_direction_py = {0.0, 1.0, 0.0};
    return Camera{
        screen,
        (std::get<0>(screen.size) + std::get<1>(screen.size)) / 2.0,
        camera_position_py,
        camera_direction_py,
        camera_up_py
    };
}
auto axis_aligned_camera_ny(const Screen& screen, double distance) -> Camera
{
    const vec3 camera_position_ny = {0.0, distance, 0.0};
    const vec3 camera_up_ny = {1.0, 0.0, 0.0};
    const vec3 camera_direction_ny = {0.0, -1.0, 0.0};
    return Camera{
        screen,
        (std::get<0>(screen.size) + std::get<1>(screen.size)) / 2.0,
        camera_position_ny,
        camera_direction_ny,
        camera_up_ny
    };
}
auto axis_aligned_camera_pz(const Screen& screen, double distance) -> Camera
{
    const vec3 camera_position_pz = {0.0, 0.0, -distance};
    const vec3 camera_up_pz = {0.0, 1.0, 0.0};
    const vec3 camera_direction_pz = {0.0, 0.0, 1.0};
    return Camera{
        screen,
        (std::get<0>(screen.size) + std::get<1>(screen.size)) / 2.0,
        camera_position_pz,
        camera_direction_pz,
        camera_up_pz
    };
}
auto axis_aligned_camera_nz(const Screen& screen, double distance) -> Camera
{
    const vec3 camera_position_nz = {0.0, 0.0, distance};
    const vec3 camera_up_nz = {0.0, 1.0, 0.0};
    const vec3 camera_direction_nz = {0.0, 0.0, -1.0};
    return Camera{
        screen,
        (std::get<0>(screen.size) + std::get<1>(screen.size)) / 2.0,
        camera_position_nz,
        camera_direction_nz,
        camera_up_nz
    };
}
