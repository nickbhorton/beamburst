#ifndef BEAMBURST_COORDINATES_HEADER_
#define BEAMBURST_COORDINATES_HEADER_

#include <array>

auto cartesian_to_spherical(const std::array<double, 3>& in)
    -> std::array<double, 3>;

auto cartesian_to_sphere_uv(const std::array<double, 3>& in)
    -> std::array<double, 3>;

auto spherical_to_cartesian(const std::array<double, 3>& in)
    -> std::array<double, 3>;

#endif
