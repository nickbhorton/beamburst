#include "texture.h"
// #include <iostream>

Texture::Texture(std::string const& filepath) : texture(0, 0)
{
    texture.load(filepath);
}
auto Texture::get_width() const -> size_t { return texture.get_width(); }
auto Texture::get_height() const -> size_t { return texture.get_height(); }
auto Texture::get_color_at(size_t ui, size_t vi) const -> std::array<double, 3>
{
    return from_color(texture.get_color_at(ui, vi));
}
auto Texture::get_normal_at(size_t ui, size_t vi) const -> std::array<double, 3>
{
    return from_tangent_space(from_color(texture.get_color_at(ui, vi)));
}
