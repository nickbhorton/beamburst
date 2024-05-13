#ifndef BEAMBURST_BVH_HEADER_
#define BEAMBURST_BVH_HEADER_

#include "aabb.h"
#include <vector>

template <typename T> class BVHNode
{
    AABB volume;
    std::vector<T*> primative_list;
};

#endif
