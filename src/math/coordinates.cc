#include <ctgmath>

#include "coordinates.h"
auto cartesion_to_spherical(const std::array<double, 3>& in)
    -> std::array<double, 3>
{
    std::array<double, 3> result{};
    result[0] =
        std::sqrt(std::pow(in[0], 2) + std::pow(in[1], 2) + std::pow(in[2], 2));
    result[1] = std::acos(in[2] / result[0]);
    result[2] = std::atan2(in[1], in[0]);
    return result;
}

auto spherical_to_cartesion(const std::array<double, 3>& in)
    -> std::array<double, 3>
{
    std::array<double, 3> result{};
    result[0] = in[0] * std::sin(in[1]) * std::cos(in[2]);
    result[1] = in[0] * std::sin(in[1]) * std::sin(in[2]);
    result[2] = in[0] * cos(in[1]);
    return result;
}
