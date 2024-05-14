
#include "bvh.h"

auto BVHNode::intersect(Line const& line) const -> std::optional<intersection_t>
{
    std::optional<intersection_t> intersection{};
    for (auto const& primatives : primatives) {
        std::optional<intersection_t> const new_intersection =
            primatives->intersect(line);
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

auto BVHNode::test_intersect(Line const& line) const -> bool
{
    return volume.test_intersect(line);
}
