#ifndef BEAMBURST_INTERSECTIONS_HEADER_
#define BEAMBURST_INTERSECTIONS_HEADER_

#include <optional>

#include "line.h"
#include "plane.h"

auto find_intersection(const Line& line, const Plane& plane)
    -> std::optional<double>;

#endif
