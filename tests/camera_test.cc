#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "coordinates.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "plane.h"
#include "sphere.h"
#include "vector_ops.h"

#include <algorithm>
#include <limits>
#include <utility>

using namespace linalg;
typedef std::tuple<
    std::size_t,
    std::size_t,
    vec3,
    vec3,
    vec3,
    Intersectable const* const>
    pixel_job_t;
typedef std::tuple<double, Intersectable*> intersection_t;

struct {
    bool operator()(intersection_t e1, intersection_t e2) const
    {
        return std::get<0>(e1) < std::get<0>(e2);
    }
} IntersectionLess;

int main()
{
    constexpr double axis_distance = 2.3;
    constexpr Screen screen{.discretization = {512, 512}, .size = {1.0, 1.0}};

    std::vector<Camera> cameras = {};
    cameras.push_back(axis_aligned_camera_nx(screen, axis_distance));
    cameras.push_back(axis_aligned_camera_ny(screen, axis_distance));
    cameras.push_back(axis_aligned_camera_nz(screen, axis_distance));
    cameras.push_back(axis_aligned_camera_px(screen, axis_distance));
    cameras.push_back(axis_aligned_camera_py(screen, axis_distance));
    cameras.push_back(axis_aligned_camera_pz(screen, axis_distance));

    std::vector<std::unique_ptr<Intersectable>> intersectables{};
    intersectables.push_back(
        std::make_unique<Sphere>(Sphere({0.0, 0.0, 0.0}, 1.0))
    );
    /*
    intersectables.push_back(
        std::make_unique<Sphere>(Sphere({-1.0, 0.0, 0.0}, 0.5))
    );
    intersectables.push_back(
        std::make_unique<Sphere>(Sphere({0.0, 0.0, 1.0}, 0.5))
    );
    intersectables.push_back(
        std::make_unique<Sphere>(Sphere({0.0, 0.0, -1.0}, 0.5))
    );
    intersectables.push_back(
        std::make_unique<Plane>(Plane({0.0, -1.0, 0.0}, {0.0, 1.0, 0.0}))
    );
    */

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    std::vector<std::vector<pixel_job_t>> camera_jobs{};
    for (auto& camera : cameras) {
        std::vector<pixel_job_t> pixel_jobs{};
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                std::vector<intersection_t> ts{};
                const Line line = camera.get_line_at(x, y);
                auto const intersect_line =
                    [&line, &ts](std::unique_ptr<Intersectable> const& i) {
                        auto t_opt = i->find_intersection(line);
                        if (t_opt) {
                            ts.push_back({t_opt.value(), i.get()});
                        }
                    };
                std::ranges::for_each(
                    std::as_const(intersectables),
                    intersect_line
                );

                if (ts.size()) {
                    std::sort(ts.begin(), ts.end(), IntersectionLess);
                    auto const& [t, intersectable_p] = ts.front();
                    const vec3 solution_position =
                        camera.get_position() + t * line.direction;
                    const vec3 solution_normal =
                        intersectable_p->find_surface_normal(solution_position);
                    pixel_jobs.push_back(
                        {x,
                         y,
                         solution_position,
                         solution_normal,
                         camera.get_position(),
                         intersectable_p}
                    );
                }
            }
        }
        camera_jobs.push_back(pixel_jobs);
    }
    Color bg_color{0, 0, 0, 64};
    Image img1{
        {screen.get_horizontal_discretization() * cameras.size() / 2,
         screen.get_vertical_discretization() * cameras.size() / 3},
        bg_color
    };

    size_t offset_x = 0;
    size_t offset_y = 0;
    PointLight light1({0.0, 5, 0.0});
    for (const auto& pixel_jobs : camera_jobs) {
        for (const auto& [x, y, position, normal, camera_position, intersectable_p] :
             pixel_jobs) {
            const vec3 view = camera_position - position;
            // const vec3 spherical_normal = cartesian_to_sphere_uv(normal);
            // const double longitude = spherical_normal[1];
            // const double latitude = spherical_normal[2];
            const double diffuse =
                phong_diffuse(light1.position, position, normal);
            const double specular = cook_torrance_specular(
                light1.position,
                position,
                view,
                normal,
                0.1,
                1.0,
                1.1
            );

            vec3 ambient_color =
                // {ucos(longitude, 2.0), ucos(latitude, 1.0), 0.0};
                {1.0, 0.0, 0.0};
            double specular_power = 0.5;
            double diffuse_power = 0.5;
            double ambient_power = 0.0;

            const Line line_to_light(
                position,
                normalize(light1.position - position)
            );
            bool in_shadow = false;
            for (auto const& i : intersectables) {
                if (i->find_intersection(line_to_light).has_value()) {
                    if (i.get() != intersectable_p) {
                        in_shadow = true;
                        break;
                    }
                }
            }
            if (in_shadow) {
                diffuse_power = 0.0;
            }
            Color c1 = to_color(
                specular_power * specular * color::white +
                diffuse_power * diffuse * color::white +
                ambient_power * ambient_color
            );
            img1.set_color_at(
                x + screen.get_horizontal_discretization() * offset_x,
                y + screen.get_vertical_discretization() * offset_y,
                c1
            );
        }
        if (offset_x < 2) {
            offset_x++;
        } else {
            offset_x = 0;
            offset_y++;
        }
    }
    img1.save("gaussian_distrabution_specular.png");
}
