#include <complex>
#include <queue>
#include <sstream>

#include "array_ops.h"
#include "intersectable.h"
#include "light_graph.h"
#include "line.h"
#include "material.h"

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

// this is very similar to the intersect group function in intersectables.cc
static auto intersect_group_with_materials(
    std::vector<std::tuple<Intersectable*, Material*>> const& group,
    Line const& line,
    Intersectable const* previous_intersection_ptr
) -> std::optional<std::tuple<intersection_t, Material*>>
{
    std::optional<intersection_t> saved_intersectable{};
    Material* saved_material{nullptr};

    for (auto const& [intersectable, material] : group) {
        auto new_intersection =
            intersectable->intersect(line, previous_intersection_ptr);
        if (!new_intersection.has_value()) {
            continue;
        }

        auto const& loop_intersection_ptr =
            std::get<4>(new_intersection.value());

        // assumption that these ptrs are not null
        if (loop_intersection_ptr == previous_intersection_ptr) {
            auto const inside_intersection =
                previous_intersection_ptr->inside_intersect(line);
            if (!inside_intersection.has_value()) {
                continue;
            }
            auto const& inside_t = std::get<0>(inside_intersection.value());
            if (saved_intersectable.has_value()) {
                auto const& saved_t = std::get<0>(saved_intersectable.value());
                if (inside_t < saved_t) {
                    saved_intersectable = inside_intersection;
                    saved_material = material;
                }
            } else {
                saved_intersectable = inside_intersection;
                saved_material = material;
            }
        } else {
            auto const& loop_t = std::get<0>(new_intersection.value());
            if (saved_intersectable.has_value()) {
                auto const& saved_t = std::get<0>(saved_intersectable.value());
                if (loop_t < saved_t) {
                    saved_intersectable = new_intersection;
                    saved_material = material;
                }
            } else {
                saved_intersectable = new_intersection;
                saved_material = material;
            }
        }
    }
    if (saved_intersectable.has_value()) {
        std::tuple<intersection_t, Material* const> const returned = {
            saved_intersectable.value(),
            saved_material
        };
        return returned;
    }

    return {};
}

auto LightGraphNode::construct_with_material(
    std::vector<std::tuple<Intersectable*, Material*>> const& group,
    Material const* void_material,
    Intersectable const* remove_ptr
) -> void
{
    if (depth >= max_tree_depth) {
        material = void_material;
        return;
    }

    auto const intersection_and_mat =
        intersect_group_with_materials(group, line, remove_ptr);

    if (!intersection_and_mat.has_value()) {
        material = void_material;
        return;
    }

    intersection = std::get<0>(intersection_and_mat.value());
    material = std::get<1>(intersection_and_mat.value());

    auto const& [t, surface_normal, btn_matrix, uv_opt, remove_intersectable_ptr] =
        intersection.value();

    auto const solution_point = solve_line(line, t);

    bool const has_normal_texture =
        material->has_texture_normals() && uv_opt.has_value();
    auto const normal{
        has_normal_texture ? btn_matrix * material->get_texture_normal(
                                              uv_opt.value()[0],
                                              uv_opt.value()[1]
                                          )
                           : surface_normal
    };

    // reflection
    auto const reflected_precent = material->get_reflect_precent();
    if (reflected_precent > 0.0) {
        Line const reflected_line(
            solution_point,
            reflected_direction(line.direction, normal)
        );
        reflected = std::make_unique<LightGraphNode>(
            material,
            depth + 1,
            light_intensity * reflected_precent,
            reflected_line,
            this
        );
        reflected->construct_with_material(
            group,
            void_material,
            remove_intersectable_ptr
        );
    }

    // refraction
    auto const refracted_precent = material->get_refract_precent();
    if (refracted_precent > 0.0) {
        Line const refracted_line = Line(
            solution_point,
            refracted_direction(
                line.direction,
                normal,
                parent == nullptr ? enviroment_index_of_refraction
                                  : parent->material->get_index_of_refraction(),
                material->get_index_of_refraction()
            )
        );
        refracted = std::make_unique<LightGraphNode>(
            material,
            depth + 1,
            light_intensity * refracted_precent,
            refracted_line,
            this
        );
        refracted->construct_with_material(
            group,
            void_material,
            remove_intersectable_ptr
        );
    }
}

