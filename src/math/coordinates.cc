#include <cmath>
#include <iostream>

#include "coordinates.h"

static double arctan(double y, double x)
{
    if (x > 0.0) {
        return std::atan(y / x);
    } else if (x < 0.0 && y >= 0) {
        return std::atan(y / x) + M_PI;
    } else if (x < 0.0 && y < 0) {
        return std::atan(y / x) - M_PI;
    } else if (y > 0) {
        return M_PI / 2.0;
    } else if (y < 0) {
        return -M_PI / 2.0;
    }
    else {
        std::cerr << "arctan undefined\n";
        return 0.0;
    }
}

auto cartesian_to_spherical(const std::array<double, 3>& in)
    -> std::array<double, 3>
{
    std::array<double, 3> result{};
    result[0] =
        std::sqrt(std::pow(in[0], 2) + std::pow(in[1], 2) + std::pow(in[2], 2));
    result[1] = std::acos(in[2] / result[0]);
    result[2] = arctan(in[1], in[0]);
    return result;
}


auto spherical_to_cartesian(const std::array<double, 3>& in)
    -> std::array<double, 3>
{
    std::array<double, 3> result{};
    result[0] = in[0] * std::sin(in[1]) * std::cos(in[2]);
    result[1] = in[0] * std::sin(in[1]) * std::sin(in[2]);
    result[2] = in[0] * cos(in[1]);
    return result;
}
