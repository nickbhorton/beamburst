#ifndef BEAMBURST_LIGHTING_HEADER_
#define BEAMBURST_LIGHTING_HEADER_

#include <array>

auto blin_phong(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double specular_hardness
) -> std::tuple<double, double>;

#endif
