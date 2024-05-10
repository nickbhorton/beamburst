#ifndef BEAMBURST_AABB_HEADER_
#define BEAMBURST_AABB_HEADER_

#include "intersectable.h"

class AABB : public Intersectable
{
    std::array<double, 3> min_point;
    std::array<double, 3> max_point;

public:
    AABB(
        std::array<double, 3> const& min_point,
        std::array<double, 3> const& max_point
    );

    auto find_intersection(Line const& line) -> std::optional<double>;
    auto find_surface_normal(std::array<double, 3> const& solution_position)
        -> std::array<double, 3>;
    auto find_uv(
        std::array<double, 3> const& solution_position,
        std::array<double, 3> const& solution_normal
    ) -> std::array<double, 2>;
};

#endif
