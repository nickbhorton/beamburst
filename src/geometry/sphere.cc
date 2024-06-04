#include "sphere.h"

// private includes
#include "array_ops.h"
#include "intersections.h"
Sphere::Sphere(std::array<double, 3> const& position, double radius)
    : position(position), radius(radius)
{
}

auto Sphere::intersect(
    Line const& line,
    [[maybe_unused]] Intersectable const* remove_ptr
) const -> std::optional<intersection_t>
{
    std::optional<double> const t_opt = ::find_intersection(line, *this);
    if (!t_opt.has_value()) {
        return {};
    }
    auto const solution_poition =
        line.position + line.direction * t_opt.value();
    auto const solution_normal = normalize(solution_poition - position);
    std::array<double, 3> const north_pole{0, 0, radius};
    auto const north_pole_dir = north_pole - solution_poition;
    auto const solution_tangent =
        normalize(cross(north_pole_dir, solution_normal));
    auto const solution_bitangent =
        normalize(cross(solution_normal, solution_tangent));
    std::array<std::array<double, 3>, 3> const normal_coords{
        {solution_tangent, solution_bitangent, solution_normal}
    };

    double const u =
        (std::atan2(solution_normal[1], solution_normal[0]) + M_PI) /
        (2.0f * M_PI);
    double const v = std::acos(solution_normal[2]) / M_PI;
    std::array<double, 2> const uv = {u, v};

    intersection_t result =
        {t_opt.value(), solution_normal, transpose(normal_coords), uv, this};
    return result;
}

auto Sphere::inside_intersect(
    [[maybe_unused]] Line const& line,
    [[maybe_unused]] Intersectable const* insider
) const -> std::optional<intersection_t>
{
    return {};
}

auto Sphere::get_max_extent() const -> std::array<double, 3>
{
    return {position[0] + radius, position[1] + radius, position[2] + radius};
}
auto Sphere::get_min_extent() const -> std::array<double, 3>
{
    return {position[0] - radius, position[1] - radius, position[2] - radius};
}
