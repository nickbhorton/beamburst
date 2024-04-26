#include <cmath>
#include <iostream>

#include "image.h"

int main()
{
    const double theta_max = 2.0 * M_PI;
    const double phi_max = M_PI;
    const size_t img_width = 512;
    const size_t img_height = static_cast<size_t>(
        static_cast<double>(img_width) * (phi_max / theta_max)
    );
    Image texture{{img_width, img_height}, Color(255, 255, 255, 255)};
    for (size_t i = 0; i < img_height; i++) {
        for (size_t j = 0; j < img_width; j++) {
            double x = static_cast<double>(j) / static_cast<double>(img_width);
            // double y = static_cast<double>(i) / static_cast<double>(img_height);
            texture.set_color_at(j, i, to_color({x, 0.0, 0.0}));
        }
    }
    texture.save("texture.png");
}