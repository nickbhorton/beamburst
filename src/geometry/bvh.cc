#include <iostream>
#include <queue>

#include "bvh.h"

BVHNode::BVHNode()
    : volume({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}), primitives(), children()
{
}

auto BVHNode::intersect(Line const& line) const -> std::optional<intersection_t>
{
    std::optional<intersection_t> intersection{};
    std::queue<BVHNode const*> q;
    q.push(this);
    while (!q.empty()) {
        BVHNode const* cn = q.front();
        q.pop();
        if (cn->test_intersect(line)) {
            bool const has_children = cn->children.size();
            bool const has_primitives = cn->primitives.size();
            if (has_children && not has_primitives) {
                for (auto const& child : cn->children) {
                    q.push(child.get());
                }
            } else if (not has_children && has_primitives) {
                for (auto const& intersectable : cn->primitives) {
                    std::optional<intersection_t> const new_intersection =
                        intersectable->intersect(line);
                    if (intersection.has_value() &&
                        new_intersection.has_value()) {
                        if (std::get<0>(new_intersection.value()) <
                            std::get<0>(intersection.value())) {
                            intersection = new_intersection;
                        }
                    } else if (!intersection.has_value() && new_intersection.has_value()) {
                        intersection = new_intersection;
                    }
                }
            } else {
                if (has_primitives && has_children) {
                    std::cerr << "bvh has children and primitives\n";
                } else {
                    std::cerr << "bvh has neither children nor primitives\n";
                }
            }
        }
    }
    return intersection;
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
