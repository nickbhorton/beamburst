#include "sphere.h"

// private includes
#include "array_ops.h"
#include "intersections.h"
Sphere::Sphere(std::array<double, 3> const& position, double radius)
    : position(position), radius(radius)
{
}

auto Sphere::intersect(Line const& line) const -> std::optional<intersection_t>
{
    std::optional<double> const t_opt = ::find_intersection(line, *this);
    if (!t_opt.has_value()) {
        return {};
    }
    auto const solution_poition =
        line.position + line.direction * t_opt.value();
    auto const solution_normal = normalize(solution_poition - position);
    double const u =
        (std::atan2(solution_normal[1], solution_normal[0]) + 2 * M_PI) /
        (2.0f * M_PI);
    double const v = std::acos(solution_normal[2]) / M_PI;
    std::array<double, 2> const uv = {u, v};
    intersection_t result =
        {t_opt.value(), normalize(solution_poition - position), uv, this};
    return result;
}

auto Sphere::get_max_extent() const -> std::array<double, 3>
{
    return {position[0] + radius, position[1] + radius, position[2] + radius};
}
auto Sphere::get_min_extent() const -> std::array<double, 3>
{
    return {position[0] - radius, position[1] - radius, position[2] - radius};
}
