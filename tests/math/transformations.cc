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

TEST_CASE("tranlation matrix moves a vector")
{
    mat4 sm = translation_matrix<double>({1.0, 1.0, -1.0});
    vec4 v = {1.0, 2.0, 3.0, 1.0};
    v = sm * v;
    CHECK(v[0] == 2.0);
    CHECK(v[1] == 3.0);
    CHECK(v[2] == 2.0);
    CHECK(v[3] == 1.0);
}

TEST_CASE("tranlate and scale matrix")
{
    mat4 tm = translation_matrix<double>({1.0, 1.0, -1.0});
    mat4 sm = scaling_matrix(2.0);
    vec4 v = {1.0, 2.0, 3.0, 1.0};
    v = tm * sm * v;
    CHECK(v[0] == 3.0);
    CHECK(v[1] == 5.0);
    CHECK(v[2] == 5.0);
    CHECK(v[3] == 1.0);

    v = {1.0, 2.0, 3.0, 1.0};
    v = sm * tm * v;
    CHECK(v[0] == 4.0);
    CHECK(v[1] == 6.0);
    CHECK(v[2] == 4.0);
    CHECK(v[3] == 1.0);
}
