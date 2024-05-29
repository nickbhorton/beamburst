#ifndef BEAMBURST_INTERSECTABLE_HEADER_
#define BEAMBURST_INTERSECTABLE_HEADER_

#include <optional>
#include <tuple>
#include <vector>

#include "line.h"

typedef std::array<std::array<double, 3>, 3> normal_coords_t;
typedef std::array<double, 2> uv_t;

class Intersectable;
typedef std::tuple<double, normal_coords_t, std::optional<uv_t>, Intersectable const*>
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
    virtual auto
    intersect(Line const& line, Intersectable const* remove_ptr = nullptr) const
        -> std::optional<intersection_t> = 0;

    virtual auto get_max_extent() const -> std::array<double, 3> = 0;
    virtual auto get_min_extent() const -> std::array<double, 3> = 0;
};

auto intersect_group(
    std::vector<Intersectable*> const& is,
    Line const& line,
    Intersectable const* remove = nullptr
) -> std::optional<intersection_t>;

#endif
