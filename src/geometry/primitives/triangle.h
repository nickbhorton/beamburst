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
        -> std::array<double, 2>;

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

    auto intersect(Line const& line, Float t_max) const
        -> std::optional<std::unique_ptr<SurfaceIntersection>>;
    auto get_max_extent() const -> std::array<double, 3>;
    auto get_min_extent() const -> std::array<double, 3>;
};

class TriangleSurfaceIntersection : public SurfaceIntersection
{
private:
    Triangle const* primative;
    Float t;
    std::array<Float, 3> position;
    std::array<std::array<Float, 3>, 3> btn_matrix;
    std::array<Float, 2> uv;

public:
    TriangleSurfaceIntersection(
        Triangle const* triangle_ptr,
        Float t,
        Line const& line,
        std::array<std::array<Float, 3>, 3> const& btn_matrix,
        std::array<Float, 2> uv
    );
    ~TriangleSurfaceIntersection() {}
    auto generate_ray(std::array<Float, 3> const& direction) const -> Line;
    auto get_position() const -> std::array<Float, 3>;
    auto get_uv() const -> std::array<Float, 2>;
    auto get_btn_matrix() const -> std::array<std::array<Float, 3>, 3>;
    auto get_t() const -> Float;
};
#endif
