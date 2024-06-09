#ifndef BEAMBURST_LIGHTING_HEADER_
#define BEAMBURST_LIGHTING_HEADER_

#include <array>

struct PointLight {
    std::array<double, 3> position;
    PointLight(std::array<double, 3> const& position);
};

struct ColoredPointLight {
    std::array<double, 3> position;
    std::array<double, 3> intensity;
    ColoredPointLight(
        std::array<double, 3> const& position,
        std::array<double, 3> const& intensity
    );
};

// helper functions
bool same_hemisphere(
    std::array<double, 3> const& a,
    std::array<double, 3> const& b
);

// functions from wikipedia mostly
auto phong_diffuse(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& normal
) -> double;

auto blin_phong_specular(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& view_direction,
    std::array<double, 3> const& normal,
    double specular_hardness
) -> double;

auto beckman_distribution_specular(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& view_direction,
    std::array<double, 3> const& normal,
    double m
) -> double;

auto cook_torrance_specular(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& view_direction,
    std::array<double, 3> const& normal,
    double m,
    double n1,
    double n2
) -> double;

auto schlick_fresnel(double cos_theta, double n1, double n2) -> double;
#endif
