#ifndef BEAMBURST_PATH_TREE_HEADER_
#define BEAMBURST_PATH_TREE_HEADER_

#include <memory>

#include "intersectable.h"
#include "line.h"

struct Material {
    double index_of_refraction;
};

struct LightGraphNode {
    // copying line which is 6 doubles
    LightGraphNode(
        Material const& material,
        size_t depth,
        double intensity,
        Line const& line
    )
        : depth(depth), light_intensity(intensity), line(line),
          intersection{0, {0, 0, 0}, {}, nullptr}, reflected{}, refracted{},
          parent{nullptr}, material(&material)
    {
    }

    size_t depth;
    double light_intensity;
    Line line;
    intersection_t intersection;
    std::unique_ptr<LightGraphNode> reflected;
    std::unique_ptr<LightGraphNode> refracted;
    LightGraphNode* parent;

    Material const* material;
};

struct LightGraph {
    std::unique_ptr<LightGraphNode> root;
};

#endif
