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

auto schlick_fresnel(double cos_theta, double n1, double n2) -> double
{
    const double r = std::pow((n1 - n2) / (n1 + n2), 2);
    return r + (1.0 - r) * std::pow(1.0 - cos_theta, 5.0);
}

auto cook_torrance_specular(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double m,
    double n1,
    double n2
) -> double
{
    const std::array<double, 3> light_direction =
        normalize(light_position - position);
    const std::array<double, 3> halfway =
        normalize(light_direction + normalize(view_direction));
    const double D = beckman_distribution_specular(
        light_position,
        position,
        view_direction,
        normal,
        m
    );
    const double F = schlick_fresnel(dot(halfway, light_direction), n1, n2);
    const double before_v_or_l_dot_n =
        2.0 * dot(halfway, normal) / dot(view_direction, halfway);
    const double v_dot_n = dot(view_direction, normal);
    const double G = std::min(
        std::min(1.0, before_v_or_l_dot_n * v_dot_n),
        before_v_or_l_dot_n * dot(light_direction, normal)
    );
    double kspec = (D * F * G) / (4.0 * v_dot_n * dot(normal, light_direction));
    return kspec;
}

PointLight::PointLight(const std::array<double, 3>& position, double power)
    : position(position), power(power)
{
}
