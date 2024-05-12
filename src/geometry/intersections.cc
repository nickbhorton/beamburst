#include <cmath>

#include "array_ops.h"
#include "intersections.h"

auto find_intersection(Line const& line, Plane const& plane)
    -> std::optional<double>
{
    double const n_dot_d = dot(plane.normal, line.direction);
    if (!std::isnormal(n_dot_d)) {
        return {};
    }
    double const v1 = dot(plane.normal, plane.position);
    if (std::isnan(v1) || std::isinf(v1)) {
        return {};
    }
    double const v2 = dot(plane.normal, line.position);
    if (std::isnan(v2) || std::isinf(v2)) {
        return {};
    }
    double const result = (v1 - v2) / n_dot_d;
    if (result != 0.0 && std::signbit(result)) {
        return {};
    }
    return result;
}
auto find_intersection(Line const& line, Sphere const& sphere)
    -> std::optional<double>
{
    double const a = dot(line.direction, line.direction);
    double const b = 2.0 * (dot(line.position, line.direction) -
                            dot(line.direction, sphere.position));
    double const c = dot(line.position, line.position) +
                     dot(sphere.position, sphere.position) -
                     2.0 * dot(line.position, sphere.position) -
                     std::pow(sphere.radius, 2.0);
    double const discriminant = b * b - 4.0 * a * c;
    if (std::isinf(discriminant) || std::signbit(discriminant) ||
        std::isnan(discriminant)) {
        return {};
    }
    if (discriminant == 0.0) {
        double t = -b / (2.0 * a);
        if (!std::isinf(t) && !std::signbit(t) && !std::isnan(t)) {
            return t;
        } else {
            return {};
        }
    }
    double const t1 = (-b + std::sqrt(discriminant)) / (2.0 * a);
    double const t2 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    bool const valid_positive_t1 =
        !std::isinf(t1) && !std::signbit(t1) && !std::isnan(t1);
    bool const valid_positive_t2 =
        !std::isinf(t2) && !std::signbit(t2) && !std::isnan(t2);
    if (!valid_positive_t1 && !valid_positive_t2) {
        return {};
    } else if (valid_positive_t1 && !valid_positive_t2) {
        return t1;
    } else if (!valid_positive_t1 && valid_positive_t2) {
        return t2;
    } else {
        return std::min(t1, t2);
    }
}

// #include "triangle.h"
// @depreciatied
// I want to use dry and potentially cache results for the normal calculator
// I will leave this here for reference but I should not be used.
/*
auto find_intersection(const Line& line, const Triangle& triangle)
    -> std::optional<double>
{
    const std::array<double, 3> e1 = *triangle.p1 - *triangle.p0;
    const std::array<double, 3> e2 = *triangle.p2 - *triangle.p0;
    const std::array<double, 3> n = cross(e1, e2);
    const double D = -dot(*triangle.p0, n);
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
    const std::array<double, 3> ep = solution_position - *triangle.p0;
    const double d11 = dot(e1, e1);
    const double d12 = dot(e1, e2);
    const double d22 = dot(e2, e2);
    const double d1p = dot(e1, ep);
    const double d2p = dot(e2, ep);
    const double det = d11 * d22 - d12 * d12;
    if (!std::isnormal(det)) {
        return {};
    }
    const double beta = (d22 * d1p - d12 * d2p) / det;
    const double gamma = (d11 * d2p - d12 * d1p) / det;
    if (beta < 0.0 || beta > 1.0 || gamma < 0.0 || gamma > 1.0 ||
        beta + gamma > 1.0 || beta + gamma < 0.0) {
        return {};
    }
    return t;
}
*/
