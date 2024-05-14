#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "sphere.h"
#include "vector_ops.h"

using namespace linalg;

typedef std::
    tuple<std::size_t, std::size_t, vec3, vec3, Intersectable const* const>
        pixel_job_t;

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
                std::optional<intersection_t> intersection{};
                Intersectable* iptr{nullptr};
                for (auto& intersectable : intersectables) {
                    std::optional<intersection_t> const new_intersection =
                        intersectable->intersect(camera.get_line_at(x, y));
                    if (intersection.has_value() &&
                        new_intersection.has_value()) {
                        if (std::get<0>(new_intersection.value()) <
                            std::get<0>(intersection.value())) {
                            intersection = new_intersection;
                            iptr = intersectable.get();
                        }
                    } else if (!intersection.has_value() && new_intersection.has_value()) {
                        intersection = new_intersection;
                        iptr = intersectable.get();
                    }
                }

                if (intersection.has_value()) {
                    auto const solution_position =
                        std::get<0>(intersection.value()) *
                            camera.get_line_at(x, y).direction +
                        camera.get_line_at(x, y).position;
                    pixel_jobs.push_back(
                        {x,
                         y,
                         solution_position,
                         std::get<1>(intersection.value()),
                         iptr}
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
        for (const auto& [x, y, position, normal, intersectable_p] :
             pixel_jobs) {
            const vec3 view =
                cameras[offset_x * 3 + offset_y].get_position() - position;
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
    img1.save("camera_test.png");
}
