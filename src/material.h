#ifndef BEAMBURST_MATERIAL_HEADER_
#define BEAMBURST_MATERIAL_HEADER_
#include <array>
#include <cmath>

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

    Texture* color_texture;
    Texture* normal_texture;

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
    auto set_color_texture(Texture* texture) { this->color_texture = texture; }
    auto set_normal_texture(Texture* texture)
    {
        this->normal_texture = texture;
    }
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
    auto get_base_ambient_color() const -> std::array<double, 3>
    {
        return base_ambient_color;
    }
    auto get_ambient_color(double u, double v) const -> std::array<double, 3>
    {
        if (color_texture != nullptr) {
            size_t const width = color_texture->get_width();
            size_t const height = color_texture->get_height();
            if (width > 0 && height > 0) {
                u = clamp(u, 0, 1);
                v = clamp(v, 0, 1);
                size_t const ui =
                    std::floor(u * (color_texture->get_width() - 1));
                size_t const vi =
                    std::floor(v * (color_texture->get_height() - 1));
                return color_texture->get_color_at(ui, vi);
            }
        }
        return base_ambient_color;
    }
    auto get_texture_normal(double u, double v) const -> std::array<double, 3>
    {
        if (color_texture != nullptr) {
            size_t const width = normal_texture->get_width();
            size_t const height = normal_texture->get_height();
            if (width > 0 && height > 0) {
                u = clamp(u, 0, 1);
                v = clamp(v, 0, 1);
                size_t const ui =
                    std::floor(u * (normal_texture->get_width() - 1));
                size_t const vi =
                    std::floor(v * (normal_texture->get_height() - 1));
                return normal_texture->get_normal_at(ui, vi);
            }
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
    auto has_texture_normals() const -> bool
    {
        if (normal_texture != nullptr && normal_texture->get_width() > 0 &&
            normal_texture->get_height() > 0) {
            return true;
        }
        return false;
    }
};

#endif
