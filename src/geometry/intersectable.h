#ifndef BEAMBURST_INTERSECTABLE_HEADER_
#define BEAMBURST_INTERSECTABLE_HEADER_

#include <optional>

#include "line.h"
class Intersectable
{

public:
    virtual std::optional<double> find_intersection(const Line& line) const = 0;
    virtual std::array<double, 3>
    find_surface_normal(const std::array<double, 3>& solution_position
    ) const = 0;
    virtual ~Intersectable() {}
};

#endif
