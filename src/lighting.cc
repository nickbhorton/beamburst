#include "lighting.h"
#include "array_ops.h"

auto blin_phong(
    const std::array<double, 3>& light_position,
    const std::array<double, 3>& position,
    const std::array<double, 3>& view_direction,
    const std::array<double, 3>& normal,
    double specular_hardness
) -> std::tuple<double, double>
{
    const std::array<double, 3> light_direction =
        normalize(light_position - position);
    const std::array<double, 3> halfway =
        normalize(light_direction + view_direction);
    double specular =
        std::pow(std::max(dot(normal, halfway), 0.0), specular_hardness);
    double diffuse = dot(light_direction, normalize(normal));
    return {diffuse, specular};
}
