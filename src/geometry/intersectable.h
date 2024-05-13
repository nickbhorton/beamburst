#ifndef BEAMBURST_INTERSECTABLE_HEADER_
#define BEAMBURST_INTERSECTABLE_HEADER_

#include <optional>
#include <tuple>

#include "line.h"

typedef std::
    tuple<double, std::array<double, 3>, std::optional<std::array<double, 2>>>
        intersection_t;

class Intersectable
{
public:
    virtual ~Intersectable() {}

    // The three functions below are not required to be const but several
    // objects use specific numbers that are expesive to calculate multiple
    // times. So there are 3 options.
    // 1. Compute the special numbers every time. Slow
    // 2. Cache the numbers in objet state. Violates constness
    // 3. Compute all wanted numbers in one function. Maybe we dont want
    // everything.
    // This function is my attempt at option 3 and the the other functions
    // are option 2.
    virtual auto intersect(Line const& line) const
        -> std::optional<intersection_t> = 0;

    virtual auto find_intersection(Line const& line)
        -> std::optional<double> = 0;

    virtual auto
    find_surface_normal(std::array<double, 3> const& solution_position)
        -> std::array<double, 3> = 0;

    virtual auto find_uv(
        std::array<double, 3> const& solution_position,
        std::array<double, 3> const& solution_normal
    ) -> std::array<double, 2> = 0;
};

#endif
