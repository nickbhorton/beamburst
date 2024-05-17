#ifndef BEAMBURST_TRIANGLE_HEADER_
#define BEAMBURST_TRIANGLE_HEADER_

#include "intersectable.h"

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

    auto compute_tabc(Line const& line) const
        -> std::optional<std::tuple<double, double, double, double>>;
    auto calculate_surface_normal(
        std::array<double, 3> const& solution_position,
        double alpha,
        double beta,
        double gamma
    ) const;
    auto calculate_uv(double alpha, double beta, double gamma) const
        -> std::optional<std::array<double, 2>>;

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

    auto
    intersect(Line const& line, Intersectable const* remove_ptr = nullptr) const
        -> std::optional<intersection_t>;
    auto get_max_extent() const -> std::array<double, 3>;
    auto get_min_extent() const -> std::array<double, 3>;

    auto get_p0() const -> std::array<double, 3>;
    auto get_p1() const -> std::array<double, 3>;
    auto get_p2() const -> std::array<double, 3>;
};

#endif
