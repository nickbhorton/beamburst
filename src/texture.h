#ifndef BEAMBURST_TEXTURE_HEADER_
#define BEAMBURST_TEXTURE_HEADER_

#include "image.h"
#include <string>
class Texture
{
    Image colors;
    Image normals;

public:
    Texture(std::string const& color_filepath, std::string const& uv_filepath);
    auto get_colors_width() const -> size_t;
    auto get_colors_height() const -> size_t;
    auto get_colors_at(size_t ui, size_t vi) const -> std::array<double, 3>;
    auto get_normals_width() const -> size_t;
    auto get_normals_height() const -> size_t;
    auto get_normals_at(size_t ui, size_t vi) const -> std::array<double, 3>;
};

#endif
