#ifndef BEAMBURST_TRIANGLE_BOUNDING_VOLUME_HEADER_
#define BEAMBURST_TRIANGLE_BOUNDING_VOLUME_HEADER_

#include <vector>

#include "aabb.h"
#include "intersectable.h"
#include "triangle.h"

class TriangleBoundingVolume : public Intersectable
{
    std::vector<Triangle> triangles;
    AABB bounding_volume;
    Triangle* previous_intersection;

public:
    TriangleBoundingVolume(std::vector<Triangle> triangles);
    ~TriangleBoundingVolume() = default;

    auto intersect( Line const& line) const
        -> std::optional<intersection_t>;

    auto find_intersection(Line const& line) -> std::optional<double>;
    auto find_surface_normal(std::array<double, 3> const& solution_position)
        -> std::array<double, 3>;
    auto find_uv(
        std::array<double, 3> const& solution_position,
        std::array<double, 3> const& solution_normal
    ) -> std::array<double, 2>;
};

#endif
