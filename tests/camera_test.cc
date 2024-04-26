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
#include <cmath>

using namespace linalg;

int main()
{
    typedef std::tuple<std::size_t, std::size_t, vec3, vec3, vec3> pixel_job;

    constexpr double user_distance_to_screen = 1.0;
    constexpr vec3 user_camera_position_pz = {0.0, 0.0, -2.3};
    constexpr vec3 user_camera_up_pz = {0.0, 1.0, 0.0};
    constexpr vec3 user_camera_direction_pz = {0.0, 0.0, 1.0};
    constexpr vec3 user_camera_position_nz = {0.0, 0.0, 2.3};
    constexpr vec3 user_camera_up_nz = {0.0, 1.0, 0.0};
    constexpr vec3 user_camera_direction_nz = {0.0, 0.0, -1.0};

    constexpr vec3 user_camera_position_nx = {2.3, 0.0, 0.0};
    constexpr vec3 user_camera_up_nx = {0.0, 1.0, 0.0};
    constexpr vec3 user_camera_direction_nx = {-1.0, 0.0, 0.0};
    constexpr vec3 user_camera_position_px = {-2.3, 0.0, 0.0};
    constexpr vec3 user_camera_up_px = {0.0, 1.0, 0.0};
    constexpr vec3 user_camera_direction_px = {1.0, 0.0, 0.0};

    constexpr vec3 user_camera_position_ny = {0.0, 2.3, 0.0};
    constexpr vec3 user_camera_up_ny = {1.0, 0.0, 0.0};
    constexpr vec3 user_camera_direction_ny = {0.0, -1.0, 0.0};
    constexpr vec3 user_camera_position_py = {0.0, -2.3, 0.0};
    constexpr vec3 user_camera_up_py = {1.0, 0.0, 0.0};
    constexpr vec3 user_camera_direction_py = {0.0, 1.0, 0.0};
    constexpr Screen screen{.discretization = {256, 256}, .size = {1.0, 1.0}};
    const Camera camera_nz{
        screen,
        user_distance_to_screen,
        user_camera_position_nz,
        user_camera_direction_nz,
        user_camera_up_nz
    };
    const Camera camera_pz{
        screen,
        user_distance_to_screen,
        user_camera_position_pz,
        user_camera_direction_pz,
        user_camera_up_pz
    };

    const Camera camera_nx{
        screen,
        user_distance_to_screen,
        user_camera_position_nx,
        user_camera_direction_nx,
        user_camera_up_nx
    };
    const Camera camera_px{
        screen,
        user_distance_to_screen,
        user_camera_position_px,
        user_camera_direction_px,
        user_camera_up_px
    };

    const Camera camera_ny{
        screen,
        user_distance_to_screen,
        user_camera_position_ny,
        user_camera_direction_ny,
        user_camera_up_ny
    };
    const Camera camera_py{
        screen,
        user_distance_to_screen,
        user_camera_position_py,
        user_camera_direction_py,
        user_camera_up_py
    };
    std::vector<Camera> cameras = {};
    cameras.push_back(camera_nz);
    cameras.push_back(camera_pz);
    cameras.push_back(camera_nx);
    cameras.push_back(camera_px);
    cameras.push_back(camera_ny);
    cameras.push_back(camera_py);

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
        {screen.get_horizontal_discretization() * cameras.size(),
         screen.get_vertical_discretization()},
        bg_color
    };

    vec3 light_position = {0.0, 0.0, 0.0};
    vec3 specular_color = {1.0, 1.0, 1.0};
    double specular_power = 0.05;
    vec3 diffuse_color = {1.0, 1.0, 1.0};
    double diffuse_power = 0.05;
    double ambient_power = 0.9;
    double offset = 0;
    for (const auto& pixel_jobs : pixel_jobs_vector) {
        for (const auto& [x, y, position, normal, view] : pixel_jobs) {
            auto const& [diffuse, specular] =
                blin_phong(light_position, position, view, normal, 10.0);
            const vec3 spherical_normal = cartesian_to_spherical(normal);
            double phi = (spherical_normal[2] + M_PI) / (2.0 * M_PI);
            double theta = spherical_normal[1] / M_PI;
            // vec3 ambient_color = {saturate(phi), saturate(theta), 0.0};
            // vec3 ambient_color = {0.0, saturate(theta), 0.0};
            const double r_freq = 10.0;
            const double r =
                saturate(0.5 * (-std::cos(r_freq * phi * 2.0 * M_PI) + 1.0));
            const double b_freq = 5.0;
            const double b =
                saturate(0.5 * (-std::cos(b_freq * theta * 2.0 * M_PI) + 1.0));
            vec3 ambient_color = {r, 0.0, b};
            Color c1 = to_color(
                specular_power * specular * specular_color +
                diffuse_power * diffuse * diffuse_color +
                ambient_power * ambient_color
            );
            img1.set_color_at(
                x + screen.get_horizontal_discretization() * offset,
                y,
                c1
            );
        }
        offset++;
    }
    img1.save("top.png");
}
