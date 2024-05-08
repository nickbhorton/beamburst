
#include "triangle.h"
#include "array_ops.h"
#include "intersections.h"
Triangle::Triangle(
    std::array<double, 3> const* p0,
    std::array<double, 3> const* p1,
    std::array<double, 3> const* p2
)
    : p0{p0}, p1{p1}, p2{p2}, n0{nullptr}, n1{nullptr}, n2{nullptr},
      t0{nullptr}, t1{nullptr}
{
}
Triangle::Triangle(
    std::array<double, 3> const* p0,
    std::array<double, 3> const* p1,
    std::array<double, 3> const* p2,
    std::array<double, 3> const* n0,
    std::array<double, 3> const* n1,
    std::array<double, 3> const* n2
)

    : p0{p0}, p1{p1}, p2{p2}, n0{n0}, n1{n1}, n2{n2}, t0{nullptr}, t1{nullptr}
{
}

Triangle::Triangle(
    std::array<double, 3> const* p0,
    std::array<double, 3> const* p1,
    std::array<double, 3> const* p2,
    std::array<double, 3> const* n0,
    std::array<double, 3> const* n1,
    std::array<double, 3> const* n2,
    std::array<double, 3> const* t0,
    std::array<double, 3> const* t1
)
    : p0{p0}, p1{p1}, p2{p2}, n0{n0}, n1{n1}, n2{n2}, t0{t0}, t1{t1}
{
}

std::optional<double> Triangle::find_intersection(const Line& line) const
{
    return ::find_intersection(line, *this);
}

// TODO: DRY hear with intersections.cc
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
    const double det = d11 * d22 - d12 * d12;
    const double beta = (d22 * d1p - d12 * d2p) / det;
    const double gamma = (d11 * d2p - d12 * d1p) / det;
    const double alpha = 1.0 - beta - gamma;

    if (n0 != nullptr && n1 != nullptr && n2 != nullptr) {
        return normalize(((alpha * *n0) + (beta * *n1) + (gamma * *n2)));
    }
    return normalize(cross((solution_position - *p0), (*p2 - *p0)));
}

double Triangle::compute_det(
    const std::array<double, 3>& e1,
    const std::array<double, 3>& e2
) const
{
    const double d11 = dot(e1, e1);
    const double d12 = dot(e1, e2);
    const double d22 = dot(e2, e2);
    const double det = d11 * d22 - d12 * d12;
    return det;
}
