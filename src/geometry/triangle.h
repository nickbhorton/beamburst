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
    std::array<double, 2> const* t0;
    std::array<double, 2> const* t1;
    std::array<double, 2> const* t2;
    double beta;
    double gamma;

public:
    Triangle(
        std::array<double, 3> const* p0,
        std::array<double, 3> const* p1,
        std::array<double, 3> const* p2
    );

    Triangle(
        std::array<double, 3> const* p0,
        std::array<double, 3> const* p1,
        std::array<double, 3> const* p2,
        std::array<double, 3> const* n0,
        std::array<double, 3> const* n1,
        std::array<double, 3> const* n2
    );

    Triangle(
        std::array<double, 3> const* p0,
        std::array<double, 3> const* p1,
        std::array<double, 3> const* p2,
        std::array<double, 3> const* n0,
        std::array<double, 3> const* n1,
        std::array<double, 3> const* n2,
        std::array<double, 2> const* t0,
        std::array<double, 2> const* t1,
        std::array<double, 2> const* t2
    );

    std::optional<double> find_intersection(const Line& line);

    std::array<double, 3>
    find_surface_normal(const std::array<double, 3>& solution_position);

    std::array<double, 2> find_uv(
        const std::array<double, 3>& solution_position,
        const std::array<double, 3>& solution_normal
    );

    auto get_p0() const -> std::array<double, 3>;
    auto get_p1() const -> std::array<double, 3>;
    auto get_p2() const -> std::array<double, 3>;
};

#endif
