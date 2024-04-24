#ifndef BEAMBURST_VECTOR_OPS_HEADER_
#define BEAMBURST_VECTOR_OPS_HEADER_

#include <array>

namespace lineartypes
{
typedef std::array<double, 2> vec2f;
typedef std::array<double, 3> vec3f;
typedef std::array<double, 4> vec4f;

typedef std::array<std::array<double, 2>, 2> mat2f;
typedef std::array<std::array<double, 3>, 3> mat3f;
typedef std::array<std::array<double, 4>, 4> mat4f;
} // namespace lineartypes

#endif
