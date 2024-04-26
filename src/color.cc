#include <cmath>
#include "color.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r{r}, g{g}, b{b}, a{a}
{
}

double saturate(double in)
{
    if (in > 1.0) {
        return 1.0;
    } else if (in < 0.0) {
        return 0.0;
    }
    return in;
}

auto to_color(const std::array<double, 3>& vec_color) -> Color
{
    uint8_t r = std::floor(saturate(vec_color[0]) * 255.0);
    uint8_t g = std::floor(saturate(vec_color[1]) * 255.0);
    uint8_t b = std::floor(saturate(vec_color[2]) * 255.0);
    return Color{r, g, b, 255};
}
