#ifndef BEAMBURST_LINE_HEADER_
#define BEAMBURST_LINE_HEADER_

#include <array>

struct Line {
    std::array<double, 3> position;
    std::array<double, 3> direction;
};

auto solve_line(Line const& line, double t) -> std::array<double, 3>;

#endif
