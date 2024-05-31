#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>

#include "array_ops.h"
#include "bvh.h"
#include "intersectable.h"

BVHNode::BVHNode()
    : volume({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}), primitives(), children()
{
}
BVHNode::BVHNode(std::vector<Intersectable const*> const& primitives)
    : volume({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}), primitives(), children()
{
    for (auto const& p : primitives) {
        add_primitive(p);
    }
    this->construct_tree();
}

static auto center_point(Intersectable const* const i) -> std::array<double, 3>
{
    return i->get_min_extent() + 0.5 * i->get_max_extent();
}

static auto
spacial_average(std::vector<Intersectable const*> const& is, size_t axis)
    -> double
{
    double sum{0.0};
    for (auto const& i : is) {
        sum += center_point(i)[axis];
    }
    return sum / static_cast<double>(is.size());
}

// returns the left and right count on partition
static auto partition_axis(
    std::vector<Intersectable const*> const& is,
    double split_val,
    size_t axis
) -> std::tuple<size_t, size_t>
{
    size_t lcount{};
    size_t rcount{};
    for (auto const& i : is) {
        if (center_point(i)[axis] < split_val) {
            ++lcount;
        } else {
            ++rcount;
        }
    }
    return {lcount, rcount};
}

static auto best_partition_axis(std::vector<Intersectable const*> const& is)
    -> std::tuple<size_t, size_t, size_t, double>
{
    size_t min_abs_diff{SIZE_MAX};
    double best_spacial_average{0.0};
    size_t best_axis{0};
    size_t clcount{0};
    size_t crcount{0};
    for (size_t axis = 0; axis < 3; axis++) {
        double const current_spacial_average = spacial_average(is, axis);
        auto const [lcount, rcount] =
            partition_axis(is, current_spacial_average, axis);
        size_t const current_abs_diff =
            std::abs(static_cast<int>(rcount) - static_cast<int>(lcount));
        if (current_abs_diff < min_abs_diff) {
            min_abs_diff = current_abs_diff;
            best_axis = axis;
            clcount = lcount;
            crcount = rcount;
            best_spacial_average = current_spacial_average;
        }
    }
    return {best_axis, clcount, crcount, best_spacial_average};
}

auto BVHNode::construct_tree() -> void
{
    constexpr size_t min_primatives = 3;
    std::stack<BVHNode*> s{};
    s.push(this);
    while (!s.empty()) {
        BVHNode* cn = s.top();
        s.pop();
        auto const [best_axis, lcount, rcount, spacial_avg] =
            best_partition_axis(cn->primitives);
        if ((rcount + lcount) / 2 > min_primatives) {
            std::vector<Intersectable const*> lis{};
            std::vector<Intersectable const*> ris{};
            for (auto const& p : cn->primitives) {
                if (center_point(p)[best_axis] < spacial_avg) {
                    lis.push_back(p);
                } else {
                    ris.push_back(p);
                }
            }
            cn->children.push_back(std::make_unique<BVHNode>(lis));
            s.push(cn->children.back().get());
            cn->children.push_back(std::make_unique<BVHNode>(ris));
            s.push(cn->children.back().get());
            cn->primitives.clear();
        }
    }
}

auto BVHNode::intersect(Line const& line, Intersectable const* remove_ptr) const
    -> std::optional<intersection_t>
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
                        intersectable->intersect(line, remove_ptr);
                    if (intersection.has_value() &&
                        new_intersection.has_value()) {
                        if (std::get<0>(new_intersection.value()) <
                                std::get<0>(intersection.value()) &&
                            std::get<3>(new_intersection.value()) !=
                                remove_ptr) {
                            intersection = new_intersection;
                        }
                    } else if (!intersection.has_value() && new_intersection.has_value() &&
                               std::get<3>(new_intersection.value()) != remove_ptr)
                     {
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

auto BVHNode::get_max_extent() const -> std::array<double, 3>
{
    return volume.get_max_extent();
}

auto BVHNode::get_min_extent() const -> std::array<double, 3>
{
    return volume.get_min_extent();
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

auto BVHNode::print(std::ostream& os) const -> void
{
    size_t inode_count{0};
    size_t leaf_count{0};
    std::stack<BVHNode const*> s;
    s.push(this);
    while (!s.empty()) {
        BVHNode const* ptr = s.top();
        s.pop();
        if (ptr->children.size()) {
            for (auto const& child : ptr->children) {
                s.push(child.get());
                inode_count++;
            }
        } else {
            leaf_count++;
            os << ptr->primitives.size() << " ";
        }
    }
    os << "\nleave count: " << leaf_count << "\n";
    os << "internal node count: " << inode_count << "\n";
}
