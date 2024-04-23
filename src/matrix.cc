#include "matrix.h"
Matrix::Matrix(size_t width, size_t height)
    : data_(width * height, 0.0), width_(width), height_(height)
{
}

double Matrix::at(size_t row, size_t col) const
{
    return data_.at(width_ * row + col);
}

void Matrix::set(size_t row, size_t col, double val)
{
    data_[width_ * row + col] = val;
}
size_t Matrix::get_width() const {
    return width_;
}
size_t Matrix::get_height() const {
    return height_;
}
size_t Matrix::size() const {
    return data_.size();
}
