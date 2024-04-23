#include "image.h"

#include <png.h>

Image::Image(size_t width, size_t height) : width_(width), height_(height)
{
    data_ = std::make_unique<std::vector<Color>>(
        width_ * height_,
        Color(0, 0, 0, 0)
    );
}

bool Image::set_color_at(size_t x, size_t y, const Color& color)
{
    if (x >= width_ || y >= height_) {
        return false;
    }
    const size_t data_index = x + width_ * y;
    data_->at(data_index).r = color.r;
    data_->at(data_index).g = color.g;
    data_->at(data_index).b = color.b;
    data_->at(data_index).a = color.a;
    return true;
}

size_t Image::get_height() const { return height_; }

size_t Image::get_width() const { return width_; }

bool Image::render(const std::string& filename) const
{
    // for now these are just the default
    constexpr int bit_depth = 8;
    constexpr int color_channels = 4;
    FILE* outfile = fopen(filename.c_str(), "wb");

    png_structp png_ptr =
        png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "png_create_write_struct()\n";
        std::exit(1);
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        std::cerr << "png_create_info_struct()\n";
        std::exit(1);
    }

    png_init_io(png_ptr, outfile);
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width_,
        height_,
        bit_depth,
        PNG_COLOR_TYPE_RGB_ALPHA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_bytepp row_pointers =
        (png_bytepp)png_malloc(png_ptr, sizeof(png_bytepp) * height_);
    for (size_t i = 0; i < height_; i++) {
        row_pointers[i] =
            (png_bytep)png_malloc(png_ptr, width_ * color_channels);
    }
    for (size_t hi = 0; hi < height_; hi++) {
        for (size_t wi = 0; wi < width_; wi++) {
            // consider changing at() as its is bounds checked (i think)
            const size_t data_index = (wi) + (width_ * hi);
            row_pointers[hi][(wi * color_channels) + 0] =
                data_->at(data_index).r;
            row_pointers[hi][(wi * color_channels) + 1] =
                data_->at(data_index).g;
            row_pointers[hi][(wi * color_channels) + 2] =
                data_->at(data_index).b;
            row_pointers[hi][(wi * color_channels) + 3] =
                data_->at(data_index).a;
        }
    }

    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return true;
}
