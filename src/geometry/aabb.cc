#include "aabb.h"

AABB::AABB(
    std::array<double, 3> const& min_point,
    std::array<double, 3> const& max_point
)
    : min_point(min_point), max_point(max_point)
{
}

auto AABB::get_max_extent() const -> std::array<double, 3> { return max_point; }
auto AABB::get_min_extent() const -> std::array<double, 3> { return min_point; }

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
auto AABB::test_intersect(Line const& line) const -> bool
{
    double txmax;
    double txmin;
    double tymax;
    double tymin;
    double tzmax;
    double tzmin;

    if (1.0 / line.direction[0] >= 0) {
        txmin = (min_point[0] - line.position[0]) / line.direction[0];
        txmax = (max_point[0] - line.position[0]) / line.direction[0];
    } else {
        txmin = (max_point[0] - line.position[0]) / line.direction[0];
        txmax = (min_point[0] - line.position[0]) / line.direction[0];
    }

    if (1.0 / line.direction[1] >= 0) {
        tymin = (min_point[1] - line.position[1]) / line.direction[1];
        tymax = (max_point[1] - line.position[1]) / line.direction[1];
    } else {
        tymin = (max_point[1] - line.position[1]) / line.direction[1];
        tymax = (min_point[1] - line.position[1]) / line.direction[1];
    }

    if ((txmin > tymax) || (tymin > txmax)) {
        return false;
    }

    if (tymin > txmin) {
        txmin = tymin;
    }

    if (tymax < txmax) {
        txmax = tymax;
    }

    if (1.0 / line.direction[2] >= 0) {
        tzmin = (min_point[2] - line.position[2]) / line.direction[2];
        tzmax = (max_point[2] - line.position[2]) / line.direction[2];
    } else {
        tzmin = (max_point[2] - line.position[2]) / line.direction[2];
        tzmax = (min_point[2] - line.position[2]) / line.direction[2];
    }

    if ((txmin > tzmax) || (tzmin > txmax)) {
        return false;
    }

    return true;
}
