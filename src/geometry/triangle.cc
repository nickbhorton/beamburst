
#include "triangle.h"
#include "array_ops.h"
#include "intersections.h"
#include <cmath>

std::optional<double> Triangle::find_intersection(const Line& line) const
{
    return ::find_intersection(line, *this);
}
std::array<double, 3>
Triangle::find_surface_normal(const std::array<double, 3>& solution_position
) const
{
    // everything is recomputed from intersection but non of
    // it is particulary expensive so for now it is ok.
    const std::array<double, 3> e1 = *p1 - *p0;
    const std::array<double, 3> e2 = *p2 - *p0;
    const std::array<double, 3> ep = solution_position - *p0;
    const double d11 = dot(e1, e1);
    const double d12 = dot(e1, e2);
    const double d22 = dot(e2, e2);
    const double d1p = dot(e1, ep);
    const double d2p = dot(e2, ep);
    // this should be safe as it should only be called after checking
    // intersection
    const double det = d11 * d22 + d12 * d12;
    const double beta = (d22 * d1p + d12 * d2p) / det;
    const double gamma = (d11 * d2p + d12 * d1p) / det;
    const double alpha = 1.0 - beta - gamma;

    if (n0 != nullptr && n1 != nullptr && n2 != nullptr) {
        return normalize(((alpha * *n0) + (beta * *n1) + (gamma * *n2)));
    }
    return normalize(cross((solution_position - *p0), (*p2 - *p0)));
}
