#ifndef BEAMBURST_PLANE_HEADER_
#define BEAMBURST_PLANE_HEADER_

#include "intersectable.h"

class Plane : public Intersectable
{
public:
    std::array<Float, 3> position;
    std::array<Float, 3> normal;
    Plane(
        std::array<Float, 3> const& position,
        std::array<Float, 3> const& normal
    );

    auto intersect(Line const& line, Float t_max) const
        -> std::optional<std::unique_ptr<SurfaceIntersection>>;
    auto get_max_extent() const -> std::array<Float, 3>;
    auto get_min_extent() const -> std::array<Float, 3>;

    friend auto find_plane_intersection(Line const& line, Plane const& plane)
        -> std::optional<Float>;
};

class PlaneSurfaceIntersection : public SurfaceIntersection
{
private:
    Plane const* primative;
    Float t;
    std::array<Float, 3> position;

public:
    PlaneSurfaceIntersection(Plane const* plane_ptr, Float t, Line const& line);
    ~PlaneSurfaceIntersection() {}
    auto generate_ray(std::array<Float, 3> const& direction) const -> Line;
    auto get_position() const -> std::array<Float, 3>;
    auto get_uv() const -> std::array<Float, 2>;
    auto get_btn_matrix() const -> std::array<std::array<Float, 3>, 3>;
    auto get_t() const -> Float;
};

#endif
