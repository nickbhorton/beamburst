#ifndef BEAMBURST_MATERIAL_HEADER_
#define BEAMBURST_MATERIAL_HEADER_
#include <array>
#include <cmath>
#include <optional>

#include "cmath_extention.h"
#include "texture.h"

class Material
{
    std::array<double, 3> base_ambient_color;
    std::array<double, 3> diffuse_color;
    std::array<double, 3> specular_color;
    double ambient_coeff;
    double diffuse_coeff;
    double specular_coeff;

    double index_of_refraction;

    double reflect_precent;
    double refract_precent;

    double specular_exponent;

    Texture* texture;

public:
    Material() = default;
    auto set_base_ambient_color(std::array<double, 3> const& color)
    {
        base_ambient_color = color;
    }
    auto set_diffuse_color(std::array<double, 3> const& color)
    {
        diffuse_color = color;
    }
    auto set_specular_color(std::array<double, 3> const& color)
    {
        specular_color = color;
    }
    // ambient then diffuse then specular
    auto set_coeffs(std::array<double, 3> const& coeffs)
    {
        ambient_coeff = coeffs[0];
        diffuse_coeff = coeffs[1];
        specular_coeff = coeffs[2];
    }
    auto set_reflect_precent(double precent) { reflect_precent = precent; }
    auto set_refract_precent(double precent) { refract_precent = precent; }
    auto set_index_of_refraction(double index) { index_of_refraction = index; }
    auto set_specular_exponent(double exponent)
    {
        specular_exponent = exponent;
    }
    auto set_texture(Texture* texture) { this->texture = texture; }
    auto get_specular_exponent() const -> double { return specular_exponent; }
    auto get_reflect_precent() const -> double { return reflect_precent; }
    auto get_refract_precent() const -> double { return refract_precent; }
    auto get_index_of_refraction() const -> double
    {
        return index_of_refraction;
    }
    auto get_ambient_coeff() const -> double { return ambient_coeff; }
    auto get_diffuse_coeff() const -> double { return diffuse_coeff; }
    auto get_specular_coeff() const -> double { return specular_coeff; }
    auto get_ambient_color(double u, double v) const -> std::array<double, 3>
    {
        if (texture != nullptr) {
            u = clamp(u, 0, 0.99);
            v = clamp(v, 0, 0.99);
            size_t ui = std::floor(u * texture->get_colors_height());
            size_t vi = std::floor(v * texture->get_colors_height());
            return texture->get_colors_at(ui, vi);
        }
        return base_ambient_color;
    }
    auto get_diffuse_color() const -> std::array<double, 3>
    {
        return diffuse_color;
    }
    auto get_specular_color() const -> std::array<double, 3>
    {
        return specular_color;
    }
};

#endif
