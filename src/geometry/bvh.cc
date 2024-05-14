
#include "bvh.h"

BVHNode::BVHNode()
    : volume({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}), primitives(), children()
{
}

auto BVHNode::intersect(Line const& line) const -> std::optional<intersection_t>
{
    if (volume.test_intersect(line)) {
        std::optional<intersection_t> intersection{};
        for (auto const& primitive : primitives) {
            std::optional<intersection_t> const new_intersection =
                primitive->intersect(line);
            if (intersection.has_value() && new_intersection.has_value()) {
                if (std::get<0>(new_intersection.value()) <
                    std::get<0>(intersection.value())) {
                    intersection = new_intersection;
                }
            } else if (!intersection.has_value() && new_intersection.has_value()) {
                intersection = new_intersection;
            }
        }
        return intersection;
    }
    return {};
}

auto BVHNode::test_intersect(Line const& line) const -> bool
{
    return volume.test_intersect(line);
}

auto BVHNode::add_primitive(Intersectable const* iptr) -> void
{
    primitives.push_back(iptr);
    auto const iptr_min_point = iptr->get_min_extent();
    auto const iptr_max_point = iptr->get_max_extent();
    for (size_t i = 0; i < 3; i++) {
        if (iptr_max_point[i] > volume.max_point[i]) {
            volume.max_point[i] = iptr_max_point[i];
        }
        if (iptr_min_point[i] < volume.min_point[i]) {
            volume.min_point[i] = iptr_min_point[i];
        }
    }
}
