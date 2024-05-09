#include "triangle.h"
#include "array_ops.h"

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
    std::array<double, 2> const* t0,
    std::array<double, 2> const* t1,
    std::array<double, 2> const* t2
)
    : p0{p0}, p1{p1}, p2{p2}, n0{n0}, n1{n1}, n2{n2}, t0{t0}, t1{t1}, t2{t2}
{
}

std::optional<double> Triangle::find_intersection(const Line& line)
{
    const std::array<double, 3> e1 = *p1 - *p0;
    const std::array<double, 3> e2 = *p2 - *p0;
    const std::array<double, 3> n = cross(e1, e2);
    const double D = -dot(*p0, n);
    const double denominator = dot(n, line.direction);
    if (!std::isnormal(denominator)) {
        return {};
    }
    const double t = -(D + dot(n, line.position)) / denominator;
    if (std::signbit(t)) {
        return {};
    }
    const std::array<double, 3> solution_position =
        line.position + (t * line.direction);
    const std::array<double, 3> ep = solution_position - *p0;
    const double d11 = dot(e1, e1);
    const double d12 = dot(e1, e2);
    const double d22 = dot(e2, e2);
    const double d1p = dot(e1, ep);
    const double d2p = dot(e2, ep);
    const double det = d11 * d22 - d12 * d12;
    if (!std::isnormal(det)) {
        return {};
    }
    beta = (d22 * d1p - d12 * d2p) / det;
    gamma = (d11 * d2p - d12 * d1p) / det;
    if (beta < 0.0 || beta > 1.0 || gamma < 0.0 || gamma > 1.0 ||
        beta + gamma > 1.0 || beta + gamma < 0.0) {
        return {};
    }
    return t;
}

std::array<double, 3>
Triangle::find_surface_normal(const std::array<double, 3>& solution_position)
{
    const double alpha = 1.0 - beta - gamma;

    if (n0 != nullptr && n1 != nullptr && n2 != nullptr) {
        return normalize(((alpha * *n0) + (beta * *n1) + (gamma * *n2)));
    }
    return normalize(cross((solution_position - *p0), (*p2 - *p0)));
}

std::array<double, 2> Triangle::find_uv(
    [[maybe_unused]] const std::array<double, 3>& solution_position,
    [[maybe_unused]] const std::array<double, 3>& solution_normal
)
{
    const double alpha = 1.0 - beta - gamma;
    double u = alpha * (*t0)[0] + beta * (*t1)[0] + gamma * (*t2)[0];
    double v = alpha * (*t0)[1] + beta * (*t1)[1] + gamma * (*t2)[1];
    return {u, v};
}
