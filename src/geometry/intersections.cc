#include <cmath>

#include "array_ops.h"
#include "intersections.h"

auto find_intersection(const Line& line, const Plane& plane)
    -> std::optional<double>
{
    const double n_dot_d = dot(plane.normal, line.direction);
    if (!std::isnormal(n_dot_d)) {
        return {};
    }
    return (dot(plane.normal, plane.position) - dot(plane.normal, line.position)
           ) /
           n_dot_d;
}
