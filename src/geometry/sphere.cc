#include "sphere.h"

// private includes
#include "array_ops.h"
#include "intersections.h"
Sphere::Sphere(const std::array<double, 3>& position, double radius)
    : position(position), radius(radius)
{
}

std::optional<double> Sphere::find_intersection(const Line& line) const
{
    return ::find_intersection(line, *this);
}

std::array<double, 3>
Sphere::find_surface_normal(const std::array<double, 3>& solution_position
) const
{
    return solution_position - position;
}


