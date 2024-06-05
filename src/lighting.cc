#include "lighting.h"
#include "array_ops.h"

auto blin_phong_specular(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& view_direction,
    std::array<double, 3> const& normal,
    double specular_hardness
) -> double
{
    std::array<double, 3> const normalized_normal = normalize(normal);
    std::array<double, 3> const light_direction =
        normalize(light_position - position);
    std::array<double, 3> const halfway =
        normalize(light_direction + normalize(view_direction));
    double const specular = std::pow(
        std::max(dot(normalized_normal, halfway), 0.0),
        specular_hardness
    );
    return specular;
}

auto phong_diffuse(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& normal
) -> double
{
    std::array<double, 3> const light_direction =
        normalize(light_position - position);
    std::array<double, 3> const normalized_normal = normalize(normal);
    double const diffuse =
        std::max(dot(light_direction, normalized_normal), 0.0);
    return diffuse;
}

auto beckman_distribution_specular(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& view_direction,
    std::array<double, 3> const& normal,
    double m
) -> double
{
    std::array<double, 3> const light_direction =
        normalize(light_position - position);
    std::array<double, 3> const halfway =
        normalize(light_direction + normalize(view_direction));
    double const a = std::acos(std::max(dot(normal, halfway), 0.0));
    double const ks = std::exp(-std::pow(std::tan(a), 2.0) / m * m) /
                      (M_PI * m * m * std::pow(std::cos(a), 2.0));
    return ks;
}

auto schlick_fresnel(double cos_theta, double n1, double n2) -> double
{
    double const r = std::pow((n1 - n2) / (n1 + n2), 2);
    return r + (1.0 - r) * std::pow(1.0 - cos_theta, 5.0);
}

auto cook_torrance_specular(
    std::array<double, 3> const& light_position,
    std::array<double, 3> const& position,
    std::array<double, 3> const& view_direction,
    std::array<double, 3> const& normal,
    double m,
    double n1,
    double n2
) -> double
{
    std::array<double, 3> const light_direction =
        normalize(light_position - position);
    std::array<double, 3> const halfway =
        normalize(light_direction + normalize(view_direction));
    double const D = beckman_distribution_specular(
        light_position,
        position,
        view_direction,
        normal,
        m
    );
    double const F = schlick_fresnel(dot(halfway, light_direction), n1, n2);
    double const before_v_or_l_dot_n =
        2.0 * dot(halfway, normal) / dot(view_direction, halfway);
    double const v_dot_n = dot(view_direction, normal);
    double const G = std::min(
        std::min(1.0, before_v_or_l_dot_n * v_dot_n),
        before_v_or_l_dot_n * dot(light_direction, normal)
    );
    double const kspec =
        (D * F * G) / (4.0 * v_dot_n * dot(normal, light_direction));
    return kspec;
}

PointLight::PointLight(std::array<double, 3> const& position)
    : position(position)
{
}

bool same_hemisphere(
    std::array<double, 3> const& a,
    std::array<double, 3> const& b
)
{
    return a[2] * b[2] > 0;
}
