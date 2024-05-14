#include <fstream>

#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "parser.h"
#include "triangle.h"
#include "vector_ops.h"

using namespace linalg;

typedef std::tuple<
    std::size_t,
    std::size_t,
    vec3,
    vec3,
    vec2,
    Intersectable const* const>
    pixel_job_t;

int main()
{
    constexpr double axis_distance = 3.3;
    constexpr Screen screen{.discretization = {512, 512}, .size = {1.0, 1.0}};

    Camera camera(
        screen,
        1.0,
        {axis_distance, axis_distance, axis_distance}, // pos
        {-1.0, -1.0, -1.0},                            // view
        {0.0, 1.0, 0.0}                                // up
    );

    std::vector<Intersectable*> intersectables{};
    std::ifstream cube_file("../resources/objects/cube.obj", std::ifstream::in);
    VertexObject cube_vo(cube_file);
    std::vector<Triangle> triangles = cube_vo.extract_triangles();
    for (size_t i = 0; i < triangles.size(); i++) {
        intersectables.push_back(&triangles[i]);
    }
    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    std::vector<pixel_job_t> pixel_jobs{};
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            std::optional<intersection_t> intersection{};
            Intersectable* iptr{nullptr};
            for (auto& intersectable : intersectables) {
                std::optional<intersection_t> const new_intersection =
                    intersectable->intersect(camera.get_line_at(x, y));
                if (intersection.has_value() && new_intersection.has_value()) {
                    if (std::get<0>(new_intersection.value()) <
                        std::get<0>(intersection.value())) {
                        intersection = new_intersection;
                        iptr = intersectable;
                    }
                } else if (!intersection.has_value() && new_intersection.has_value()) {
                    intersection = new_intersection;
                    iptr = intersectable;
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
                     std::get<2>(intersection.value()).value(),
                     iptr}
                );
            }
        }
    }

    Color bg_color{255, 255, 255, 255};
    Image img1{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        bg_color
    };

    PointLight light({5, 0.0, 0.0});
    for (const auto& [x, y, position, normal, uv, intersectable_p] :
         pixel_jobs) {
        const vec3 view = camera.get_position() - position;
        const double diffuse = phong_diffuse(light.position, position, normal);
        const double specular =
            blin_phong_specular(light.position, position, view, normal, 100.0);

        const vec3 ambient_color = {uv[0], 0.0, uv[1]};
        constexpr double ambient_power = 0.4;
        constexpr double diffuse_power = 0.3;
        constexpr double specular_power = 0.3;

        Color c1 = to_color(
            specular_power * specular * color::white +
            diffuse_power * diffuse * color::white +
            ambient_power * ambient_color
        );
        img1.set_color_at(x, y, c1);
    }

    img1.save("cube_test.png");
}
