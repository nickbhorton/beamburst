#ifndef BEAMBURST_MATERIAL_HEADER_
#define BEAMBURST_MATERIAL_HEADER_

#include <array>

struct Material {
    double index_of_refraction;
    double reflect_precent;
    double refract_precent;
    std::array<double, 3> ambient_color;
    std::array<double, 3> diffuse_color;
    std::array<double, 3> specular_color;
    double ambient_coeff;
    double diffuse_coeff;
    double specular_coeff;
};

#endif
