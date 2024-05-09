#ifndef BEAMBURST_INTERSECTIONS_HEADER_
#define BEAMBURST_INTERSECTIONS_HEADER_

#include <optional>

#include "line.h"
#include "plane.h"
#include "sphere.h"

auto find_intersection(const Line& line, const Plane& plane)
    -> std::optional<double>;

auto find_intersection(const Line& line, const Sphere& sphere)
    -> std::optional<double>;

#endif
