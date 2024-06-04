#ifndef BEAMBURST_PARSER_HEADER_
#define BEAMBURST_PARSER_HEADER_

#include "primitives/triangle.h"

#include <array>
#include <istream>
#include <optional>
#include <vector>

class VertexAttributes
{
    size_t poition;
    std::optional<size_t> normal;
    std::optional<size_t> uv;

public:
    VertexAttributes(
        size_t position,
        std::optional<size_t> normal,
        std::optional<size_t> uv
    )
        : poition{position}, normal{normal}, uv{uv}
    {
    }
    size_t get_position_idx() const { return poition - 1; }
    bool has_normal_idx() const { return normal.has_value(); }
    size_t get_normal_idx() const { return normal.value() - 1; }
    bool has_uv_idx() const { return uv.has_value(); }
    size_t get_uv_idx() const { return uv.value() - 1; }
};

struct VertexObject {
private:
    auto parse_file(std::istream& input) -> void;
    std::vector<std::array<double, 3>> vertex_positions;
    std::vector<std::array<double, 3>> vertex_normals;
    std::vector<std::array<double, 2>> vertex_uvs;
    std::vector<std::vector<VertexAttributes>> faces;
    // does this imply rule of 5
    VertexObject();

public:
    VertexObject(std::istream& input);
    auto extract_triangles() const -> std::vector<Triangle>;

    auto copy_and_transform(
        std::array<std::array<double, 4>, 4> const& transformation_matrix
    ) const -> VertexObject;
};

#endif
