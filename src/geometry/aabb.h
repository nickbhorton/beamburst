#ifndef BEAMBURST_AABB_HEADER_
#define BEAMBURST_AABB_HEADER_

#include "line.h"

#include <array>

class AABB
{
public:
    std::array<double, 3> min_point;
    std::array<double, 3> max_point;

    AABB(
        std::array<double, 3> const& min_point,
        std::array<double, 3> const& max_point
    );

    auto test_intersect(Line const& line) const -> bool;
    auto get_max_extent() const -> std::array<double, 3>;
    auto get_min_extent() const -> std::array<double, 3>;
};

#endif
