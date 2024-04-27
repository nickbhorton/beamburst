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

std::array<double, 3> to_tangent_space(const std::array<double, 3> v);

double saturate(double in);
double mod(double in, double mod_by);
double fract(double in);
auto ucos(double x, double frequency) -> double;
auto norm(double x, double y, double s, double x0, double y0) -> double;
auto grad_norm(double x, double y, double s, double x0, double y0, double a)
    -> std::array<double, 3>;

namespace color
{
constexpr std::array<double, 3> white = {1.0, 1.0, 1.0};
constexpr std::array<double, 3> black = {0.0, 0.0, 0.0};
constexpr std::array<double, 3> red = {1.0, 0.0, 0.0};
constexpr std::array<double, 3> green = {0.0, 1.0, 0.0};
constexpr std::array<double, 3> blue = {0.0, 0.0, 1.0};
} // namespace color

#endif
