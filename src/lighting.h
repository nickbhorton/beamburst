#ifndef BEAMBURST_LIGHTING_HEADER_
#define BEAMBURST_LIGHTING_HEADER_

#include <array>

struct PointLight {
    std::array<double, 3> position;
    double power;
    PointLight(const std::array<double, 3>& position, double power);
};

auto phong_diffuse(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& normal
) -> double;

auto blin_phong_specular(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double specular_hardness
) -> double;

auto beckman_distribution_specular(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double m
) -> double;

#endif
