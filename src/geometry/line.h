#ifndef BEAMBURST_LINE_HEADER_
#define BEAMBURST_LINE_HEADER_

#include <array>

struct Line {
    std::array<double, 3> position;
    std::array<double, 3> direction;
};

#endif
