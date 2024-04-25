#ifndef BEAMBURST_TRANSFORMATIONS_HEADER_
#define BEAMBURST_TRANSFORMATIONS_HEADER_

#include <array>

namespace transformations
{

template <typename T>
auto scaling_matrix(T scalar) -> std::array<std::array<T, 4>, 4>
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
auto translation_matrix(std::array<T, 3> translation_vector)
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

} // namespace transformations

#endif
