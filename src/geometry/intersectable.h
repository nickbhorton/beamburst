#ifndef BEAMBURST_INTERSECTABLE_HEADER_
#define BEAMBURST_INTERSECTABLE_HEADER_

#include <memory>
#include <optional>

#include "line.h"

typedef double Float;

class SurfaceIntersection
{
public:
    virtual ~SurfaceIntersection() {}
    // Ensures that round off errors do not cause intersections with this
    // surface again.
    virtual auto generate_ray(std::array<Float, 3> const& direction) const
        -> Line = 0;
    virtual auto get_position() const -> std::array<Float, 3> = 0;
    virtual auto get_uv() const -> std::array<Float, 2> = 0;
    virtual auto get_btn_matrix() const
        -> std::array<std::array<Float, 3>, 3> = 0;
    virtual auto get_t() const -> Float = 0;
};

class Intersectable
{
public:
    virtual ~Intersectable() {}

    virtual auto intersect(Line const& line, Float max_t) const
        -> std::optional<std::unique_ptr<SurfaceIntersection>> = 0;
    virtual auto get_max_extent() const -> std::array<Float, 3> = 0;
    virtual auto get_min_extent() const -> std::array<Float, 3> = 0;
};

#endif
