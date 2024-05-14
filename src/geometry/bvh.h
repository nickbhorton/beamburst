#ifndef BEAMBURST_BVH_HEADER_
#define BEAMBURST_BVH_HEADER_
#include <memory>
#include <vector>

#include "aabb.h"
#include "intersectable.h"

class BVHNode : Intersectable
{
    AABB volume;
    // only leaves should contain primatives
    std::vector<Intersectable const*> primatives;
    // only interior nodes should contain children
    std::vector<std::unique_ptr<BVHNode>> children;

    // intersects primatives
    auto intersect(Line const& line) const -> std::optional<intersection_t>;
    // checks intersection with bounding volume
    auto test_intersect(Line const& line) const -> bool;
};

#endif
