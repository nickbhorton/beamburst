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

    bool set_color_at(size_t x, size_t y, const Color& color);
    Color get_color_at(size_t x, size_t y);
    void fill(const Color& color);

    size_t get_height() const;

    size_t get_width() const;

    bool save(const std::string& filename) const;
    void load(const std::string& filename);
};

#endif
