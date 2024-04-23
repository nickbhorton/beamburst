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
}
