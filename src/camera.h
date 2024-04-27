#ifndef BEAMBURST_CAMERA_HEADER_
#define BEAMBURST_CAMERA_HEADER_

#include <array>
#include <tuple>

#include "line.h"

struct Screen {
    std::tuple<std::size_t, std::size_t> discretization;
    std::tuple<double, double> size;
    const std::tuple<std::size_t, std::size_t> get_discretization() const
    {
        return discretization;
    }

    constexpr double get_width() const { return std::get<0>(size); }

    constexpr double get_height() const { return std::get<1>(size); }

    constexpr std::size_t get_horizontal_discretization() const
    {
        return std::get<0>(discretization);
    }

    constexpr std::size_t get_vertical_discretization() const
    {
        return std::get<1>(discretization);
    }

    constexpr double get_vertical_delta() const
    {
        return 1.0 / static_cast<double>(std::get<0>(discretization));
    }

    constexpr double get_horizontal_delta() const
    {
        return 1.0 / static_cast<double>(std::get<1>(discretization));
    }
};

class Camera
{
    const Screen& screen;
    double distance_to_screen;
    std::array<double, 3> position;
    std::array<double, 3> front_direction;
    std::array<double, 3> up_direction;
    std::array<double, 3> right_direction;
    // this is very messy
    std::array<double, 3> screen_up;
    std::array<double, 3> screen_right;
    std::array<double, 3> screen_delta_right;
    std::array<double, 3> screen_delta_down;
    std::array<double, 3> screen_center;
    std::array<double, 3> screen_top_left;

public:
    Camera(
        const Screen& screen,
        double distance_to_screen,
        const std::array<double, 3>& position,
        const std::array<double, 3>& user_front_direction,
        const std::array<double, 3>& user_up_direction
    );

    std::array<double, 3> get_position() const;
    Line get_line_at(std::size_t x, std::size_t y) const;
};

auto axis_aligned_camera_px(const Screen& screen, double distance) -> Camera;
auto axis_aligned_camera_nx(const Screen& screen, double distance) -> Camera;
auto axis_aligned_camera_py(const Screen& screen, double distance) -> Camera;
auto axis_aligned_camera_ny(const Screen& screen, double distance) -> Camera;
auto axis_aligned_camera_pz(const Screen& screen, double distance) -> Camera;
auto axis_aligned_camera_nz(const Screen& screen, double distance) -> Camera;

#endif // !BEAMBURST_CAMERA_HEADER_
