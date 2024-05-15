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

int main()
{
    Screen constexpr screen{.discretization = {512, 512}, .size = {1.0, 1.0}};

    Camera const camera(
        screen,
        1.0,
        {14.0, 0.0, 0.0}, // pos
        {-1, 0, 0},       // view
        {0.0, 1.0, 0.0}   // up
    );

    std::ifstream cow_file("../resources/objects/cow.obj", std::ifstream::in);
    VertexObject const cow_vo(cow_file);
    BVHNode cow_bvh{};
    std::vector<Triangle> const triangles = cow_vo.extract_triangles();
    for (auto const& triangle : triangles) {
        cow_bvh.add_primitive(&triangle);
    }
    cow_bvh.construct_tree();
    // cow_bvh.print(std::cout);

    std::vector<pixel_job_t> pixel_jobs{};

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    long total_time{0};
    for (size_t y = 0; y < height; y++) {
        auto start = high_resolution_clock::now();
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
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        total_time += duration.count();
        std::cout << y << "\t" << duration.count() << "ms\n";
        std::flush(std::cout);
    }
    std::cout << "total time " << total_time / 1000.0 << "s\n";

    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        Color(255, 255, 255, 255)
    };
    PointLight const light({14, 0.0, 0.0});
    for (const auto& [x, y, position, normal, intersectable_p] : pixel_jobs) {
        vec3 const view = camera.get_position() - position;
        double const diffuse = phong_diffuse(light.position, position, normal);
        double const specular =
            blin_phong_specular(light.position, position, view, normal, 100.0);

        vec3 constexpr ambient_color = {0.0, 0.0, 1.0};
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
