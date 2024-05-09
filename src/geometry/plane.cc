#include "plane.h"

#include "intersections.h"
#include <iostream>

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

std::array<double, 2> Plane::find_uv(
    [[maybe_unused]] const std::array<double, 3>& solution_position,
    [[maybe_unused]] const std::array<double, 3>& solution_normal
)
{
    std::cerr << "not implemented\n";
    return {0.0, 0.0};
}
