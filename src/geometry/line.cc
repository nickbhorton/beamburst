#include "line.h"
#include "array_ops.h"

auto solve_line(Line const& line, double t) -> std::array<double, 3>
{
    return line.position + line.direction * t;
}
