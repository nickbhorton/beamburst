#include "aabb.h"
#include "intersectable.h"
#include <cassert>
#include <optional>

AABB::AABB(
    std::array<double, 3> const& min_point,
    std::array<double, 3> const& max_point
)
    : min_point(min_point), max_point(max_point)
{
}

auto AABB::intersect(Line const& line) const -> std::optional<intersection_t>
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
    intersection_t result = {txmin, {0.0, 1.0, 0.0}, {}};
    return result;
}

auto AABB::find_intersection(Line const& line) -> std::optional<double>
{
    std::optional<intersection_t> const result = intersect(line);
    if (result.has_value()) {
        return std::get<0>(result.value());
    }
    return {};
}

auto AABB::find_surface_normal(
    [[maybe_unused]] std::array<double, 3> const& solution_position
) -> std::array<double, 3>
{
    return {0.0, 1.0, 0.0};
}

auto AABB::find_uv(
    [[maybe_unused]] std::array<double, 3> const& solution_position,
    [[maybe_unused]] std::array<double, 3> const& solution_normal
) -> std::array<double, 2>
{
    return {0.0, 0.0};
}
