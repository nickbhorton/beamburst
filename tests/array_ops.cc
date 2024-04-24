#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "array_ops.h"

TEST_CASE("std array addtion")
{
    std::array<double, 3> v1 = {1, 2, 3};
    std::array<double, 3> v2 = {1, 2, 3};
    std::array<double, 3> v3 = v1 + v2;
    CHECK(v3[0] == 2.0);
    CHECK(v3[1] == 4.0);
    CHECK(v3[2] == 6.0);

    v3 = v1 + v1;
    CHECK(v3[0] == 2.0);
    CHECK(v3[1] == 4.0);
    CHECK(v3[2] == 6.0);

    v3 = v3 + v3;
    CHECK(v3[0] == 4.0);
    CHECK(v3[1] == 8.0);
    CHECK(v3[2] == 12.0);

}

TEST_CASE("std array subtraction")
{
    std::array<double, 3> v1 = {1, 2, 3};
    std::array<double, 3> v2 = {1, 2, 3};
    std::array<double, 3> v3 = v1 - v2;
    CHECK(v3[0] == 0.0);
    CHECK(v3[1] == 0.0);
    CHECK(v3[2] == 0.0);
    std::array<double, 3> v4 = {4, 4, 4};
    v3 = v1 - v4;
    CHECK(v3[0] == -3.0);
    CHECK(v3[1] == -2.0);
    CHECK(v3[2] == -1.0);
}

TEST_CASE("std array scalar mult")
{
    std::array<double, 3> v1 = {1, 2, 3};
    std::array<double, 3> v3 = v1 * 2.0;
    CHECK(v3[0] == 2.0);
    CHECK(v3[1] == 4.0);
    CHECK(v3[2] == 6.0);
    v3 = 2.0 * v1;
    CHECK(v3[0] == 2.0);
    CHECK(v3[1] == 4.0);
    CHECK(v3[2] == 6.0);
}

TEST_CASE("std array negation")
{
    std::array<double, 3> v1 = {1, 2, 3};
    std::array<double, 3> v2 = -v1;

    CHECK(v2[0] == -1.0);
    CHECK(v2[1] == -2.0);
    CHECK(v2[2] == -3.0);
}

TEST_CASE("std array dot product")
{
    std::array<double, 3> v1 = {1, 2, 3};
    std::array<double, 3> v2 = {1, 1, 1};

    CHECK(dot(v1, v1) == 14);
    CHECK(dot(v2, v2) == 3);
    CHECK(dot(v2, v1) == 6);
    CHECK(dot(v1, v2) == 6);
}

TEST_CASE("std array cross product")
{
    std::array<double, 3> x = {1, 0, 0};
    std::array<double, 3> y = {0, 1, 0};
    std::array<double, 3> z = {0, 0, 1};
    CHECK(cross(x, y) == z);
    CHECK(cross(y, x) == -z);
    CHECK(cross(x, z) == -y);
    CHECK(cross(z, x) == y);
}

TEST_CASE("std array normalization")
{
    std::array<double, 3> x = {4, 0, 0};
    std::array<double, 3> xnorm = normalize(x);
    CHECK(xnorm[0] == 1.0);
    CHECK(xnorm[1] == 0.0);
    CHECK(xnorm[2] == 0.0);

    x = {0, 0, 0};
    xnorm = normalize(x);
    // nan criterion for ieee
    CHECK(xnorm[0] != xnorm[0]);
    CHECK(xnorm[1] != xnorm[1]);
    CHECK(xnorm[2] != xnorm[2]);
}
