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

    auto find_intersection(Line const& line) -> std::optional<double>;

    auto find_surface_normal(std::array<double, 3> const& solution_position)
        -> std::array<double, 3>;
    auto find_uv(
        std::array<double, 3> const& solution_position,
        std::array<double, 3> const& solution_normal
    ) -> std::array<double, 2>;

    friend auto find_intersection(Line const& line, Plane const& plane)
        -> std::optional<double>;
};

#endif
