#include "line.h"
#include "array_ops.h"

auto solve_line(Line const& line, double t) -> std::array<double, 3>
{
    return line.position + line.direction * t;
}

auto relected_direction(
    std::array<double, 3> const& incident,
    std::array<double, 3> const& normal
) -> std::array<double, 3>
{
    auto i = normalize(incident);
    auto n = normalize(normal);
    return i - 2.0 * n * dot(i, n);
}
