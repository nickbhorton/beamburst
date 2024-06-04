#ifndef BEAMBURST_SPHERE_HEADER_
#define BEAMBURST_SPHERE_HEADER_

#include "intersectable.h"

class Sphere : public Intersectable
{
    std::array<double, 3> position;
    double radius;

public:
    Sphere(std::array<double, 3> const& position, double radius);

    auto
    intersect(Line const& line, Intersectable const* remove_ptr = nullptr) const
        -> std::optional<intersection_t>;
    auto inside_intersect(Line const& line) const
        -> std::optional<intersection_t>;
    auto get_max_extent() const -> std::array<double, 3>;
    auto get_min_extent() const -> std::array<double, 3>;

    friend auto find_intersection(Line const& line, Sphere const& sphere)
        -> std::optional<double>;
    friend auto find_both_intersections(Line const& line, Sphere const& sphere)
        -> std::tuple<std::optional<double>, std::optional<double>>;
};

#endif
