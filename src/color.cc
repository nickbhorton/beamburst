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

auto to_tangent_space(std::array<double, 3> const& v) -> std::array<double, 3>
{
    auto const nv = normalize(v);
    std::array<double, 3> const result{
        0.5 * nv[0] + 0.5,
        0.5 * nv[1] + 0.5,
        1.0 - (0.5 * nv[0] - 0.5)
    };
    return result;
}
