#include "sphere.h"

// private includes
#include "array_ops.h"
#include "intersections.h"
Sphere::Sphere(const std::array<double, 3>& position, double radius)
    : position(position), radius(radius)
{
}

std::optional<double> Sphere::find_intersection(const Line& line)
{
    return ::find_intersection(line, *this);
}

std::array<double, 3>
Sphere::find_surface_normal(const std::array<double, 3>& solution_position)
{
    return solution_position - position;
}

std::array<double, 2> Sphere::find_uv(
    [[maybe_unused]] const std::array<double, 3>& solution_position,
    const std::array<double, 3>& solution_normal
)
{
    const double u =
        (std::atan2(solution_normal[1], solution_normal[0]) + 2 * M_PI) /
        (2.0f * M_PI);
    const double v = std::acos(solution_normal[2]) / M_PI;
    return {u, v};
}
