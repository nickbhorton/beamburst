#include "doctest.h"

#include "array_ops.h"
#include "linear_types.h"
#include "transformations.h"

using namespace linalg;
using namespace transformations;

TEST_CASE("scaling matrix scales a vector")
{
    mat4 sm = scaling_matrix(2.0);
    vec4 v = {1.0, 2.0, 3.0, 1.0};
    v = sm * v;
    CHECK(v[0] == 2.0);
    CHECK(v[1] == 4.0);
    CHECK(v[2] == 6.0);
    CHECK(v[3] == 1.0);

    sm = scaling_matrix(-4.0);
    v = {1.0, 2.0, 3.0, 1.0};
    v = sm * v;
    CHECK(v[0] == -4.0);
    CHECK(v[1] == -8.0);
    CHECK(v[2] == -12.0);
    CHECK(v[3] == 1.0);
}
