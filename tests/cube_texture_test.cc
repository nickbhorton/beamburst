#include <fstream>

#include "array_ops.h"
#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "image.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "linear_types.h"
#include "material.h"
#include "parser.h"
#include "path_tree.h"
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

    Material cube_material{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {0, 0, 1},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.5
    };
    Material bg_material{
        .index_of_refraction = 1.0,
        .reflect_precent = 0.0,
        .refract_precent = 0.0,
        .ambient_color = {1, 1, 1},
        .diffuse_color = {1, 1, 1},
        .specular_color = {1, 1, 1},
        .ambient_coeff = 0.2,
        .diffuse_coeff = 0.3,
        .specular_coeff = 0.5
    };
    std::vector<std::tuple<Intersectable*, Material*>> os{};

    std::ifstream cube_file("../resources/objects/cube.obj", std::ifstream::in);
    VertexObject cube_vo(cube_file);
    BVHNode cube_bvh{};
    std::vector<Triangle> const triangles = cube_vo.extract_triangles();
    for (auto const& triangle : triangles) {
        cube_bvh.add_primitive(&triangle);
    }
    cube_bvh.construct_tree();
    os.push_back({&cube_bvh, &cube_material});

    size_t const height = screen.get_vertical_discretization();
    size_t const width = screen.get_horizontal_discretization();
    Image img{
        {screen.get_horizontal_discretization(),
         screen.get_vertical_discretization()},
        {255, 255, 255, 255}
    };
    PointLight const light(
        camera.get_position() + std::array<double, 3>({0, 10, 0})
    );
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            LightGraphNode
                root{&bg_material, 0, 1, camera.get_line_at(x, y), nullptr};
            root.construct_with_material(os);
            double const total_intensity = root.sum_light_intensity();
            vec3 const vcol =
                root.calculate_color(camera, light, total_intensity);
            img.set_color_at(x, y, to_color(vcol));
        }
    }

    img.save("cube_texture_test.png");
}
