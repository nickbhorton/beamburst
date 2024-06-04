#ifndef BEAMBURST_TEXTURE_HEADER_
#define BEAMBURST_TEXTURE_HEADER_

#include "image.h"
#include <string>
class Texture
{
    Image texture;

public:
    Texture(std::string const& filepath);
    auto get_width() const -> size_t;
    auto get_height() const -> size_t;
    auto get_color_at(size_t ui, size_t vi) const -> std::array<double, 3>;
    auto get_normal_at(size_t ui, size_t vi) const -> std::array<double, 3>;
};

#endif
