#include "doctest.h"

#include "aabb.h"

TEST_CASE("AABB intersection")
{
    AABB a({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0});
    Line l{.position{0.5, 0.5, 1.5}, .direction{0.0, 0.0, -1.0}};
    auto result = a.find_intersection(l);
    REQUIRE(result.has_value());
    CHECK(result.value() == 0.5);
}
