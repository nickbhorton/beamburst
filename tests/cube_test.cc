#include <algorithm>
#include <fstream>
#include <utility>

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

typedef std::
    tuple<std::size_t, std::size_t, vec3, vec3, Intersectable const* const>
        pixel_job_t;

typedef std::tuple<double, Intersectable const*> intersection_t;

struct {
    bool operator()(intersection_t e1, intersection_t e2) const
    {
        return std::get<0>(e1) < std::get<0>(e2);
    }
} IntersectionLess;

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

    std::vector<std::unique_ptr<Intersectable>> intersectables{};
    std::ifstream cube_file("../resources/objects/cube.obj", std::ifstream::in);
    const obj_t cube_obj = basic_obj_parse(cube_file);
    const auto& [vertexes, normals, uvs, faces] = cube_obj;
    for (auto face : faces) {
        // this is a quad so we need two triangles
        if (face.size() == 4) {
            intersectables.push_back(std::make_unique<Triangle>(Triangle{
                &vertexes[std::get<0>(face[0]) - 1],
                &vertexes[std::get<0>(face[1]) - 1],
                &vertexes[std::get<0>(face[2]) - 1]
            }));
            intersectables.push_back(std::make_unique<Triangle>(Triangle{
                &vertexes[std::get<0>(face[2]) - 1],
                &vertexes[std::get<0>(face[3]) - 1],
                &vertexes[std::get<0>(face[0]) - 1]
            }));
        }
    }
    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
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
                    {x, y, solution_position, solution_normal, intersectable_p}
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

    PointLight light(camera.get_position());
    for (const auto& [x, y, position, normal, intersectable_p] : pixel_jobs) {
        const vec3 view = camera.get_position() - position;
        const double diffuse = phong_diffuse(light.position, position, normal);
        const double specular =
            blin_phong_specular(light.position, position, view, normal, 100.0);

        constexpr vec3 ambient_color = {0.0, 0.4, 1.0};
        constexpr double ambient_power = 0.2;
        constexpr double diffuse_power = 0.4;
        constexpr double specular_power = 0.4;

        Color c1 = to_color(
            specular_power * specular * color::white +
            diffuse_power * diffuse * color::white +
            ambient_power * ambient_color
        );
        img1.set_color_at(x, y, c1);
    }

    img1.save("cube_test.png");
}
