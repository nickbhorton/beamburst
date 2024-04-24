#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "matrix.h"

TEST_CASE("matrix constructor fills matrix with zeros")
{
    Matrix m{10, 10};
    REQUIRE(m.get_width() == 10);
    REQUIRE(m.get_height() == 10);
    REQUIRE(m.size() == 100);
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            CHECK(m.at(i, j) == 0.0);
        }
    }
}

TEST_CASE("matrix set and at")
{
    Matrix m{10, 10};
    REQUIRE(m.get_width() == 10);
    REQUIRE(m.get_height() == 10);
    REQUIRE(m.size() == 100);
    m.set(0, 0, 10.0);
    CHECK(m.at(0, 0) == 10.0);
    m.set(9, 9, 10.0);
    CHECK(m.at(9, 9) == 10.0);
}

TEST_CASE("matrix from 2d vec")
{
    Matrix m({{1, 2}, {3, 4}});
    CHECK(m.at(0, 0) == 1);
    CHECK(m.at(0, 1) == 2);
    CHECK(m.at(1, 0) == 3);
    CHECK(m.at(1, 1) == 4);
    CHECK(m.get_height() == 2);
    CHECK(m.get_width() == 2);

    Matrix m2({{1, 2}, {3, 4}, {5, 6}});
    CHECK(m2.at(0, 0) == 1);
    CHECK(m2.at(0, 1) == 2);
    CHECK(m2.at(1, 0) == 3);
    CHECK(m2.at(1, 1) == 4);
    CHECK(m2.at(2, 0) == 5);
    CHECK(m2.at(2, 1) == 6);
    CHECK(m2.get_height() == 3);
    CHECK(m2.get_width() == 2);
}

TEST_CASE("matrix additon")
{
    Matrix m({{1, 2}, {3, 4}});
    Matrix m2 = m + m;
    CHECK(m2.at(0, 0) == 2);
    CHECK(m2.at(0, 1) == 4);
    CHECK(m2.at(1, 0) == 6);
    CHECK(m2.at(1, 1) == 8);
    CHECK(m2.get_height() == 2);
    CHECK(m2.get_width() == 2);
}

TEST_CASE("matrix subtraction")
{
    Matrix m({{1, 2}, {3, 4}});
    Matrix m2 = m - m;
    CHECK(m2.at(0, 0) == 0);
    CHECK(m2.at(0, 1) == 0);
    CHECK(m2.at(1, 0) == 0);
    CHECK(m2.at(1, 1) == 0);
    CHECK(m2.get_height() == 2);
    CHECK(m2.get_width() == 2);
}

TEST_CASE("matrix negation")
{
    Matrix m({{1, 2}, {3, 4}});
    Matrix m2 = -m;
    CHECK(m2.at(0, 0) == -1);
    CHECK(m2.at(0, 1) == -2);
    CHECK(m2.at(1, 0) == -3);
    CHECK(m2.at(1, 1) == -4);
    CHECK(m2.get_height() == 2);
    CHECK(m2.get_width() == 2);
}

TEST_CASE("matrix scalar mult")
{
    Matrix m({{1, 2}, {3, 4}});
    Matrix m2 = 2.0 * m;
    CHECK(m2.at(0, 0) == 2);
    CHECK(m2.at(0, 1) == 4);
    CHECK(m2.at(1, 0) == 6);
    CHECK(m2.at(1, 1) == 8);
    CHECK(m2.get_height() == 2);
    CHECK(m2.get_width() == 2);
    m2 = m * 2.0;
    CHECK(m2.at(0, 0) == 2);
    CHECK(m2.at(0, 1) == 4);
    CHECK(m2.at(1, 0) == 6);
    CHECK(m2.at(1, 1) == 8);
    CHECK(m2.get_height() == 2);
    CHECK(m2.get_width() == 2);
}

TEST_CASE("matrix multiplication")
{
    Matrix m1({{1, 2}, {3, 4}});
    Matrix m2({{1, 2}, {3, 4}});
    Matrix m3({{7, 10}, {15, 22}});
    CHECK((m1 * m2) == m3);
}
