#ifndef BEAMBURST_PLANE_HEADER_
#define BEAMBURST_PLANE_HEADER_

#include "intersectable.h"

class Plane : public Intersectable
{
    std::array<double, 3> position;
    std::array<double, 3> normal;

public:
    Plane(
        std::array<double, 3> const& position,
        std::array<double, 3> const& normal
    );

    auto intersect(Line const& line) const -> std::optional<intersection_t>;

    friend auto find_intersection(Line const& line, Plane const& plane)
        -> std::optional<double>;
};

#endif
