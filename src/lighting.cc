#include "lighting.h"
#include "array_ops.h"

auto blin_phong_specular(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double specular_hardness
) -> double
{
    const std::array<double, 3> normalized_normal = normalize(normal);
    const std::array<double, 3> light_direction =
        normalize(light_position - position);
    const std::array<double, 3> halfway =
        normalize(light_direction + normalize(view_direction));
    double specular = std::pow(
        std::max(dot(normalized_normal, halfway), 0.0),
        specular_hardness
    );
    return specular;
}
auto phong_diffuse(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& normal
) -> double
{
    const std::array<double, 3> light_direction =
        normalize(light_position - position);
    const std::array<double, 3> normalized_normal = normalize(normal);
    double diffuse = std::max(dot(light_direction, normalized_normal), 0.0);
    return diffuse;
}

auto beckman_distribution_specular(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double m
) -> double
{
    const std::array<double, 3> light_direction =
        normalize(light_position - position);
    const std::array<double, 3> halfway =
        normalize(light_direction + normalize(view_direction));
    const double a = std::acos(std::max(dot(normal, halfway), 0.0));
    const double ks = std::exp(-std::pow(std::tan(a), 2.0) / m * m) /
                      (M_PI * m * m * std::pow(std::cos(a), 2.0));
    return ks;
}

PointLight::PointLight(const std::array<double, 3>& position, double power)
    : position(position), power(power)
{
}
