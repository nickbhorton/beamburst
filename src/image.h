#ifndef BEAMBURST_IMAGE_HEADER_
#define BEAMBURST_IMAGE_HEADER_

#include <memory>
#include <vector>

#include "color.h"

class Image
{
    std::unique_ptr<std::vector<Color>> data_;
    size_t width_;
    size_t height_;

public:
    Image(size_t width, size_t height);
    Image(
        std::tuple<std::size_t, std::size_t> size,
        Color const& background_color
    );

    auto get_height() const -> size_t;
    auto get_width() const -> size_t;

    auto save(std::string const& filename) const -> bool;
    auto load(std::string const& filename) -> void;

    auto set_color_at(size_t x, size_t y, Color const& color) -> bool;
    auto get_color_at(size_t x, size_t y)  const -> Color;
    auto fill(Color const& color) -> void;
};

#endif
