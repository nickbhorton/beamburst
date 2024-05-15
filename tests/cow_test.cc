#include <chrono>
#include <fstream>
using namespace std::chrono;

#include "array_ops.h"
#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "parser.h"
#include "vector_ops.h"

using namespace linalg;

typedef std::
    tuple<std::size_t, std::size_t, vec3, vec3, Intersectable const* const>
        pixel_job_t;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    constexpr size_t img_size = 1024;
    Screen screen{.discretization = {img_size, img_size}, .size = {1.0, 1.0}};

    Camera const camera(
        screen,
        1.0,
        {8.2, 0.0, 6.0}, // pos
        {-1, 0.0, -1},   // view
        {0.0, 1.0, 0.0}  // up
    );

    std::ifstream cow_file("../resources/objects/cow.obj", std::ifstream::in);
    VertexObject const cow_vo(cow_file);
    BVHNode cow_bvh{};
    std::vector<Triangle> const triangles = cow_vo.extract_triangles();
    for (auto const& triangle : triangles) {
        cow_bvh.add_primitive(&triangle);
    }
    cow_bvh.construct_tree();

    std::vector<pixel_job_t> pixel_jobs{};

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            auto intersection = cow_bvh.intersect(camera.get_line_at(x, y));

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
                     std::get<3>(intersection.value())}
                );
            }
        }
    }

    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        Color(255, 255, 255, 255)
    };
    PointLight const light({8.2, 0.0, 6.0});
    for (const auto& [x, y, position, normal, intersectable_p] : pixel_jobs) {
        vec3 const view = camera.get_position() - position;
        double const diffuse = phong_diffuse(light.position, position, normal);
        double const specular =
            blin_phong_specular(light.position, position, view, normal, 100.0);

        vec3 constexpr ambient_color = {0.0, 0.0, 0.0};
        double constexpr ambient_power = 0.1;
        double constexpr diffuse_power = 0.5;
        double constexpr specular_power = 0.4;

        Color const c1 = to_color(
            specular_power * specular * color::green +
            diffuse_power * diffuse * color::blue +
            ambient_power * ambient_color
        );
        img.set_color_at(x, y, c1);
    }
    img.save("cow_test.png");
}
