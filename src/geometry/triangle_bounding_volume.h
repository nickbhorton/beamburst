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

    auto intersect(Line const& line) const -> std::optional<intersection_t>;
};

#endif
