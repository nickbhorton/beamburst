#include "aabb.h"
#include <cassert>
#include <optional>

AABB::AABB(
    std::array<double, 3> const& min_point,
    std::array<double, 3> const& max_point
)
    : min_point(min_point), max_point(max_point)
{
}

auto AABB::find_intersection([[maybe_unused]] Line const& line)
    -> std::optional<double>
{
    double txmin = (min_point[0] - line.position[0]) / line.direction[0];
    double txmax = (max_point[0] - line.position[0]) / line.direction[0];
    if (txmin > txmax) {
        std::swap(txmin, txmax);
    }

    double tymin = (min_point[1] - line.position[1]) / line.direction[1];
    double tymax = (max_point[1] - line.position[1]) / line.direction[1];
    if (tymin > tymax) {
        std::swap(tymin, tymax);
    }

    if ((txmin > tymax) || (tymin > txmax)) {
        return {};
    }

    if (tymin > txmin) {
        txmin = tymin;
    }

    if (tymax < txmax) {
        txmax = tymax;
    }

    double tzmin = (min_point[2] - line.position[2]) / line.direction[2];
    double tzmax = (max_point[2] - line.position[2]) / line.direction[2];
    if (tzmin > tzmax) {
        std::swap(tzmin, tzmax);
    }

    if ((txmin > tzmax) || (tzmin > txmax))
        return {};

    if (tzmin > txmin) {
        txmin = tzmin;
    }
    if (tzmax < txmax) {
        txmax = tzmax;
    }
    if (txmin > txmax) {
        std::swap(txmin, txmax);
    }
    return txmin;
}

auto AABB::find_surface_normal(
    [[maybe_unused]] std::array<double, 3> const& solution_position
) -> std::array<double, 3>
{
    // don't use this function. Im being lazy with interfaces at the moment
    // because sometimes code that works is better than me losing 3-4 coding
    // sessions on learning how to do abstract classes correctly.
    assert(1 == 0);
    return {0.0, 0.0, 0.0};
}

auto AABB::find_uv(
    [[maybe_unused]] std::array<double, 3> const& solution_position,
    [[maybe_unused]] std::array<double, 3> const& solution_normal
) -> std::array<double, 2>
{
    assert(1 == 0);
    return {0.0, 0.0};
}
