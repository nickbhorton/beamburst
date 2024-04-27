#include "color.h"
#include "math/array_ops.h"
#include <cmath>

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

double clamp(double x, double min, double max)
{
    return std::min(std::max(x, min), max);
}

double smoothstep(double edge0, double edge1, double x)
{
    const double t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

double fract(double in) { return in - std::floor(in); }

double mod(double in, double mod_by)
{
    if (in == mod_by) {
        return in;
    }
    in = std::abs(in);
    mod_by = std::abs(mod_by);
    double div = in / mod_by;
    return in - static_cast<double>(std::floor(div)) * mod_by;
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

auto ucos(double x, double frequency) -> double
{
    return 0.5 * cos(x * frequency * 2.0 * M_PI);
}

auto gaussian(double x, double y, double c, double a, double x0, double y0)
    -> double
{
    return (a *
            std::exp(
                -(std::pow((x - x0) / c, 2) + std::pow((y - y0) / c, 2)) / 2.0
            )) /
           (std::sqrt(M_PI));
};

auto gaussian_normal(
    double x,
    double y,
    double c,
    double a,
    double x0,
    double y0
) -> std::array<double, 3>
{
    std::array<double, 3> result{};
    result[0] =
        -a * (2.0 / (std::sqrt(M_PI) * std::pow(c, 2))) * (x - x0) *
        std::exp(
            -(std::pow((x - x0) / c, 2) + std::pow((y - y0) / c, 2)) / 2.0
        );
    result[1] =
        -a * (2.0 / (std::sqrt(M_PI) * std::pow(c, 2))) * (y - y0) *
        std::exp(
            -(std::pow((x - x0) / c, 2) + std::pow((y - y0) / c, 2)) / 2.0
        );
    result[2] = -1.0;
    return result;
};
