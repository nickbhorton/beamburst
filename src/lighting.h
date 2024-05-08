#ifndef BEAMBURST_LIGHTING_HEADER_
#define BEAMBURST_LIGHTING_HEADER_

#include <array>

struct PointLight {
    std::array<double, 3> position;
    PointLight(const std::array<double, 3>& position);
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

auto cook_torrance_specular(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double m,
    double n1,
    double n2
) -> double;

auto schlick_fresnel(double cos_theta, double n1, double n2) -> double;
#endif
