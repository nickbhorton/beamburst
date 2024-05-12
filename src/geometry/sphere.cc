#include "sphere.h"

// private includes
#include "array_ops.h"
#include "intersections.h"
Sphere::Sphere(std::array<double, 3> const& position, double radius)
    : position(position), radius(radius)
{
}

auto Sphere::find_intersection(Line const& line) -> std::optional<double>
{
    return ::find_intersection(line, *this);
}

auto Sphere::find_surface_normal(std::array<double, 3> const& solution_position)
    -> std::array<double, 3>
{
    return solution_position - position;
}

auto Sphere::find_uv(
    [[maybe_unused]] std::array<double, 3> const& solution_position,
    std::array<double, 3> const& solution_normal
) -> std::array<double, 2>
{
    double const u =
        (std::atan2(solution_normal[1], solution_normal[0]) + 2 * M_PI) /
        (2.0f * M_PI);
    double const v = std::acos(solution_normal[2]) / M_PI;
    return {u, v};
}
