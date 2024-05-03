#ifndef BEAMBURST_TRIANGLE_HEADER_
#define BEAMBURST_TRIANGLE_HEADER_

#include "intersectable.h"
#include <array>

class Triangle : public Intersectable
{
    std::array<double, 3> const* p0;
    std::array<double, 3> const* p1;
    std::array<double, 3> const* p2;
    std::array<double, 3> const* n0;
    std::array<double, 3> const* n1;
    std::array<double, 3> const* n2;
    std::array<double, 3> const* t0;
    std::array<double, 3> const* t1;
    std::array<double, 3> const* t2;
    double beta;
    double gamma;

public:
    std::optional<double> find_intersection(const Line& line) const;
    std::array<double, 3>
    find_surface_normal(const std::array<double, 3>& solution_position) const;

    friend auto find_intersection(const Line& line, const Triangle& triangle)
        -> std::optional<double>;
};

#endif
