#ifndef BEAMBURST_AABB_HEADER_
#define BEAMBURST_AABB_HEADER_

#include "intersectable.h"

// TODO: see if this needs to be inheriting from Intersectable
class AABB : public Intersectable
{
public:
    std::array<double, 3> min_point;
    std::array<double, 3> max_point;

    AABB(
        std::array<double, 3> const& min_point,
        std::array<double, 3> const& max_point
    );

    auto
    intersect(Line const& line, Intersectable const* remove_ptr = nullptr) const
        -> std::optional<intersection_t>;
    auto inside_intersect(
        Line const& line,
        Intersectable const* inside = nullptr
    ) const -> std::optional<intersection_t>;
    auto test_intersect(Line const& line) const -> bool;
    auto get_max_extent() const -> std::array<double, 3>;
    auto get_min_extent() const -> std::array<double, 3>;
};

#endif
