#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "image.h"

TEST_CASE("save and load a basic png")
{
    Image img1{32, 64};
    for (size_t y = 0; y < img1.get_height(); y++) {
        for (size_t x = 0; x < img1.get_height(); x++) {
            img1.set_color_at(x, y, Color(x, y, 0, 255));
        }
    }
    img1.save("test.png");
    Image img2{0, 0};
    img2.load("test.png");
    REQUIRE(img1.get_height() == img2.get_height());
    REQUIRE(img1.get_width() == img2.get_width());
    for (size_t i = 0; i < img1.get_height(); i++) {
        for (size_t j = 0; j < img1.get_width(); j++) {
            CHECK(img1.get_color_at(i, j) == img2.get_color_at(i, j));
        }
    }
}
