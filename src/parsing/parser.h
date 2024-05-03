#ifndef BEAMBURST_PARSER_HEADER_
#define BEAMBURST_PARSER_HEADER_

#include <array>
#include <istream>
#include <optional>
#include <tuple>
#include <vector>

typedef std::tuple<
    std::vector<std::array<double, 3>>,
    std::vector<std::array<double, 3>>,
    std::vector<std::array<double, 2>>,
    std::vector<std::vector<
        std::tuple<size_t, std::optional<size_t>, std::optional<size_t>>>>>
    obj_t;

auto basic_obj_parse(std::istream& input) -> obj_t;

#endif
