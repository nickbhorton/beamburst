#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "coordinates.h"
#include "image.h"
#include "intersections.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "sphere.h"
#include "vector_ops.h"
#include <format>

using namespace linalg;
typedef std::tuple<std::size_t, std::size_t, vec3, vec3, vec3> pixel_job;

int main()
{
    size_t image_count = 0;
    for (double d = -4.0; d < 4.0; d += 0.5) {
        image_count++;
        vec3 light_position = {0.0, 4.0, d};
        constexpr double axis_distance = 2.3;
        constexpr Screen screen{
            .discretization = {512, 512},
            .size = {1.0, 1.0}
        };

        std::vector<Camera> cameras = {};
        cameras.push_back(axis_aligned_camera_nx(screen, axis_distance));
        cameras.push_back(axis_aligned_camera_ny(screen, axis_distance));
        cameras.push_back(axis_aligned_camera_nz(screen, axis_distance));
        cameras.push_back(axis_aligned_camera_px(screen, axis_distance));
        cameras.push_back(axis_aligned_camera_py(screen, axis_distance));
        cameras.push_back(axis_aligned_camera_pz(screen, axis_distance));

        constexpr Sphere sphere{.position = {0.0, 0.0, 0.0}, .radius = 1.0};
        std::vector<std::vector<pixel_job>> pixel_jobs_vector = {};
        for (size_t camera_index = 0; camera_index < cameras.size();
             camera_index++) {
            std::vector<pixel_job> jobs{};
            for (size_t y = 0; y < screen.get_vertical_discretization(); y++) {
                for (size_t x = 0; x < screen.get_horizontal_discretization();
                     x++) {
                    {
                        Line line = cameras[camera_index].get_line_at(x, y);
                        const auto t_opt = find_intersection(line, sphere);
                        if (t_opt.has_value()) {
                            const vec3 solution_position =
                                cameras[camera_index].get_position() +
                                t_opt.value() * line.direction;
                            const vec3 view_direction =
                                cameras[camera_index].get_position() -
                                solution_position;
                            vec3 solution_normal =
                                solution_position - sphere.position;
                            jobs.push_back(
                                {x,
                                 y,
                                 solution_position,
                                 solution_normal,
                                 view_direction}
                            );
                        }
                    }
                }
            }
            pixel_jobs_vector.push_back(jobs);
        }
        Color bg_color{0, 0, 0, 64};
        Image img1{
            {screen.get_horizontal_discretization() * cameras.size() / 2,
             screen.get_vertical_discretization() * cameras.size() / 3},
            bg_color
        };

        size_t offset_x = 0;
        size_t offset_y = 0;
        for (const auto& pixel_jobs : pixel_jobs_vector) {
            for (const auto& [x, y, position, normal, view] : pixel_jobs) {
                const vec3 spherical_normal = cartesian_to_sphere_uv(normal);
                const double longitude = spherical_normal[1];
                const double latitude = spherical_normal[2];
                /*
                vec3 gn = to_tangent_space(normalize(gaussian_normal(
                    fract(longitude * 64.0),
                    fract(smoothstep(0.05, 0.95, latitude) * 20.0),
                    0.15,
                    1.0,
                    0.5,
                    0.5
                )));
                */
                auto const& [diffuse, specular] =
                    blin_phong(light_position, position, view, normal, 100.0);

                vec3 ambient_color = {
                    ucos(longitude, 2.0) * ucos(latitude, 1.0),
                    ucos(longitude, 4.0) * ucos(latitude, 2.0),
                    ucos(longitude, 8.0) * ucos(latitude, 4.0)
                };

                constexpr double specular_power = 0.3;
                constexpr double diffuse_power = 0.3;
                constexpr double ambient_power = 0.8;
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
        std::cout << image_count << " ";
        std::flush(std::cout);
        img1.save(std::format("camera_test_{}.png", image_count));
    }
    std::cout << "\n";
}
