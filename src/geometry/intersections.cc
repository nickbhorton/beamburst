#include <cmath>

#include "array_ops.h"
#include "line.h"
#include "primitives/plane.h"
#include "primitives/sphere.h"

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

auto find_both_intersections(Line const& line, Sphere const& sphere)
    -> std::tuple<std::optional<double>, std::optional<double>>
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
            return {t, {}};
        } else {
            return {{}, {}};
        }
    }
    double const t1 = (-b + std::sqrt(discriminant)) / (2.0 * a);
    double const t2 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    return {t1, t2};
}
