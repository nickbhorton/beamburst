#ifndef BEAMBURST_PLANE_HEADER_
#define BEAMBURST_PLANE_HEADER_

#include "intersectable.h"

class Plane : public Intersectable
{
    std::array<double, 3> position;
    std::array<double, 3> normal;

public:
    Plane(
        const std::array<double, 3>& position,
        const std::array<double, 3>& normal
    );

    std::optional<double> find_intersection(const Line& line);
    std::array<double, 3>
    find_surface_normal(const std::array<double, 3>& solution_position);
    std::array<double, 2> find_uv(
        const std::array<double, 3>& solution_position,
        const std::array<double, 3>& solution_normal
    );

    friend auto find_intersection(const Line& line, const Plane& plane)
        -> std::optional<double>;
};

#endif
