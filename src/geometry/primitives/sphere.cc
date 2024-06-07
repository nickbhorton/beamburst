#include "sphere.h"
#include "array_ops.h"
#include "line.h"

/// SphereSurfaceIntersection
SphereSurfaceIntersection::SphereSurfaceIntersection(
    Sphere const* sphere_ptr,
    Float t,
    Line const& line,
    std::array<std::array<Float, 3>, 3> const& btn_matrix,
    std::array<Float, 2> uv
)
    : primative(sphere_ptr), t(t), position(solve_line(line, t)),
      btn_matrix(btn_matrix), uv(uv)
{
}

auto SphereSurfaceIntersection::generate_ray(
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

auto SphereSurfaceIntersection::get_position() const -> std::array<Float, 3>
{
    return position;
};

auto SphereSurfaceIntersection::get_uv() const -> std::array<Float, 2>
{
    return uv;
}

auto SphereSurfaceIntersection::get_btn_matrix() const
    -> std::array<std::array<Float, 3>, 3>
{
    return btn_matrix;
}

auto SphereSurfaceIntersection::get_t() const -> Float { return t; }

/// Sphere
Sphere::Sphere(std::array<Float, 3> const& position, Float radius)
    : position(position), radius(radius)
{
}

auto find_sphere_intersection(Line const& line, Sphere const& sphere)
    -> std::optional<Float>
{
    Float const a = dot(line.direction, line.direction);
    Float const b = 2.0 * (dot(line.position, line.direction) -
                           dot(line.direction, sphere.position));
    Float const c = dot(line.position, line.position) +
                    dot(sphere.position, sphere.position) -
                    2.0 * dot(line.position, sphere.position) -
                    std::pow(sphere.radius, 2.0);
    Float const discriminant = b * b - 4.0 * a * c;
    if (std::isinf(discriminant) || std::signbit(discriminant) ||
        std::isnan(discriminant)) {
        return {};
    }
    if (discriminant == 0.0) {
        Float t = -b / (2.0 * a);
        if (!std::isinf(t) && !std::signbit(t) && !std::isnan(t)) {
            return t;
        } else {
            return {};
        }
    }
    Float const t1 = (-b + std::sqrt(discriminant)) / (2.0 * a);
    Float const t2 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    bool const valid_positive_t1 =
        !std::isinf(t1) && !std::signbit(t1) && !std::isnan(t1);
    bool const valid_positive_t2 =
        !std::isinf(t2) && !std::signbit(t2) && !std::isnan(t2);
    if (!valid_positive_t1 && !valid_positive_t2) {
        return {};
    } else if (valid_positive_t1 && !valid_positive_t2) {
        return t1;
    } else if (!valid_positive_t1 && valid_positive_t2) {
        return t2;
    } else {
        return std::min(t1, t2);
    }
}

auto Sphere::intersect(Line const& line, Float t_max) const
    -> std::optional<std::unique_ptr<SurfaceIntersection>>
{
    std::optional<Float> const t_opt = find_sphere_intersection(line, *this);
    if (!t_opt.has_value()) {
        return {};
    } else if (t_opt.value() > t_max) {
        return {};
    }
    auto const solution_poition =
        line.position + line.direction * t_opt.value();
    auto const solution_normal = normalize(solution_poition - position);
    std::array<Float, 3> const north_pole{0, 0, radius};
    auto const north_pole_dir = north_pole - solution_poition;
    auto const solution_tangent =
        normalize(cross(north_pole_dir, solution_normal));
    auto const solution_bitangent =
        normalize(cross(solution_normal, solution_tangent));
    std::array<std::array<Float, 3>, 3> const normal_coords{
        {solution_tangent, solution_bitangent, solution_normal}
    };

    Float const u =
        (std::atan2(solution_normal[1], solution_normal[0]) + M_PI) /
        (2.0f * M_PI);
    Float const v = std::acos(solution_normal[2]) / M_PI;
    std::array<Float, 2> const uv = {u, v};

    std::unique_ptr<SurfaceIntersection> result =
        std::make_unique<SphereSurfaceIntersection>(
            this,
            t_opt.value(),
            line,
            transpose(normal_coords),
            uv
        );
    return result;
}

auto Sphere::get_max_extent() const -> std::array<Float, 3>
{
    return {position[0] + radius, position[1] + radius, position[2] + radius};
}
auto Sphere::get_min_extent() const -> std::array<Float, 3>
{
    return {position[0] - radius, position[1] - radius, position[2] - radius};
}
