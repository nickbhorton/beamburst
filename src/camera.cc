#include "camera.h"
#include "array_ops.h"
#include "transformations.h"

using namespace transformations;

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
    Line l{.position = position, .direction = line_direction};
    return l;
}
