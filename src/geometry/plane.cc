#include "plane.h"

#include "intersections.h"
#include <iostream>

Plane::Plane(
    std::array<double, 3> const& position,
    std::array<double, 3> const& normal
)
    : position(position), normal(normal)
{
}

std::optional<double> Plane::find_intersection(Line const& line)
{
    return ::find_intersection(line, *this);
}

std::array<double, 3> Plane::find_surface_normal(
    [[maybe_unused]] std::array<double, 3> const& solution_position
)
{
    return normal;
}

std::array<double, 2> Plane::find_uv(
    [[maybe_unused]] std::array<double, 3> const& solution_position,
    [[maybe_unused]] std::array<double, 3> const& solution_normal
)
{
    std::cerr << "not implemented\n";
    return {0.0, 0.0};
}
