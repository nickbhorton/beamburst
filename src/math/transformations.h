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

} // namespace transformations

#endif
