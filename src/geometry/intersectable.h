#ifndef BEAMBURST_INTERSECTABLE_HEADER_
#define BEAMBURST_INTERSECTABLE_HEADER_

#include <optional>

#include "line.h"
class Intersectable
{

public:
    virtual ~Intersectable() {}

    virtual auto find_intersection(Line const& line)
        -> std::optional<double> = 0;

    virtual auto
    find_surface_normal(std::array<double, 3> const& solution_position)
        -> std::array<double, 3> = 0;

    virtual auto find_uv(
        std::array<double, 3> const& solution_position,
        std::array<double, 3> const& solution_normal
    ) -> std::array<double, 2> = 0;
};

#endif
