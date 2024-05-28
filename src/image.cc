#include "image.h"

#include <iostream>
#include <memory>
#include <png.h>

Image::Image(size_t width, size_t height) : width_(width), height_(height)
{
    data_ = std::make_unique<std::vector<Color>>(
        width_ * height_,
        Color(0, 0, 0, 0)
    );
}

Image::Image(
    std::tuple<std::size_t, std::size_t> size,
    Color const& background_color
)
    : width_(std::get<0>(size)), height_(std::get<1>(size))
{
    data_ = std::make_unique<std::vector<Color>>(
        width_ * height_,
        background_color
    );
}

auto Image::set_color_at(size_t x, size_t y, Color const& color) -> bool
{
    if (x >= width_ || y >= height_) {
        return false;
    }
    size_t const data_index = x + width_ * y;
    data_->at(data_index).r = color.r;
    data_->at(data_index).g = color.g;
    data_->at(data_index).b = color.b;
    data_->at(data_index).a = color.a;
    return true;
}

auto Image::get_color_at(size_t x, size_t y) const -> Color
{
    size_t const data_index = x + width_ * y;
    return Color(
        data_->at(data_index).r,
        data_->at(data_index).g,
        data_->at(data_index).b,
        data_->at(data_index).r
    );
}

auto Image::fill(Color const& color) -> void
{
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            set_color_at(j, i, color);
        }
    }
}

auto Image::get_height() const -> size_t { return height_; }

auto Image::get_width() const -> size_t { return width_; }

auto Image::load(std::string const& filename) -> void
{
    FILE* fp = fopen(filename.c_str(), "rb");
    if (fp == NULL) {
        std::cerr << "fopen returned NULL\n";
        std::exit(1);
    }

    png_structp png =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
        abort();

    png_infop info = png_create_info_struct(png);
    if (!info)
        abort();

    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "an error has occured\n";
        std::exit(1);
    }

    png_init_io(png, fp);

    png_read_info(png, info);

    width_ = png_get_image_width(png, info);
    height_ = png_get_image_height(png, info);
    data_ = std::make_unique<std::vector<Color>>(
        width_ * height_,
        Color(0, 0, 0, 0)
    );
    png_byte color_type = png_get_color_type(png, info);
    // png_byte bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    /*
    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    */

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    /*
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);
    */

    png_read_update_info(png, info);

    png_bytep* row_pointers = NULL;
    if (row_pointers)
        abort();

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height_);
    for (size_t y = 0; y < height_; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);
    for (size_t y = 0; y < height_; y++) {
        for (size_t x = 0; x < width_; x++) {
            data_->at(width_ * y + x).r = row_pointers[y][x * 4 + 0];
            data_->at(width_ * y + x).g = row_pointers[y][x * 4 + 1];
            data_->at(width_ * y + x).b = row_pointers[y][x * 4 + 2];
            data_->at(width_ * y + x).a = row_pointers[y][x * 4 + 3];
        }
    }

    for (size_t y = 0; y < height_; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);
}

bool Image::save(std::string const& filename) const
{
    // for now these are just the default
    int constexpr bit_depth = 8;
    int constexpr color_channels = 4;
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

    for (size_t y = 0; y < height_; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(outfile);
    return true;
}
