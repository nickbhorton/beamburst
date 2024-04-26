#ifndef BEAMBURST_COORDINATES_HEADER_
#define BEAMBURST_COORDINATES_HEADER_

#include <array>

auto cartesion_to_spherical(const std::array<double, 3>& in)
    -> std::array<double, 3>;

auto spherical_to_cartesion(const std::array<double, 3>& in)
    -> std::array<double, 3>;

#endif
