#ifndef BEAMBURST_SPHERE_HEADER_
#define BEAMBURST_SPHERE_HEADER_

#include "intersectable.h"

class Sphere : public Intersectable
{
    std::array<double, 3> position;
    double radius;

public:
    Sphere(std::array<double, 3> const& position, double radius);

    auto find_intersection(const Line& line) -> std::optional<double>;

    auto find_surface_normal(const std::array<double, 3>& solution_position)
        -> std::array<double, 3>;
    auto find_uv(
        std::array<double, 3> const& solution_position,
        std::array<double, 3> const& solution_normal
    ) -> std::array<double, 2>;

    friend auto find_intersection(Line const& line, Sphere const& sphere)
        -> std::optional<double>;
};

#endif
