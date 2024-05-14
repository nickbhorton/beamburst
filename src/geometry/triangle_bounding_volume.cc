#include <limits>
#include <utility>

#include "triangle.h"
#include "triangle_bounding_volume.h"

TriangleBoundingVolume::TriangleBoundingVolume(std::vector<Triangle> triangles)
    : triangles(std::move(triangles)), bounding_volume(
                                           {std::numeric_limits<double>::max(),
                                            std::numeric_limits<double>::max(),
                                            std::numeric_limits<double>::max()},
                                           {std::numeric_limits<double>::min(),
                                            std::numeric_limits<double>::min(),
                                            std::numeric_limits<double>::min()}
                                       ),
      previous_intersection(nullptr)
{
    for (auto const& t : this->triangles) {
        for (size_t i = 0; i < 3; i++) {
            if (t.get_p0()[i] < bounding_volume.min_point[i]) {
                bounding_volume.min_point[i] = t.get_p0()[i];
            }
            if (t.get_p1()[i] < bounding_volume.min_point[i]) {
                bounding_volume.min_point[i] = t.get_p1()[i];
            }
            if (t.get_p2()[i] < bounding_volume.min_point[i]) {
                bounding_volume.min_point[i] = t.get_p2()[i];
            }

            if (t.get_p0()[i] > bounding_volume.max_point[i]) {
                bounding_volume.max_point[i] = t.get_p0()[i];
            }
            if (t.get_p1()[i] > bounding_volume.max_point[i]) {
                bounding_volume.max_point[i] = t.get_p1()[i];
            }
            if (t.get_p2()[i] > bounding_volume.max_point[i]) {
                bounding_volume.max_point[i] = t.get_p2()[i];
            }
        }
    }
}

auto TriangleBoundingVolume::intersect(Line const& line) const
    -> std::optional<intersection_t>
{
    if (bounding_volume.intersect(line).has_value()) {
        std::optional<intersection_t> intersection{};
        for (auto tri : triangles) {
            std::optional<intersection_t> const new_intersection =
                tri.intersect(line);
            if (intersection.has_value() && new_intersection.has_value()) {
                if (std::get<0>(new_intersection.value()) <
                    std::get<0>(intersection.value())) {
                    intersection = new_intersection;
                }
            } else if (!intersection.has_value() && new_intersection.has_value()) {
                intersection = new_intersection;
            }
        }
        return intersection;
    }
    return {};
}

auto TriangleBoundingVolume::get_max_extent() const -> std::array<double, 3>
{
    return bounding_volume.max_point;
}
auto TriangleBoundingVolume::get_min_extent() const -> std::array<double, 3>
{
    return bounding_volume.min_point;
}
