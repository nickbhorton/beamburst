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

auto Triangle::find_intersection(const Line& line) -> std::optional<double>
{
    std::array<double, 3> const e1 = *p1 - *p0;
    std::array<double, 3> const e2 = *p2 - *p0;
    std::array<double, 3> const n = cross(e1, e2);
    double const D = -dot(*p0, n);
    double const denominator = dot(n, line.direction);
    if (!std::isnormal(denominator)) {
        return {};
    }
    double const t = -(D + dot(n, line.position)) / denominator;
    if (std::signbit(t)) {
        return {};
    }
    std::array<double, 3> const solution_position =
        line.position + (t * line.direction);
    std::array<double, 3> const ep = solution_position - *p0;
    double const d11 = dot(e1, e1);
    double const d12 = dot(e1, e2);
    double const d22 = dot(e2, e2);
    double const d1p = dot(e1, ep);
    double const d2p = dot(e2, ep);
    double det = d11 * d22 - d12 * d12;
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

auto Triangle::find_surface_normal(
    const std::array<double, 3>& solution_position
) -> std::array<double, 3>
{
    const double alpha = 1.0 - beta - gamma;
    if (n0 != nullptr && n1 != nullptr && n2 != nullptr) {
        return normalize(((alpha * *n0) + (beta * *n1) + (gamma * *n2)));
    }
    return normalize(cross((solution_position - *p0), (*p2 - *p0)));
}
auto Triangle::find_uv(
    [[maybe_unused]] const std::array<double, 3>& solution_position,
    [[maybe_unused]] const std::array<double, 3>& solution_normal
) -> std::array<double, 2>
{
    const double alpha = 1.0 - beta - gamma;
    double u = alpha * (*t0)[0] + beta * (*t1)[0] + gamma * (*t2)[0];
    double v = alpha * (*t0)[1] + beta * (*t1)[1] + gamma * (*t2)[1];
    return {u, v};
}
auto Triangle::get_p0() const -> std::array<double, 3>
{
    if (p0 != nullptr) {
        return *p0;
    }
    std::cerr << "vertex position p0 was nullptr\n";
    return {0.0, 0.0, 0.0};
}
auto Triangle::get_p1() const -> std::array<double, 3>
{
    if (p1 != nullptr) {
        return *p1;
    }
    std::cerr << "vertex position p1 was nullptr\n";
    return {0.0, 0.0, 0.0};
}
auto Triangle::get_p2() const -> std::array<double, 3>
{
    if (p2 != nullptr) {
        return *p2;
    }
    std::cerr << "vertex position p2 was nullptr\n";
    return {0.0, 0.0, 0.0};
}
