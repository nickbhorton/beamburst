#ifndef BEAMBURST_SPHERE_HEADER_
#define BEAMBURST_SPHERE_HEADER_

#include "intersectable.h"

class Sphere : public Intersectable
{
    std::array<double, 3> position;
    double radius;

public:
    Sphere(const std::array<double, 3>& position, double radius);

    std::optional<double> find_intersection(const Line& line);
    std::array<double, 3>
    find_surface_normal(const std::array<double, 3>& solution_position);

    friend auto find_intersection(const Line& line, const Sphere& sphere)
        -> std::optional<double>;
};

#endif
