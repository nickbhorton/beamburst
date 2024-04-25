#ifndef BEAMBURST_TRANSFORMATIONS_HEADER_
#define BEAMBURST_TRANSFORMATIONS_HEADER_

#include <array>

namespace transformations
{

template <typename T>
auto constexpr scaling_matrix(T scalar) -> std::array<std::array<T, 4>, 4>
{
    return std::array<std::array<T, 4>, 4>{
        {{scalar, static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)},
         {static_cast<T>(0), scalar, static_cast<T>(0), static_cast<T>(0)},
         {static_cast<T>(0), static_cast<T>(0), scalar, static_cast<T>(0)},
         {static_cast<T>(0),
          static_cast<T>(0),
          static_cast<T>(0),
          static_cast<T>(1)}}
    };
}

template <typename T>
auto constexpr translation_matrix(std::array<T, 3> translation_vector)
    -> std::array<std::array<T, 4>, 4>
{
    return std::array<std::array<T, 4>, 4>{
        {{static_cast<T>(1),
          static_cast<T>(0),
          static_cast<T>(0),
          translation_vector[0]},
         {static_cast<T>(0),
          static_cast<T>(1),
          static_cast<T>(0),
          translation_vector[1]},
         {static_cast<T>(0),
          static_cast<T>(0),
          static_cast<T>(1),
          translation_vector[2]},
         {static_cast<T>(0),
          static_cast<T>(0),
          static_cast<T>(0),
          static_cast<T>(1)}}
    };
}

template <typename T>
auto constexpr transform(
    const std::array<std::array<T, 4>, 4>& m,
    const std::array<T, 3>& v
) -> std::array<T, 3>
{
    std::array<T, 4> v2 = {v[0], v[1], v[2], static_cast<T>(1)};
    v2 = m * v2;
    std::array<T, 3> result{v2[0], v2[1], v2[2]};
    return result;
}

} // namespace transformations

#endif
