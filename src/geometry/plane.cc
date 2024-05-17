#include "plane.h"
#include "intersections.h"
#include <cassert>
#include <limits>

Plane::Plane(
    std::array<double, 3> const& position,
    std::array<double, 3> const& normal
)
    : position(position), normal(normal)
{
}

auto Plane::intersect(
    Line const& line,
    [[maybe_unused]] Intersectable const* remove_ptr
) const -> std::optional<intersection_t>
{
    std::optional<double> t_opt = ::find_intersection(line, *this);
    if (!t_opt.has_value()) {
        return {};
    }
    intersection_t result = {t_opt.value(), normal, {}, this};
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
