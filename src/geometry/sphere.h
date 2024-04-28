#ifndef BEAMBURST_SPHERE_HEADER_
#define BEAMBURST_SPHERE_HEADER_

#include <array>
#include <optional>

#include "line.h"

struct Sphere {
    std::array<double, 3> position;
    double radius;
    std::optional<double> find_intersection(const Line& line) const;
};

#endif
