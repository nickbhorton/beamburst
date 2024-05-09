#ifndef BEAMBURST_INTERSECTABLE_HEADER_
#define BEAMBURST_INTERSECTABLE_HEADER_

#include <optional>

#include "line.h"
class Intersectable
{

public:
    virtual ~Intersectable() {}

    virtual std::optional<double> find_intersection(const Line& line) = 0;

    virtual std::array<double, 3>
    find_surface_normal(const std::array<double, 3>& solution_position) = 0;

    virtual std::array<double, 2> find_uv(
        const std::array<double, 3>& solution_position,
        const std::array<double, 3>& solution_normal
    ) = 0;
};

#endif
