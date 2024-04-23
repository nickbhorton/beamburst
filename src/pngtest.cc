#include "image.h"

int main()
{
    Image img{256, 256};
    for (size_t y = 0; y < 256; y++) {
        for (size_t x = 0; x < 256; x++) {
            img.set_color_at(x, y, Color(x, y, 0, 255));
        }
    }
    img.render("test.png");
}
