#ifndef BEAMBURST_SPHERE_HEADER_
#define BEAMBURST_SPHERE_HEADER_

#include "intersectable.h"

class Sphere : public Intersectable
{
    std::array<double, 3> position;
    double radius;

public:
    Sphere(std::array<double, 3> const& position, double radius);

    auto intersect([[maybe_unused]] Line const& line) const
        -> std::optional<intersection_t>
    {
        return {};
    };

    auto find_intersection(Line const& line) -> std::optional<double>;

    auto find_surface_normal(std::array<double, 3> const& solution_position)
        -> std::array<double, 3>;
    auto find_uv(
        std::array<double, 3> const& solution_position,
        std::array<double, 3> const& solution_normal
    ) -> std::array<double, 2>;

    friend auto find_intersection(Line const& line, Sphere const& sphere)
        -> std::optional<double>;
};

#endif
