#include <cmath>

#include "array_ops.h"
#include "cmath_extention.h"
#include "color.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r{r}, g{g}, b{b}, a{a}
{
}

auto to_color(std::array<double, 3> const& vec_color) -> Color
{
    uint8_t const r = std::floor(saturate(vec_color[0]) * 255.0);
    uint8_t const g = std::floor(saturate(vec_color[1]) * 255.0);
    uint8_t const b = std::floor(saturate(vec_color[2]) * 255.0);
    return Color{r, g, b, 255};
}
auto from_color(Color const& color) -> std::array<double, 3>
{
    return {color.r / 255.0, color.g / 255.0, color.b / 255.0};
}

auto to_tangent_space(std::array<double, 3> const& v) -> std::array<double, 3>
{
    auto const nv = normalize(v);
    std::array<double, 3> const result{
        0.5 * nv[0] + 0.5,
        0.5 * nv[1] + 0.5,
        1.0 - (0.5 * nv[2] - 0.5)
    };
    return result;
}
auto from_tangent_space(std::array<double, 3> const& v) -> std::array<double, 3>
{
    std::array<double, 3> const result{
        2.0 * (v[0] - 0.5),
        2.0 * (v[1] - 0.5),
        2.0 * (1.0 - v[2]) + 1.0
    };
    return result;
}
