#ifndef BEAMBURST_CAMERA_HEADER_
#define BEAMBURST_CAMERA_HEADER_

#include <tuple>

#include "line.h"

struct Screen {
    std::tuple<std::size_t, std::size_t> discretization;
    std::tuple<double, double> size;

    auto get_discretization() const -> std::tuple<std::size_t, std::size_t>
    {
        return discretization;
    }

    auto get_width() const -> double { return std::get<0>(size); }
    auto get_height() const -> double { return std::get<1>(size); }

    auto get_horizontal_discretization() const -> std::size_t
    {
        return std::get<0>(discretization);
    }
    auto get_vertical_discretization() const -> std::size_t
    {
        return std::get<1>(discretization);
    }

    auto get_vertical_delta() const -> double
    {
        return 1.0 / static_cast<double>(std::get<0>(discretization));
    }
    auto get_horizontal_delta() const -> double
    {
        return 1.0 / static_cast<double>(std::get<1>(discretization));
    }
};

class Camera
{
    Screen const& screen;
    double distance_to_screen;
    std::array<double, 3> position;
    std::array<double, 3> front_direction;
    std::array<double, 3> up_direction;
    std::array<double, 3> right_direction;

    // cached values computed once but does not really belong in a class
    std::array<double, 3> screen_up;
    std::array<double, 3> screen_right;
    std::array<double, 3> screen_delta_right;
    std::array<double, 3> screen_delta_down;
    std::array<double, 3> screen_center;
    std::array<double, 3> screen_top_left;

public:
    Camera(
        Screen const& screen,
        double distance_to_screen,
        std::array<double, 3> const& position,
        std::array<double, 3> const& user_front_direction,
        std::array<double, 3> const& user_up_direction
    );

    auto get_position() const -> std::array<double, 3>;
    auto get_line_at(std::size_t x, std::size_t y) const -> Line;
};

auto axis_aligned_camera_px(Screen const& screen, double distance) -> Camera;
auto axis_aligned_camera_nx(Screen const& screen, double distance) -> Camera;
auto axis_aligned_camera_py(Screen const& screen, double distance) -> Camera;
auto axis_aligned_camera_ny(Screen const& screen, double distance) -> Camera;
auto axis_aligned_camera_pz(Screen const& screen, double distance) -> Camera;
auto axis_aligned_camera_nz(Screen const& screen, double distance) -> Camera;

#endif // !BEAMBURST_CAMERA_HEADER_
