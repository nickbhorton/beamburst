#ifndef BEAMBURST_LIGHT_GRAPH_HEADER_
#define BEAMBURST_LIGHT_GRAPH_HEADER_

#include <memory>

#include "camera.h"
#include "intersectable.h"
#include "lighting.h"
#include "line.h"
#include "material.h"

size_t constexpr max_tree_depth = 10;
double constexpr enviroment_index_of_refraction = 1.0;

typedef std::tuple<Intersectable*, Material const*> object_t;

struct LightGraphNode {
    LightGraphNode(
        Material const* material,
        size_t depth,
        double intensity,
        Line const& line,
        LightGraphNode* parent
    );

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
    ) -> void;
    auto construct_with_material(
        std::vector<std::tuple<Intersectable*, Material*>> const& os,
        Material const* bg_material,
        Intersectable const* remove_ptr = nullptr
    ) -> void;

    auto calculate_color(
        Camera const& camera,
        PointLight const& light,
        double total_intensity
    ) const -> std::array<double, 3>;
    auto count_nodes() const -> size_t;
    auto sum_light_intensity() const -> double;
    auto to_string() const -> std::string;
    auto to_string_helper(size_t depth, std::stringstream& ss) const -> void;
};

#endif
