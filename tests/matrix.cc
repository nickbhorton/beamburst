#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "matrix.h"

TEST_CASE("matrix constructor fills matrix with zeros")
{
    Matrix m{10, 10};
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            CHECK(m.at(i, j) == 0.0);
        }
    }
}
