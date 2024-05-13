#include "triangle.h"
#include "array_ops.h"
#include "intersectable.h"
#include <optional>

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

auto Triangle::compute_tabc(Line const& line) const
    -> std::optional<std::tuple<double, double, double, double>>
{
    std::array<double, 3> const e1 = *p1 - *p0;
    std::array<double, 3> const e2 = *p2 - *p0;
    std::array<double, 3> const n = cross(e1, e2);
    double const D = -dot(*p0, n);
    double const denominator = dot(n, line.direction);
    if (!std::isnormal(denominator)) {
        return {};
    }
    double const time = -(D + dot(n, line.position)) / denominator;
    if (std::signbit(time)) {
        return {};
    }
    std::array<double, 3> const solution_position =
        line.position + (time * line.direction);
    std::array<double, 3> const ep = solution_position - *p0;
    double const d11 = dot(e1, e1);
    double const d12 = dot(e1, e2);
    double const d22 = dot(e2, e2);
    double const d1p = dot(e1, ep);
    double const d2p = dot(e2, ep);
    double const det = d11 * d22 - d12 * d12;
    if (!std::isnormal(det)) {
        return {};
    }
    double const beta = (d22 * d1p - d12 * d2p) / det;
    double const gamma = (d11 * d2p - d12 * d1p) / det;
    double const alpha = 1 - beta - gamma;
    if (beta < 0.0 || beta > 1.0 || gamma < 0.0 || gamma > 1.0 ||
        beta + gamma > 1.0 || beta + gamma < 0.0) {
        return {};
    }
    std::tuple<double, double, double, double> const result =
        {time, alpha, beta, gamma};
    return result;
}

auto Triangle::find_intersection(Line const& line) -> std::optional<double>
{
    auto const opt = compute_tabc(line);
    if (opt.has_value()) {
        auto const& [t, alpha, beta, gamma] = opt.value();
        this->beta = beta;
        this->gamma = gamma;
        return t;
    }
    return {};
}

auto Triangle::calculate_surface_normal(
    std::array<double, 3> const& solution_position,
    double alpha,
    double beta,
    double gamma
) const
{
    if (n0 != nullptr && n1 != nullptr && n2 != nullptr) {
        return normalize(((alpha * *n0) + (beta * *n1) + (gamma * *n2)));
    }
    return normalize(cross((solution_position - *p0), (*p2 - *p0)));
}

auto Triangle::find_surface_normal(
    std::array<double, 3> const& solution_position
) -> std::array<double, 3>
{
    return calculate_surface_normal(
        solution_position,
        1.0 - beta - gamma,
        beta,
        gamma
    );
}

auto Triangle::calculate_uv(double alpha, double beta, double gamma) const
    -> std::optional<std::array<double, 2>>
{
    if (t0 != nullptr && t1 != nullptr && t2 != nullptr) {
        double u = alpha * (*t0)[0] + beta * (*t1)[0] + gamma * (*t2)[0];
        double v = alpha * (*t0)[1] + beta * (*t1)[1] + gamma * (*t2)[1];
        return std::array<double, 2>({u, v});
    }
    return {};
}

auto Triangle::find_uv(
    [[maybe_unused]] const std::array<double, 3>& solution_position,
    [[maybe_unused]] const std::array<double, 3>& solution_normal
) -> std::array<double, 2>
{
    return calculate_uv(1.0 - beta - gamma, beta, gamma).value();
}

auto Triangle::intersect(Line const& line) const
    -> std::optional<intersection_t>
{
    auto const opt = compute_tabc(line);
    if (opt.has_value()) {
        auto const& [t, alpha, beta, gamma] = opt.value();
        auto const solution_position = line.position + line.direction * t;
        intersection_t const result = {
            t,
            calculate_surface_normal(solution_position, alpha, beta, gamma),
            calculate_uv(alpha, beta, gamma)
        };
        return result;
    }
    return {};
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
