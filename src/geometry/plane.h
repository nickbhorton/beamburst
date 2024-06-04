#ifndef BEAMBURST_PLANE_HEADER_
#define BEAMBURST_PLANE_HEADER_

#include "intersectable.h"

class Plane : public Intersectable
{
    std::array<double, 3> position;
    std::array<double, 3> normal;

public:
    Plane(
        std::array<double, 3> const& position,
        std::array<double, 3> const& normal
    );

    auto
    intersect(Line const& line, Intersectable const* remove_ptr = nullptr) const
        -> std::optional<intersection_t>;
    auto inside_intersect(
        Line const& line,
        Intersectable const* inside = nullptr
    ) const -> std::optional<intersection_t>;
    auto get_max_extent() const -> std::array<double, 3>;
    auto get_min_extent() const -> std::array<double, 3>;

    friend auto find_intersection(Line const& line, Plane const& plane)
        -> std::optional<double>;
};

#endif
