#include "triangle.h"
#include "array_ops.h"
#include "intersectable.h"
#include "line.h"
#include <optional>

TriangleSurfaceIntersection::TriangleSurfaceIntersection(
    Triangle const* triangle_ptr,
    Float t,
    Line const& line,
    std::array<std::array<Float, 3>, 3> const& btn_matrix,
    std::array<Float, 2> uv
)
    : primative(triangle_ptr), t(t), position(solve_line(line, t)),
      btn_matrix(btn_matrix), uv(uv)
{
}

auto TriangleSurfaceIntersection::generate_ray(
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

auto TriangleSurfaceIntersection::get_position() const -> std::array<Float, 3>
{
    return position;
}

auto TriangleSurfaceIntersection::get_uv() const -> std::array<Float, 2>
{
    return uv;
}

auto TriangleSurfaceIntersection::get_btn_matrix() const
    -> std::array<std::array<Float, 3>, 3>
{
    return btn_matrix;
}

auto TriangleSurfaceIntersection::get_t() const -> Float { return t; }

Triangle::Triangle(
    std::array<Float, 3> const* p0,
    std::array<Float, 3> const* p1,
    std::array<Float, 3> const* p2
)
    : p0{p0}, p1{p1}, p2{p2}, n0{nullptr}, n1{nullptr}, n2{nullptr},
      t0{nullptr}, t1{nullptr}
{
}
Triangle::Triangle(
    std::array<Float, 3> const* p0,
    std::array<Float, 3> const* p1,
    std::array<Float, 3> const* p2,
    std::array<Float, 3> const* n0,
    std::array<Float, 3> const* n1,
    std::array<Float, 3> const* n2
)
    : p0{p0}, p1{p1}, p2{p2}, n0{n0}, n1{n1}, n2{n2}, t0{nullptr}, t1{nullptr}
{
}

Triangle::Triangle(
    std::array<Float, 3> const* p0,
    std::array<Float, 3> const* p1,
    std::array<Float, 3> const* p2,
    std::array<Float, 3> const* n0,
    std::array<Float, 3> const* n1,
    std::array<Float, 3> const* n2,
    std::array<Float, 2> const* t0,
    std::array<Float, 2> const* t1,
    std::array<Float, 2> const* t2
)
    : p0{p0}, p1{p1}, p2{p2}, n0{n0}, n1{n1}, n2{n2}, t0{t0}, t1{t1}, t2{t2}
{
}

auto Triangle::compute_tabc(Line const& line) const
    -> std::optional<std::tuple<Float, Float, Float, Float>>
{
    std::array<Float, 3> const e1 = *p1 - *p0;
    std::array<Float, 3> const e2 = *p2 - *p0;
    std::array<Float, 3> const n = cross(e1, e2);
    Float const D = -dot(*p0, n);
    Float const denominator = dot(n, line.direction);
    if (!std::isnormal(denominator)) {
        return {};
    }
    Float const time = -(D + dot(n, line.position)) / denominator;
    if (std::signbit(time)) {
        return {};
    }
    std::array<Float, 3> const solution_position =
        line.position + (time * line.direction);
    std::array<Float, 3> const ep = solution_position - *p0;
    Float const d11 = dot(e1, e1);
    Float const d12 = dot(e1, e2);
    Float const d22 = dot(e2, e2);
    Float const d1p = dot(e1, ep);
    Float const d2p = dot(e2, ep);
    Float const det = d11 * d22 - d12 * d12;
    if (!std::isnormal(det)) {
        return {};
    }
    Float const beta = (d22 * d1p - d12 * d2p) / det;
    Float const gamma = (d11 * d2p - d12 * d1p) / det;
    Float const alpha = 1 - beta - gamma;
    if (beta < 0.0 || beta > 1.0 || gamma < 0.0 || gamma > 1.0 ||
        beta + gamma > 1.0 || beta + gamma < 0.0) {
        return {};
    }
    std::tuple<Float, Float, Float, Float> const result =
        {time, alpha, beta, gamma};
    return result;
}

auto Triangle::calculate_surface_normal(
    std::array<Float, 3> const& solution_position,
    Float alpha,
    Float beta,
    Float gamma
) const
{
    if (n0 != nullptr && n1 != nullptr && n2 != nullptr) {
        return normalize(((alpha * *n0) + (beta * *n1) + (gamma * *n2)));
    }
    return normalize(cross((solution_position - *p0), (*p2 - *p0)));
}

auto Triangle::calculate_uv(Float alpha, Float beta, Float gamma) const
    -> std::array<Float, 2>
{
    if (t0 != nullptr && t1 != nullptr && t2 != nullptr) {
        Float u = alpha * (*t0)[0] + beta * (*t1)[0] + gamma * (*t2)[0];
        Float v = alpha * (*t0)[1] + beta * (*t1)[1] + gamma * (*t2)[1];
        return std::array<Float, 2>({u, v});
    }
    return {0, 0};
}

auto Triangle::intersect(Line const& line, Float t_max) const
    -> std::optional<std::unique_ptr<SurfaceIntersection>>
{
    auto const opt = compute_tabc(line);
    if (opt.has_value()) {
        auto const& [t, alpha, beta, gamma] = opt.value();
        if (t > t_max) {
            return {};
        }
        auto const solution_position = line.position + line.direction * t;
        std::array<Float, 3> const surface_normal =
            calculate_surface_normal(solution_position, alpha, beta, gamma);
        std::array<std::array<Float, 3>, 3> tangent_space{};
        // obviously textures will not work if there are not texture coords for
        // each point. If we cant get proper tangent and bitangent values x and
        // y are better than nothing. Maybe? It could be bettwe to use a unit
        // matrix. SKETCHY
        if (t0 == nullptr || t1 == nullptr || t2 == nullptr) {
            tangent_space = {{{1, 0, 0}, {0, 1, 0}, surface_normal}};
        } else {
            auto const edge1 = *p1 - *p0;
            auto const edge2 = *p2 - *p0;
            auto const delta_uv_1 = *t1 - *t0;
            auto const delta_uv_2 = *t2 - *t0;
            Float const division_factor = 1.0 / (delta_uv_1[0] * delta_uv_2[1] -
                                                 delta_uv_2[0] * delta_uv_1[1]);
            std::array<Float, 3> const tangent{
                division_factor *
                    (delta_uv_2[1] * edge1[0] - delta_uv_1[1] * edge2[0]),
                division_factor *
                    (delta_uv_2[1] * edge1[1] - delta_uv_1[1] * edge2[1]),
                division_factor *
                    (delta_uv_2[1] * edge1[2] - delta_uv_1[1] * edge2[2])
            };

            std::array<Float, 3> const bitangent{
                division_factor *
                    (delta_uv_2[0] * edge1[0] - delta_uv_1[0] * edge2[0]),
                division_factor *
                    (delta_uv_2[0] * edge1[1] - delta_uv_1[0] * edge2[1]),
                division_factor *
                    (delta_uv_2[0] * edge1[2] - delta_uv_1[0] * edge2[2])
            };
            tangent_space = {{tangent, bitangent, surface_normal}};
        }
        auto const uv = calculate_uv(alpha, beta, gamma);

        std::unique_ptr<SurfaceIntersection> result =
            std::make_unique<TriangleSurfaceIntersection>(
                this,
                t,
                line,
                transpose(tangent_space),
                uv
            );
        return result;
    }
    return {};
}

auto Triangle::get_max_extent() const -> std::array<Float, 3>
{
    return {
        std::max(std::max((*p0)[0], (*p1)[0]), (*p2)[0]),
        std::max(std::max((*p0)[1], (*p1)[1]), (*p2)[1]),
        std::max(std::max((*p0)[2], (*p1)[2]), (*p2)[2]),
    };
}
auto Triangle::get_min_extent() const -> std::array<Float, 3>
{
    return {
        std::min(std::min((*p0)[0], (*p1)[0]), (*p2)[0]),
        std::min(std::min((*p0)[1], (*p1)[1]), (*p2)[1]),
        std::min(std::min((*p0)[2], (*p1)[2]), (*p2)[2]),
    };
}
