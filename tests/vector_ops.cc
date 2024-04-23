#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "vector_ops.h"

TEST_CASE("vector addtion") {
    std::vector<double> v1 = {1, 2, 3};
    std::vector<double> v2 = {1, 2, 3};
    std::vector<double> v3 = v1 + v2;
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

TEST_CASE("vector subtraction") {
    std::vector<double> v1 = {1, 2, 3};
    std::vector<double> v2 = {1, 2, 3};
    std::vector<double> v3 = v1 - v2;
    CHECK(v3[0] == 0.0);
    CHECK(v3[1] == 0.0);
    CHECK(v3[2] == 0.0);
    std::vector<double> v4 = {4, 4, 4};
    v3 = v1 - v4;
    CHECK(v3[0] == -3.0);
    CHECK(v3[1] == -2.0);
    CHECK(v3[2] == -1.0);
}

TEST_CASE("vector scalar mult") {
    std::vector<double> v1 = {1, 2, 3};
    std::vector<double> v3 = v1 * 2.0;
    CHECK(v3[0] == 2.0);
    CHECK(v3[1] == 4.0);
    CHECK(v3[2] == 6.0);
    v3 = 2.0 * v1;
    CHECK(v3[0] == 2.0);
    CHECK(v3[1] == 4.0);
    CHECK(v3[2] == 6.0);
}

TEST_CASE("vector negation") {
    std::vector<double> v1 = {1, 2, 3};
    std::vector<double> v2 = -v1;

    CHECK(v2[0] == -1.0);
    CHECK(v2[1] == -2.0);
    CHECK(v2[2] == -3.0);
}
