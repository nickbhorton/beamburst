#include "plane.h"
#include "intersections.h"

Plane::Plane(
    std::array<double, 3> const& position,
    std::array<double, 3> const& normal
)
    : position(position), normal(normal)
{
}

auto Plane::intersect(Line const& line) const -> std::optional<intersection_t>
{
    std::optional<double> t_opt = ::find_intersection(line, *this);
    if (!t_opt.has_value()) {
        return {};
    }
    intersection_t result = {t_opt.value(), normal, {}};
    return result;
}
