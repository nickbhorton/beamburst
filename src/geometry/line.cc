#include "line.h"
#include "array_ops.h"
#include <cmath>

auto solve_line(Line const& line, double t) -> std::array<double, 3>
{
    return line.position + line.direction * t;
}

auto reflected_direction(
    std::array<double, 3> const& incident,
    std::array<double, 3> const& normal
) -> std::array<double, 3>
{
    auto i = normalize(incident);
    auto n = normalize(normal);
    return i - 2.0 * n * dot(i, n);
}

auto refracted_direction(
    std::array<double, 3> const& incident,
    std::array<double, 3> const& normal,
    double ni,
    double nf
) -> std::array<double, 3>
{
    double const n = ni / nf;
    auto const I = -incident;
    auto const N = dot(normal, I) > 0.0 ? normal : -normal;
    double const cos_theta = dot(N, I);
    double const coeff =
        std::sqrt(1 - std::pow(n, 2) * (1.0 - std::pow(cos_theta, 2)));

    if (!std::isnormal(coeff)) {
        return reflected_direction(incident, normal);
    }

    return coeff * (-N) + n * (cos_theta * N - I);
}
