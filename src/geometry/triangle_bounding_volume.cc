#include "triangle_bounding_volume.h"
#include "triangle.h"
#include <algorithm>
#include <limits>
#include <utility>

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

auto TriangleBoundingVolume::find_intersection(Line const& line)
    -> std::optional<double>
{
    auto const t_opt = bounding_volume.find_intersection(line);
    if (!t_opt.has_value()) {
        return {};
    }
    return t_opt.value();

    std::vector<std::tuple<double, Triangle*>> ts;
    auto const intersect_line = [&line, &ts](Triangle& i) {
        auto const t_opt = i.find_intersection(line);
        if (t_opt) {
            ts.push_back({t_opt.value(), &i});
        }
    };

    std::ranges::for_each(triangles, intersect_line);

    typedef std::tuple<double, Triangle*> tri_intersect_t;
    struct {
        bool operator()(tri_intersect_t e1, tri_intersect_t e2) const
        {
            return std::get<0>(e1) < std::get<0>(e2);
        }
    } FirstTupleLess;

    if (ts.size()) {
        auto const min = std::ranges::min_element(ts, FirstTupleLess);
        previous_intersection = std::get<1>(*min);
        return std::get<0>(*min);
    }
    return {};
}

auto TriangleBoundingVolume::find_surface_normal(
    std::array<double, 3> const& solution_position
) -> std::array<double, 3>
{
    if (previous_intersection == nullptr) {
        return bounding_volume.find_surface_normal(solution_position);
    }
    return previous_intersection->find_surface_normal(solution_position);
}

auto TriangleBoundingVolume::find_uv(
    std::array<double, 3> const& solution_position,
    std::array<double, 3> const& solution_normal
) -> std::array<double, 2>
{
    if (previous_intersection == nullptr) {
        return bounding_volume.find_uv(solution_position, solution_normal);
    }
    return previous_intersection->find_uv(solution_position, solution_normal);
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
