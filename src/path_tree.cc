#include <queue>

#include "array_ops.h"
#include "color.h"
#include "path_tree.h"

// copying line which is 6 doubles
LightGraphNode::LightGraphNode(
    Material const* material,
    size_t depth,
    double intensity,
    Line const& line,
    LightGraphNode* parent
)
    : depth(depth), light_intensity(intensity), line(line), intersection{},
      reflected{}, refracted{}, parent{parent}, material(material)
{
}

auto LightGraphNode::construct(
    std::vector<Intersectable*> const& is,
    Intersectable const* remove_ptr
) -> void
{
    intersection = intersect(is, line, remove_ptr);
    if (intersection.has_value() && depth < max_tree_depth) {
        auto const position =
            solve_line(line, std::get<0>(intersection.value()));
        if (material->reflect_precent > 0.0) {
            Line const reflected_line = Line(
                position,
                reflected_direction(
                    line.direction,
                    std::get<1>(intersection.value())
                )
            );
            reflected = std::make_unique<LightGraphNode>(
                material,
                depth + 1,
                light_intensity * material->reflect_precent,
                reflected_line,
                this
            );
            reflected->construct(is, std::get<3>(intersection.value()));
        }
        if (material->refract_precent > 0.0) {
            Line const refracted_line = Line(
                position,
                refracted_direction(
                    line.direction,
                    std::get<1>(intersection.value()),
                    parent == nullptr ? enviroment_index_of_refraction
                                      : parent->material->index_of_refraction,
                    material->index_of_refraction
                )
            );
            refracted = std::make_unique<LightGraphNode>(
                material,
                depth + 1,
                light_intensity * material->refract_precent,
                refracted_line,
                this
            );
            refracted->construct(is, std::get<3>(intersection.value()));
        }
    }
}

auto LightGraphNode::calculate_color(
    Camera const& camera,
    PointLight const& light,
    double total_intensity
) const -> std::array<double, 3>
{
    std::array<double, 3> vcol{0, 0, 0};
    if (intersection.has_value()) {
        auto const& [t, normal, uv_opt, iptr] = intersection.value();
        std::array<double, 3> const position = solve_line(line, t);
        std::array<double, 3> const view = camera.get_position() - position;
        double const diffuse = phong_diffuse(light.position, position, normal);
        double const specular =
            blin_phong_specular(light.position, position, view, normal, 100.0);

        std::array<double, 3> constexpr ambient_color = {0.0, 0.0, 1.0};
        double constexpr ambient_power = 0.1;
        double constexpr diffuse_power = 0.5;
        double constexpr specular_power = 0.4;

        vcol = (light_intensity / total_intensity) *
               (specular_power * specular * color::white +
                diffuse_power * diffuse * color::white +
                ambient_power * ambient_color);
        if (refracted) {
            vcol = vcol +
                   refracted->calculate_color(camera, light, total_intensity);
        }
        if (reflected) {
            vcol = vcol +
                   reflected->calculate_color(camera, light, total_intensity);
        }
    }
    return vcol;
}

auto LightGraphNode::count_nodes() const -> size_t
{
    size_t count = 0;
    std::queue<LightGraphNode const*> q{};
    q.push(this);
    while (!q.empty()) {
        LightGraphNode const* node = q.front();
        q.pop();
        count++;
        if (node->reflected) {
            q.push(node->reflected.get());
        }
        if (node->refracted) {
            q.push(node->refracted.get());
        }
    }
    return count;
}

auto LightGraphNode::sum_light_intensity() const -> double
{
    double intensity = 0.0;
    std::queue<LightGraphNode const*> q{};
    if (intersection.has_value()) {
        q.push(this);
    }
    while (!q.empty()) {
        LightGraphNode const* node = q.front();
        q.pop();
        intensity += node->light_intensity;
        if (node->reflected && node->refracted->intersection.has_value()) {
            q.push(node->reflected.get());
        }
        if (node->refracted && node->refracted->intersection.has_value()) {
            q.push(node->refracted.get());
        }
    }
    return intensity;
}
