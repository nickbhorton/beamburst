#include "intersectable.h"

auto intersect(
    std::vector<Intersectable*> const& is,
    Line const& line,
    Intersectable const* remove
) -> std::optional<intersection_t>
{
    std::optional<intersection_t> intersection{};
    for (auto const& intersectable : is) {
        std::optional<intersection_t> new_intersection =
            intersectable->intersect(line, remove);
        // skip if intersected in remove param
        if (new_intersection.has_value()) {
            if (std::get<3>(new_intersection.value()) == remove) {
                new_intersection = {};
            }
        }
        if (intersection.has_value() && new_intersection.has_value()) {
            if (std::get<0>(new_intersection.value()) <
                std::get<0>(intersection.value())) {
                intersection = new_intersection;
            }
        } else if (!intersection.has_value() && new_intersection.has_value()) {
            intersection = new_intersection;
        }
    }
    return intersection;
}
