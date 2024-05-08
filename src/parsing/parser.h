#ifndef BEAMBURST_PARSER_HEADER_
#define BEAMBURST_PARSER_HEADER_

#include <array>
#include <istream>
#include <optional>
#include <tuple>
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

typedef std::tuple<
    std::vector<std::array<double, 3>>,
    std::vector<std::array<double, 3>>,
    std::vector<std::array<double, 2>>,
    std::vector<std::vector<VertexAttributes>>>
    obj_t;

auto basic_obj_parse(std::istream& input) -> obj_t;

#endif
