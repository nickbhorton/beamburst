#include "plane.h"

// private includes
#include "intersections.h"

Plane::Plane(
    const std::array<double, 3>& position,
    const std::array<double, 3>& normal
)
    : position(position), normal(normal)
{
}

std::optional<double> Plane::find_intersection(const Line& line)
{
    return ::find_intersection(line, *this);
}

std::array<double, 3> Plane::find_surface_normal(
    [[maybe_unused]] const std::array<double, 3>& solution_position
)
{
    return normal;
}
