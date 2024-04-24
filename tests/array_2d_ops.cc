#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "array_ops.h"
#include "linear_types.h"

using namespace lineartypes;

TEST_CASE("2d array addition")
{
    mat2 m1 = {{{1, 2}, {3, 4}}};
    mat2 m2 = m1 + m1;

    CHECK(m2[0][0] == 2);
    CHECK(m2[0][1] == 4);
    CHECK(m2[1][0] == 6);
    CHECK(m2[1][1] == 8);

    std::array<std::array<int, 3>, 2> m3 = {{{1, 2, 3}, {4, 5, 6}}};
    std::array<std::array<int, 3>, 2> m4 = m3 + m3;

    CHECK(m4[0][0] == 2);
    CHECK(m4[0][1] == 4);
    CHECK(m4[0][2] == 6);
    CHECK(m4[1][0] == 8);
    CHECK(m4[1][1] == 10);
    CHECK(m4[1][2] == 12);

    std::array<std::array<int, 2>, 3> m5 = {{{1, 2}, {3, 4}, {5, 6}}};
    std::array<std::array<int, 2>, 3> m6 = m5 + m5;

    CHECK(m6[0][0] == 2);
    CHECK(m6[0][1] == 4);
    CHECK(m6[1][0] == 6);
    CHECK(m6[1][1] == 8);
    CHECK(m6[2][0] == 10);
    CHECK(m6[2][1] == 12);
}

TEST_CASE("2d array subtraction")
{
    mat2 m1 = {{{1, 2}, {3, 4}}};
    mat2 m12 = {{{4, 2}, {1, 1}}};
    mat2 m2 = m1 - m12;

    CHECK(m2[0][0] == -3);
    CHECK(m2[0][1] == 0);
    CHECK(m2[1][0] == 2);
    CHECK(m2[1][1] == 3);
}

TEST_CASE("2d array negation")
{
    mat2 m1 = {{{1, 2}, {3, 4}}};
    mat2 m2 = -m1;

    CHECK(m2[0][0] == -1);
    CHECK(m2[0][1] == -2);
    CHECK(m2[1][0] == -3);
    CHECK(m2[1][1] == -4);
}

TEST_CASE("2d array scalar multiplication")
{
    mat2 m1 = {{{1, 2}, {3, 4}}};
    mat2 m2 = 2.0 * m1;

    CHECK(m2[0][0] == 2);
    CHECK(m2[0][1] == 4);
    CHECK(m2[1][0] == 6);
    CHECK(m2[1][1] == 8);

    m2 = m1 * 2.0;
    CHECK(m2[0][0] == 2);
    CHECK(m2[0][1] == 4);
    CHECK(m2[1][0] == 6);
    CHECK(m2[1][1] == 8);
}

TEST_CASE("2d array 1d array multiplication")
{
    mat2 m1 = {{{1, 2}, {3, 4}}};
    vec2 v1 = {1, 2};
    vec2 v2 = m1 * v1;

    CHECK(v2[0] == 5);
    CHECK(v2[1] == 11);
}

TEST_CASE("2d array 2d array multiplication")
{
    std::array<std::array<int, 3>, 2> m1 = {{{1, 2, 3}, {4, 5, 6}}};
    std::array<std::array<int, 2>, 3> m2 = {{{10, 11}, {20, 21}, {30, 31}}};
    std::array<std::array<int, 2>, 2> mr = m1 * m2;

    CHECK(mr[0][0] == 140);
    CHECK(mr[0][1] == 146);
    CHECK(mr[1][0] == 320);
    CHECK(mr[1][1] == 335);
}
