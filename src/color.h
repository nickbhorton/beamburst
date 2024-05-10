#ifndef BEAMBURST_COLOR_HEADER_
#define BEAMBURST_COLOR_HEADER_

#include <array>
#include <cstdint>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    friend bool operator<=>(Color const& c1, Color const& c2) = default;
};

auto to_color(std::array<double, 3> const& vec_color) -> Color;

auto to_tangent_space(std::array<double, 3> const& v) -> std::array<double, 3>;

namespace color
{
constexpr std::array<double, 3> white = {1.0, 1.0, 1.0};
constexpr std::array<double, 3> black = {0.0, 0.0, 0.0};
constexpr std::array<double, 3> red = {1.0, 0.0, 0.0};
constexpr std::array<double, 3> green = {0.0, 1.0, 0.0};
constexpr std::array<double, 3> blue = {0.0, 0.0, 1.0};
} // namespace color

#endif