auto LightGraphNode::calculate_color(
    Camera const& camera,
    PointLight const& light,
    double total_intensity
) const -> std::array<double, 3>
{
    if (intersection.has_value()) {
        std::array<double, 3> final_color{};
        auto const& [t, surface_normal, btn_matrix, uv_opt, iptr] =
            intersection.value();
        std::array<double, 3> normal{};
        if (uv_opt.has_value() && material->has_texture_normals()) {
            // convert to normal_coord system
            normal = btn_matrix * material->get_texture_normal(
                                      std::get<0>(uv_opt.value()),
                                      std::get<1>(uv_opt.value())
                                  );
        } else {
            normal = surface_normal;
        }
        normal = normalize(normal);
        auto const position = solve_line(line, t);
        auto const view = normalize(camera.get_position() - position);
        auto const incoming = normalize(light.position - position);
        [[maybe_unused]] auto const outgoing =
            normalize(reflected_direction(incoming, normal));
        [[maybe_unused]] double const cos_incident_angle =
            dot(incoming, normal);

        double const diffuse = phong_diffuse(light.position, position, normal);
        double const specular = blin_phong_specular(
            light.position,
            position,
            view,
            normal,
            material->get_specular_exponent()
        );

        if (uv_opt.has_value()) {
            final_color = (light_intensity / total_intensity) *
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
            final_color = (light_intensity / total_intensity) *
                          (material->get_specular_coeff() * specular *
                               material->get_specular_color() +
                           material->get_diffuse_coeff() * diffuse *
                               material->get_diffuse_color() +
                           material->get_ambient_coeff() *
                               material->get_base_ambient_color());
        }

        if (refracted) {
            final_color =
                final_color +
                refracted->calculate_color(camera, light, total_intensity);
        }
        if (reflected) {
            final_color =
                final_color +
                reflected->calculate_color(camera, light, total_intensity);
        }
        return final_color;
    }
    return (light_intensity / total_intensity) *
           material->get_base_ambient_color();
}

auto LightGraphNode::light_pixel(Camera const& camera, PointLight const& light)
    const -> std::array<double, 3>
{
    if (intersection.has_value()) {
        auto const& [t, surface_normal, btn_matrix, uv_opt, iptr] =
            intersection.value();
        std::array<double, 3> normal{};
        if (uv_opt.has_value() && material->has_texture_normals()) {
            // convert to normal_coord system
            normal = btn_matrix * material->get_texture_normal(
                                      std::get<0>(uv_opt.value()),
                                      std::get<1>(uv_opt.value())
                                  );
        } else {
            normal = surface_normal;
        }
        normal = normalize(normal);
        auto const position = solve_line(line, t);
        auto const outgoing = normalize(position - camera.get_position());
        auto const incoming = normalize(light.position - position);
        // auto const reflected = normalize(reflected_direction(incoming,
        // normal));
        auto const halfway = normalize((outgoing + incoming) * 0.5);
        double const cos_incident_angle = dot(incoming, normal);

        // This is blin phong
        auto const brdf = []([[maybe_unused]] std::array<double, 3> position,
                             [[maybe_unused]] std::array<double, 3> incoming,
                             [[maybe_unused]] std::array<double, 3> outgoing,
                             [[maybe_unused]] std::array<double, 3> halfway,
                             [[maybe_unused]] std::array<double, 3> normal,
                             double shininess) -> std::array<double, 3> {
            constexpr std::array<double, 3> c_diffuse{1, 1, 1};
            constexpr std::array<double, 3> c_specular{1, 1, 1};
            return c_diffuse +
                   c_specular * std::pow(dot(normal, halfway), shininess);
        };

        std::array<double, 3> const final_color{
            brdf(position, incoming, outgoing, halfway, normal, 1.0) *
            light_intensity * cos_incident_angle
        };
        return final_color;
    }

    return (light_intensity)*material->get_base_ambient_color();
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
    q.push(this);
    while (!q.empty()) {
        LightGraphNode const* node = q.front();
        q.pop();
        intensity += node->light_intensity;
        if (node->reflected) {
            q.push(node->reflected.get());
        }
        if (node->refracted) {
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
    ss << pad(depth) << "intenity: " << light_intensity << "\n";
    ss << pad(depth)
       << "ambient_base_color: " << material->get_base_ambient_color() << "\n";
    if (intersection.has_value()) {
        ss << pad(depth) << "position: "
           << solve_line(line, std::get<0>(intersection.value())) << "\n";
        ss << pad(depth)
           << "intersection ptr: " << std::get<4>(intersection.value()) << "\n";
        ss << pad(depth) << "t: " << std::get<0>(intersection.value()) << "\n";
        ss << pad(depth) << "line dir: " << line.direction << "\n";
    }
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

// I keep these functions around because im lazy and don't want to refractor
// some test. Do not include on new test.
// @depreciated
static auto intersect_group(
    std::vector<Intersectable*> const& is,
    Line const& line,
    Intersectable const* remove
) -> std::optional<intersection_t>
{
    std::optional<intersection_t> intersection{};
    for (auto const& intersectable : is) {
        std::optional<intersection_t> new_intersection =
            intersectable->intersect(line, remove);
        // skip if intersected in remove param
        if (new_intersection.has_value()) {
            if (std::get<4>(new_intersection.value()) == remove) {
                new_intersection = {};
            }
        }
        if (intersection.has_value() && new_intersection.has_value()) {
            if (std::get<0>(new_intersection.value()) <
                std::get<0>(intersection.value())) {
                intersection = new_intersection;
            }
        } else if (!intersection.has_value() && new_intersection.has_value()) {
            intersection = new_intersection;
        }
    }
    return intersection;
}

// @depreciated
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
                    // grabs the surface normal (or local z coordinate)
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
            reflected->construct(is, std::get<4>(intersection.value()));
        }
        if (material->get_refract_precent() > 0.0) {
            Line const refracted_line = Line(
                position,
                refracted_direction(
                    line.direction,
                    // grabs the surface normal (or local z coordinate)
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
            refracted->construct(is, std::get<4>(intersection.value()));
        }
    }
}
