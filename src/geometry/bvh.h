#ifndef BEAMBURST_BVH_HEADER_
#define BEAMBURST_BVH_HEADER_
#include <memory>
#include <vector>

#include "aabb.h"
#include "intersectable.h"

class BVHNode
{
    AABB volume;
    // only leaves should contain primatives
    std::vector<Intersectable const*> primitives;
    // only interior nodes should contain children
    std::vector<std::unique_ptr<BVHNode>> children;

public:
    // TODO: rule of 5 :(
    BVHNode();
    BVHNode(std::vector<Intersectable const*> const& primatives);

    // intersects primatives
    auto intersect(Line const& line) const -> std::optional<intersection_t>;
    // checks intersection with bounding volume
    auto test_intersect(Line const& line) const -> bool;

    auto print(std::ostream& os) const -> void;

    auto add_primitive(Intersectable const* iptr) -> void;
    auto construct_tree() -> void;
};

#endif
