#include <cassert>
#include <limits>
#include <memory>

#include "array_ops.h"
#include "intersectable.h"
#include "line.h"
#include "plane.h"

/// PlaneSurfaceIntersection
PlaneSurfaceIntersection::PlaneSurfaceIntersection(
    Plane const* plane_ptr,
    Float t,
    Line const& line
)
    : primative(plane_ptr), t(t), position(solve_line(line, t))
{
}

auto PlaneSurfaceIntersection::generate_ray(
    std::array<Float, 3> const& direction
) const -> Line
{
    float constexpr epsilon_multiplier = 5.0;
    return Line{
        .position = this->position + epsilon_multiplier *
                                         std::numeric_limits<Float>::epsilon() *
                                         direction,
        .direction = direction
    };
}

auto PlaneSurfaceIntersection::get_position() const -> std::array<Float, 3>
{
    return position;
}

// Im not aware of a way to make this make sense
// TODO: Somehow repeat a uv along tangent/bitangent direction
auto PlaneSurfaceIntersection::get_uv() const -> std::array<Float, 2>
{
    return {0, 0};
}

// TODO: make the tangent and bitangent vectors correct
auto PlaneSurfaceIntersection::get_btn_matrix() const
    -> std::array<std::array<Float, 3>, 3>
{
    std::array<std::array<Float, 3>, 3> returned{
        {{1, 0, 0}, {0, 1, 0}, primative->normal}
    };
    return transpose(returned);
}

auto PlaneSurfaceIntersection::get_t() const -> Float { return t; }

/// Plane
Plane::Plane(
    std::array<double, 3> const& position,
    std::array<double, 3> const& normal
)
    : position(position), normal(normal)
{
}

auto find_plane_intersection(Line const& line, Plane const& plane)
    -> std::optional<double>
{
    double const n_dot_d = dot(plane.normal, line.direction);
    if (!std::isnormal(n_dot_d)) {
        return {};
    }
    double const v1 = dot(plane.normal, plane.position);
    if (std::isnan(v1) || std::isinf(v1)) {
        return {};
    }
    double const v2 = dot(plane.normal, line.position);
    if (std::isnan(v2) || std::isinf(v2)) {
        return {};
    }
    double const result = (v1 - v2) / n_dot_d;
    if (result != 0.0 && std::signbit(result)) {
        return {};
    }
    return result;
}

auto Plane::intersect(Line const& line, Float t_max) const
    -> std::optional<std::unique_ptr<SurfaceIntersection>>
{
    std::optional<double> t_opt = find_plane_intersection(line, *this);
    if (!t_opt.has_value()) {
        return {};
    } else if (t_opt.value() > t_max) {
        return {};
    }
    std::unique_ptr<SurfaceIntersection> result =
        std::make_unique<PlaneSurfaceIntersection>(this, t_opt.value(), line);
    return result;
}

// I don't know how to deal with planes for bvhs at the moment
auto Plane::get_max_extent() const -> std::array<double, 3>
{
    assert(1 == 0);
    return {
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity()
    };
}
auto Plane::get_min_extent() const -> std::array<double, 3>
{
    assert(1 == 0);
    return {
        -std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity()
    };
}
