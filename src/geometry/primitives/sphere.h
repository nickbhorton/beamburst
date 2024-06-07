#ifndef BEAMBURST_SPHERE_HEADER_
#define BEAMBURST_SPHERE_HEADER_

#include "intersectable.h"

class Sphere : public Intersectable
{
    std::array<Float, 3> position;
    Float radius;

public:
    Sphere(std::array<Float, 3> const& position, Float radius);

    auto intersect(Line const& line, Float t_max) const
        -> std::optional<std::unique_ptr<SurfaceIntersection>>;

    auto get_max_extent() const -> std::array<Float, 3>;
    auto get_min_extent() const -> std::array<Float, 3>;

    friend auto find_sphere_intersection(Line const& line, Sphere const& sphere)
        -> std::optional<Float>;
};

class SphereSurfaceIntersection : public SurfaceIntersection
{
private:
    Sphere const* primative;
    Float t;
    std::array<Float, 3> position;
    std::array<std::array<Float, 3>, 3> btn_matrix;
    std::array<Float, 2> uv;

public:
    SphereSurfaceIntersection(
        Sphere const* sphere_ptr,
        Float t,
        Line const& line,
        std::array<std::array<Float, 3>, 3> const& btn_matrix,
        std::array<Float, 2> uv
    );
    ~SphereSurfaceIntersection() {}
    auto generate_ray(std::array<Float, 3> const& direction) const -> Line;
    auto get_position() const -> std::array<Float, 3>;
    auto get_uv() const -> std::array<Float, 2>;
    auto get_btn_matrix() const -> std::array<std::array<Float, 3>, 3>;
    auto get_t() const -> Float;
};

#endif
