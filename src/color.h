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
    friend bool operator<=>(const Color& c1, const Color& c2) = default;
};

auto to_color(const std::array<double, 3>& vec_color) -> Color;
double saturate(double in);

#endif
