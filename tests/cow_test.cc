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
#include "triangle_bounding_volume.h"
#include "vector_ops.h"

using namespace linalg;

typedef std::
    tuple<std::size_t, std::size_t, vec3, vec3, Intersectable const* const>
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
    Screen constexpr screen{.discretization = {64, 64}, .size = {1.0, 1.0}};

    Camera const camera(
        screen,
        1.0,
        {8.0, 5.0, 6.0},    // pos
        {-1.0, -1.0, -1.0}, // view
        {0.0, 1.0, 0.0}     // up
    );

    std::vector<Intersectable*> intersectables{};
    std::ifstream cow_file("../resources/objects/cow.obj", std::ifstream::in);
    VertexObject const cow_vo(cow_file);
    TriangleBoundingVolume cow_tbv(cow_vo.extract_triangles());
    intersectables.push_back(&cow_tbv);

    std::vector<pixel_job_t> pixel_jobs{};

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            std::vector<intersection_t> ts{};
            Line const line = camera.get_line_at(x, y);
            auto const intersect_line = [&line, &ts](Intersectable* i) {
                auto t_opt = i->find_intersection(line);
                if (t_opt) {
                    ts.push_back({t_opt.value(), i});
                }
            };
            std::ranges::for_each(
                std::as_const(intersectables),
                intersect_line
            );

            if (ts.size()) {
                std::sort(ts.begin(), ts.end(), IntersectionLess);
                auto const& [t, intersectable_p] = ts.front();
                vec3 const solution_position =
                    camera.get_position() + t * line.direction;
                vec3 const solution_normal =
                    intersectable_p->find_surface_normal(solution_position);
                pixel_jobs.push_back(
                    {x, y, solution_position, solution_normal, intersectable_p}
                );
            }
        }
    }

    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        Color(255, 255, 255, 255)
    };
    PointLight const light({10, 0.0, 0.0});
    for (const auto& [x, y, position, normal, intersectable_p] : pixel_jobs) {
        vec3 const view = camera.get_position() - position;
        double const diffuse = phong_diffuse(light.position, position, normal);
        double const specular =
            blin_phong_specular(light.position, position, view, normal, 100.0);

        vec3 constexpr ambient_color = {1.0, 0.0, 0.0};
        double constexpr ambient_power = 0.4;
        double constexpr diffuse_power = 0.3;
        double constexpr specular_power = 0.3;

        Color const c1 = to_color(
            specular_power * specular * color::white +
            diffuse_power * diffuse * color::white +
            ambient_power * ambient_color
        );
        img.set_color_at(x, y, c1);
    }
    img.save("cow_test.png");
}
