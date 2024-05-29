#include "texture.h"
// #include <iostream>

Texture::Texture(
    std::string const& color_filepath,
    std::string const& uv_filepath
)
    : colors(0, 0), normals(0, 0)
{
    colors.load(color_filepath);
    normals.load(uv_filepath);
    // std::cout << colors.get_width() << " " << colors.get_height() << "\n";
    // std::cout << normals.get_width() << " " << normals.get_height() << "\n";
}
auto Texture::get_colors_width() const -> size_t { return colors.get_width(); }
auto Texture::get_colors_height() const -> size_t
{
    return colors.get_height();
}
auto Texture::get_normals_width() const -> size_t
{
    return normals.get_width();
}
auto Texture::get_normals_height() const -> size_t
{
    return normals.get_height();
}
auto Texture::get_colors_at(size_t ui, size_t vi) const -> std::array<double, 3>
{
    return from_color(colors.get_color_at(ui, vi));
}
auto Texture::get_normals_at(size_t ui, size_t vi) const
    -> std::array<double, 3>
{
    return from_tangent_space(from_color(normals.get_color_at(ui, vi)));
}
