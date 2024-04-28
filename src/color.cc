#include "color.h"
#include "array_ops.h"
#include "cmath_extention.h"
#include <cmath>

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r{r}, g{g}, b{b}, a{a}
{
}


auto to_color(const std::array<double, 3>& vec_color) -> Color
{
    uint8_t r = std::floor(saturate(vec_color[0]) * 255.0);
    uint8_t g = std::floor(saturate(vec_color[1]) * 255.0);
    uint8_t b = std::floor(saturate(vec_color[2]) * 255.0);
    return Color{r, g, b, 255};
}

std::array<double, 3> to_tangent_space(const std::array<double, 3> v)
{
    auto nv = normalize(v);
    std::array<double, 3> result{
        0.5 * nv[0] + 0.5,
        0.5 * nv[1] + 0.5,
        1.0 - (0.5 * nv[0] - 0.5)
    };
    return result;
}

