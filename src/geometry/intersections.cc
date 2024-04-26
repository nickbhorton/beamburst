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
    const double result = (v1 - v2) / n_dot_d;
    if (result != 0.0 && std::signbit(result)) {
        return {};
    }
    return result;
}
auto find_intersection(const Line& line, const Sphere& sphere)
    -> std::optional<double>
{
    const double a = dot(line.direction, line.direction);
    const double b = 2.0 * (dot(line.position, line.direction) -
                            dot(line.direction, sphere.position));
    const double c = dot(line.position, line.position) +
                     dot(sphere.position, sphere.position) -
                     2.0 * dot(line.position, sphere.position) -
                     std::pow(sphere.radius, 2.0);
    const double discriminant = b * b - 4.0 * a * c;
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
    const double t1 = (-b + std::sqrt(discriminant)) / (2.0 * a);
    const double t2 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    const bool valid_positive_t1 =
        !std::isinf(t1) && !std::signbit(t1) && !std::isnan(t1);
    const bool valid_positive_t2 =
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
