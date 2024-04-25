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
    const double v1 = dot(plane.normal, plane.position);
    if (std::isnan(v1) || std::isinf(v1)) {
        return {};
    }
    const double v2 = dot(plane.normal, line.position);
    if (std::isnan(v2) || std::isinf(v2)) {
        return {};
    }
    return (v1 - v2) / n_dot_d;
}
