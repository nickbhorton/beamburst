/// This is an insult to god but I like it.
#ifndef BEAMBURST_VECTOR_OPS_HEADER_
#define BEAMBURST_VECTOR_OPS_HEADER_

#include <array>

namespace lineartypes
{
typedef std::array<double, 2> vec2;
typedef std::array<double, 3> vec3;
typedef std::array<double, 4> vec4;

typedef std::array<int, 2> ivec2;
typedef std::array<int, 3> ivec3;
typedef std::array<int, 4> ivec4;

typedef std::array<unsigned int, 2> uvec2;
typedef std::array<unsigned int, 3> uvec3;
typedef std::array<unsigned int, 4> uvec4;

typedef std::array<std::array<double, 2>, 2> mat2;
typedef std::array<std::array<double, 3>, 3> mat3;
typedef std::array<std::array<double, 4>, 4> mat4;

typedef std::array<std::array<int, 2>, 2> imat2;
typedef std::array<std::array<int, 3>, 3> imat3;
typedef std::array<std::array<int, 4>, 4> imat4;

typedef std::array<std::array<unsigned int, 2>, 2> umat2;
typedef std::array<std::array<unsigned int, 3>, 3> umat3;
typedef std::array<std::array<unsigned int, 4>, 4> umat4;
} // namespace lineartypes

#endif
