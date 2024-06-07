#ifndef BEAMBURST_BVH_HEADER_
#define BEAMBURST_BVH_HEADER_
#include <memory>
#include <vector>

#include "aabb.h"
#include "intersectable.h"

class BVHNode : public Intersectable
{
    AABB volume;
    // only leaves should contain primatives
    std::vector<Intersectable const*> primitives;
    // only interior nodes should contain children
    std::vector<std::unique_ptr<BVHNode>> children;

public:
    BVHNode();
    BVHNode(std::vector<Intersectable const*> const& primatives);

    auto intersect(Line const& line, Float max_t) const
        -> std::optional<std::unique_ptr<SurfaceIntersection>>;
    auto get_max_extent() const -> std::array<Float, 3>;
    auto get_min_extent() const -> std::array<Float, 3>;

    auto test_intersect(Line const& line) const -> bool;

    auto print(std::ostream& os) const -> void;

    auto add_primitive(Intersectable const* iptr) -> void;
    auto construct_tree() -> void;
};

#endif
