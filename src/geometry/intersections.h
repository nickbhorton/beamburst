#ifndef BEAMBURST_INTERSECTIONS_HEADER_
#define BEAMBURST_INTERSECTIONS_HEADER_

#include <optional>

#include "line.h"
#include "plane.h"
#include "sphere.h"

auto find_intersection(Line const& line, Plane const& plane)
    -> std::optional<double>;

auto find_intersection(Line const& line, Sphere const& sphere)
    -> std::optional<double>;

#endif
