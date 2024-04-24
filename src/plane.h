#ifndef BEAMBURST_PLANE_HEADER_
#define BEAMBURST_PLANE_HEADER_

#include "linear_types.h"
#include <optional>

struct Line {
    linalg::vec3 position;
    linalg::vec3 direction;
};

struct Plane {
    linalg::vec3 position;
    linalg::vec3 normal;
};

constexpr auto find_intersection(const Line& line, const Plane& plane)
    -> std::optional<double>;

#endif
