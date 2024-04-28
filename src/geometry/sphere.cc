#include "sphere.h"

#include "intersections.h"

std::optional<double> Sphere::find_intersection(const Line& line) const {
    return ::find_intersection(line, *this);
}
