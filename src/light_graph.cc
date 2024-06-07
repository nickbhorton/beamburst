#include <limits>
#include <queue>
#include <sstream>

#include "array_ops.h"
#include "intersectable.h"
#include "light_graph.h"
#include "line.h"
#include "material.h"

// copying line which is 6 Floats
LightGraphNode::LightGraphNode(
    Material const* material,
    size_t depth,
    Float intensity,
    Line const& line,
    LightGraphNode* parent
)
    : depth(depth), light_intensity(intensity), line(line), intersection{},
      reflected{}, refracted{}, parent{parent}, material(material)
{
}

// this is very similar to the intersect group function in intersectables.cc
static auto intersect_group_with_materials(
    std::vector<std::tuple<Intersectable const*, Material const*>> const& group,
    Line const& line
)
    -> std::optional<
        std::tuple<std::unique_ptr<SurfaceIntersection>, Material const*>>
{
    std::optional<std::unique_ptr<SurfaceIntersection>> saved_intersectable{};
    Material const* saved_material{nullptr};

    for (auto const& [intersectable, material] : group) {
        auto new_intersection =
            intersectable->intersect(line, std::numeric_limits<Float>::max());
        if (!new_intersection.has_value()) {
            continue;
        }

        auto const& loop_t = new_intersection.value()->get_t();
        if (saved_intersectable.has_value()) {
            auto const& saved_t = saved_intersectable.value()->get_t();
            if (loop_t < saved_t) {
                saved_intersectable.value().swap(new_intersection.value());
                saved_material = material;
            }
        } else {
            saved_intersectable = std::move(new_intersection.value());
            saved_material = material;
        }
    }
    if (saved_intersectable.has_value()) {
        std::tuple<std::unique_ptr<SurfaceIntersection>, Material const*>
            returned{};
        std::get<0>(returned) = std::move(saved_intersectable.value());
        std::get<1>(returned) = saved_material;
        return returned;
    }

    return {};
}

auto LightGraphNode::construct_with_material(
    std::vector<std::tuple<Intersectable const*, Material const*>> const& group,
    Material const* void_material
) -> void
{
    if (depth >= max_tree_depth) {
        material = void_material;
        return;
    }

    auto intersection_and_mat = intersect_group_with_materials(group, line);

    if (!intersection_and_mat.has_value()) {
        material = void_material;
        intersection = nullptr;
        return;
    }

    intersection.swap(std::get<0>(intersection_and_mat.value()));
    material = std::get<1>(intersection_and_mat.value());
    auto const btn_matrix = intersection->get_btn_matrix();
    std::array<Float, 3> const surface_normal{
        btn_matrix[0][2],
        btn_matrix[1][2],
        btn_matrix[2][2]
    };
    auto const uv = intersection->get_uv();

    bool const has_normal_texture = material->has_texture_normals();
    auto const normal{
        has_normal_texture
            ? btn_matrix * material->get_texture_normal(uv[0], uv[1])
            : surface_normal
    };

    // reflection
    auto const reflected_precent = material->get_reflect_precent();
    if (reflected_precent > 0.0) {
        Line const reflected_line = intersection->generate_ray(
            reflected_direction(line.direction, normal)
        );
        reflected = std::make_unique<LightGraphNode>(
            material,
            depth + 1,
            light_intensity * reflected_precent,
            reflected_line,
            this
        );
        reflected->construct_with_material(group, void_material);
    }

    // refraction
    auto const refracted_precent = material->get_refract_precent();
    if (refracted_precent > 0.0) {
        Line const refracted_line =
            intersection->generate_ray(refracted_direction(
                line.direction,
                normal,
                parent == nullptr ? enviroment_index_of_refraction
                                  : parent->material->get_index_of_refraction(),
                material->get_index_of_refraction()
            ));
        refracted = std::make_unique<LightGraphNode>(
            material,
            depth + 1,
            light_intensity * refracted_precent,
            refracted_line,
            this
        );
        refracted->construct_with_material(group, void_material);
    }
}

auto LightGraphNode::calculate_color(
    Camera const& camera,
    PointLight const& light,
    Float total_intensity
) const -> std::array<Float, 3>
{
    if (intersection) {
        std::array<Float, 3> final_color{};
        std::array<Float, 3> normal{};
        auto const btn_matrix = intersection->get_btn_matrix();
        std::array<Float, 3> const surface_normal{
            btn_matrix[0][2],
            btn_matrix[1][2],
            btn_matrix[2][2]
        };
        auto const uv = intersection->get_uv();
        auto const position = intersection->get_position();
        if (material->has_texture_normals()) {
            // convert to normal_coord system
            normal = btn_matrix * material->get_texture_normal(uv[0], uv[1]);
        } else {
            normal = surface_normal;
        }
        normal = normalize(normal);
        auto const view = normalize(camera.get_position() - position);
        auto const incoming = normalize(light.position - position);
        [[maybe_unused]] auto const outgoing =
            normalize(reflected_direction(incoming, normal));
        [[maybe_unused]] Float const cos_incident_angle = dot(incoming, normal);

        Float const diffuse = phong_diffuse(light.position, position, normal);
        Float const specular = blin_phong_specular(
            light.position,
            position,
            view,
            normal,
            material->get_specular_exponent()
        );

        final_color = (light_intensity / total_intensity) *
                      (material->get_specular_coeff() * specular *
                           material->get_specular_color() +
                       material->get_diffuse_coeff() * diffuse *
                           material->get_diffuse_color() +
                       material->get_ambient_coeff() *
                           material->get_ambient_color(uv[0], uv[1]));

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
    const -> std::array<Float, 3>
{
    if (intersection) {
        auto const btn_matrix = intersection->get_btn_matrix();
        std::array<Float, 3> const surface_normal{
            btn_matrix[0][2],
            btn_matrix[1][2],
            btn_matrix[2][2]
        };
        auto const uv = intersection->get_uv();
        auto const position = intersection->get_position();
        std::array<Float, 3> normal{};
        if (material->has_texture_normals()) {
            // convert to normal_coord system
            normal = btn_matrix * material->get_texture_normal(uv[0], uv[1]);
        } else {
            normal = surface_normal;
        }
        normal = normalize(normal);
        auto const outgoing = normalize(position - camera.get_position());
        auto const incoming = normalize(light.position - position);
        // auto const reflected = normalize(reflected_direction(incoming,
        // normal));
        auto const halfway = normalize((outgoing + incoming) * 0.5);
        Float const cos_incident_angle = dot(incoming, normal);

        // This is blin phong
        auto const brdf = []([[maybe_unused]] std::array<Float, 3> position,
                             [[maybe_unused]] std::array<Float, 3> incoming,
                             [[maybe_unused]] std::array<Float, 3> outgoing,
                             [[maybe_unused]] std::array<Float, 3> halfway,
                             [[maybe_unused]] std::array<Float, 3> normal,
                             Float shininess) -> std::array<double, 3> {
            constexpr std::array<Float, 3> c_diffuse{1, 1, 1};
            constexpr std::array<Float, 3> c_specular{1, 1, 1};
            return c_diffuse +
                   c_specular * std::pow(dot(normal, halfway), shininess);
        };

        std::array<Float, 3> const final_color{
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

auto LightGraphNode::sum_light_intensity() const -> Float
{
    Float intensity = 0.0;
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
    if (intersection) {
        ss << pad(depth) << "position: " << intersection->get_position()
           << "\n";
        ss << pad(depth) << "t: " << intersection->get_t() << "\n";
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
