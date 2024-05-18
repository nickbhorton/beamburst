#include <cmath>

#include "cmath_extention.h"

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
