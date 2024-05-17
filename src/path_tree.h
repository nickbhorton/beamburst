#ifndef BEAMBURST_PATH_TREE_HEADER_
#define BEAMBURST_PATH_TREE_HEADER_

#include <memory>

#include "array_ops.h"
#include "camera.h"
#include "color.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"

size_t constexpr max_tree_depth = 10;
double constexpr enviroment_index_of_refraction = 1.0;

struct Material {
    double index_of_refraction;
    double reflect_precent;
    double refract_precent;
};

struct LightGraphNode {
    // copying line which is 6 doubles
    LightGraphNode(
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

    size_t depth;
    double light_intensity;
    Line line;
    std::optional<intersection_t> intersection;
    std::unique_ptr<LightGraphNode> reflected;
    std::unique_ptr<LightGraphNode> refracted;
    LightGraphNode* parent;

    Material const* material;

    auto construct(
        std::vector<Intersectable*> const& is,
        Intersectable const* remove_ptr = nullptr
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
                        parent == nullptr
                            ? enviroment_index_of_refraction
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

    auto calculate_color(Camera const& camera, PointLight const& light) const
        -> std::array<double, 3>
    {
        std::array<double, 3> vcol{0, 0, 0};
        if (intersection.has_value()) {
            auto const& [t, normal, uv_opt, iptr] = intersection.value();
            std::array<double, 3> const position = solve_line(line, t);
            std::array<double, 3> const view = camera.get_position() - position;
            double const diffuse =
                phong_diffuse(light.position, position, normal);
            double const specular = blin_phong_specular(
                light.position,
                position,
                view,
                normal,
                100.0
            );

            std::array<double, 3> constexpr ambient_color = {0.0, 0.0, 1.0};
            double constexpr ambient_power = 0.1;
            double constexpr diffuse_power = 0.5;
            double constexpr specular_power = 0.4;

            vcol = light_intensity * (specular_power * specular * color::white +
                                      diffuse_power * diffuse * color::white +
                                      ambient_power * ambient_color);
            if (refracted) {
                vcol = vcol + refracted->calculate_color(camera, light);
            }
            if (reflected) {
                vcol = vcol + reflected->calculate_color(camera, light);
            }
        }
        return vcol;
    }
};

struct LightGraph {
    std::unique_ptr<LightGraphNode> root;
};

#endif
