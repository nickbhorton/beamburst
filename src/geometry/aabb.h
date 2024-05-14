#ifndef BEAMBURST_AABB_HEADER_
#define BEAMBURST_AABB_HEADER_

#include "intersectable.h"

class AABB : public Intersectable
{
public:
    std::array<double, 3> min_point;
    std::array<double, 3> max_point;

    AABB(
        std::array<double, 3> const& min_point,
        std::array<double, 3> const& max_point
    );

    auto intersect(Line const& line) const -> std::optional<intersection_t>;
    auto test_intersect(Line const& line) const -> bool;
};

#endif
