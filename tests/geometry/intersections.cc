#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "intersections.h"
#include "plane.h"

TEST_CASE("basic line plane intersections")
{
    Line l{.position = {0, 0, 0}, .direction = {1, 0, 0}};
    Plane p{.position = {5, 0, 0}, .normal = {1, 0, 0}};
    REQUIRE(find_intersection(l, p).has_value());
    double t = find_intersection(l, p).value();
    CHECK(t == 5);
}
