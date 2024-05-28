#include <queue>
#include <sstream>

#include "array_ops.h"
#include "intersectable.h"
#include "material.h"
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
    intersection = intersect_group(is, line, remove_ptr);
    if (intersection.has_value() && depth < max_tree_depth) {
        auto const position =
            solve_line(line, std::get<0>(intersection.value()));
        if (material->get_reflect_precent() > 0.0) {
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
                light_intensity * material->get_reflect_precent(),
                reflected_line,
                this
            );
            reflected->construct(is, std::get<3>(intersection.value()));
        }
        if (material->get_refract_precent() > 0.0) {
            Line const refracted_line = Line(
                position,
                refracted_direction(
                    line.direction,
                    std::get<1>(intersection.value()),
                    parent == nullptr
                        ? enviroment_index_of_refraction
                        : parent->material->get_index_of_refraction(),
                    material->get_index_of_refraction()
                )
            );
            refracted = std::make_unique<LightGraphNode>(
                material,
                depth + 1,
                light_intensity * material->get_refract_precent(),
                refracted_line,
                this
            );
            refracted->construct(is, std::get<3>(intersection.value()));
        }
    }
}

// this is very similar to the intersect group function in intersectables.cc
static auto intersect_group_with_materials(
    std::vector<std::tuple<Intersectable*, Material*>> os,
    Line const& line,
    Intersectable const* remove
) -> std::optional<std::tuple<intersection_t, Material*>>
{
    std::optional<intersection_t> intersection{};
    Material* saved_mat{nullptr};
    for (auto const& [intersectable, mat] : os) {
        std::optional<intersection_t> new_intersection =
            intersectable->intersect(line, remove);
        if (new_intersection.has_value()) {
            if (std::get<3>(new_intersection.value()) == remove) {
                new_intersection = {};
            }
        }
        if (intersection.has_value() && new_intersection.has_value()) {
            if (std::get<0>(new_intersection.value()) <
                std::get<0>(intersection.value())) {
                intersection = new_intersection;
                saved_mat = mat;
            }
        } else if (!intersection.has_value() && new_intersection.has_value()) {
            intersection = new_intersection;
            saved_mat = mat;
        }
    }
    if (intersection.has_value()) {
        std::tuple<intersection_t, Material* const> returned = {
            intersection.value(),
            saved_mat
        };
        return returned;
    } else {
        return {};
    }
}

auto LightGraphNode::construct_with_material(
    std::vector<std::tuple<Intersectable*, Material*>> const& os,
    Material const* bg_material,
    Intersectable const* remove_ptr
) -> void
{
    auto const intersection_and_mat =
        intersect_group_with_materials(os, line, remove_ptr);
    if (intersection_and_mat.has_value() && depth < max_tree_depth) {
        intersection = std::get<0>(intersection_and_mat.value());
        material = std::get<1>(intersection_and_mat.value());

        auto const position =
            solve_line(line, std::get<0>(intersection.value()));
        if (material->get_reflect_precent() > 0.0) {
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
                light_intensity * material->get_reflect_precent(),
                reflected_line,
                this
            );
            reflected->construct_with_material(
                os,
                bg_material,
                std::get<3>(intersection.value())
            );
        }
        if (material->get_refract_precent() > 0.0) {
            Line const refracted_line = Line(
                position,
                refracted_direction(
                    line.direction,
                    std::get<1>(intersection.value()),
                    parent == nullptr
                        ? enviroment_index_of_refraction
                        : parent->material->get_index_of_refraction(),
                    material->get_index_of_refraction()
                )
            );
            refracted = std::make_unique<LightGraphNode>(
                material,
                depth + 1,
                light_intensity * material->get_refract_precent(),
                refracted_line,
                this
            );
            refracted->construct_with_material(
                os,
                bg_material,
                std::get<3>(intersection.value())
            );
        }
    } else {
        material = bg_material;
    }
}

auto LightGraphNode::calculate_color(
    Camera const& camera,
    PointLight const& light,
    double total_intensity
) const -> std::array<double, 3>
{
    if (intersection.has_value()) {
        std::array<double, 3> vcol{0, 0, 0};
        auto const& [t, normal, uv_opt, iptr] = intersection.value();
        std::array<double, 3> const position = solve_line(line, t);
        std::array<double, 3> const view = camera.get_position() - position;
        double const diffuse = phong_diffuse(light.position, position, normal);
        double const specular = blin_phong_specular(
            light.position,
            position,
            view,
            normal,
            material->get_specular_exponent()
        );

        if (uv_opt.has_value()) {
            vcol = (light_intensity / total_intensity) *
                   (material->get_specular_coeff() * specular *
                        material->get_specular_color() +
                    material->get_diffuse_coeff() * diffuse *
                        material->get_diffuse_color() +
                    material->get_ambient_coeff() *
                        material->get_ambient_color(
                            std::get<0>(uv_opt.value()),
                            std::get<1>(uv_opt.value())
                        ));
        } else {
            vcol = (light_intensity / total_intensity) *
                   (material->get_specular_coeff() * specular *
                        material->get_specular_color() +
                    material->get_diffuse_coeff() * diffuse *
                        material->get_diffuse_color() +
                    material->get_ambient_coeff() *
                        material->get_ambient_color(0, 0));
        }
        if (refracted) {
            vcol = vcol +
                   refracted->calculate_color(camera, light, total_intensity);
        }
        if (reflected) {
            vcol = vcol +
                   reflected->calculate_color(camera, light, total_intensity);
        }
        return vcol;
    }
    return material->get_ambient_color(0, 0);
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
        if (node->reflected && node->reflected->intersection.has_value()) {
            q.push(node->reflected.get());
        }
        if (node->refracted && node->refracted->intersection.has_value()) {
            q.push(node->refracted.get());
        }
    }
    return intensity;
}
static auto pad(size_t depth, std::string const& repeated = " ") -> std::string
{
    std::string returned{};
    for (size_t i = 0; i < depth; i++) {
        returned += repeated;
    }
    return returned;
}

auto LightGraphNode::to_string_helper(size_t depth, std::stringstream& ss) const
    -> void
{
    ss << pad(depth) << "ambient_color: " << material->get_ambient_color(0, 0)
       << "\n";
    if (reflected) {
        ss << pad(depth) << "reflected:\n";
        reflected->to_string_helper(depth + 1, ss);
    }
    if (refracted) {
        ss << pad(depth) << "refracted:\n";
        refracted->to_string_helper(depth + 1, ss);
    }
}

auto LightGraphNode::to_string() const -> std::string
{
    std::stringstream ss;
    to_string_helper(0, ss);
    return ss.str();
}
