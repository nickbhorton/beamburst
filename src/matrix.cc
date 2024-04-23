#include "matrix.h"
Matrix::Matrix(size_t width, size_t height)
    : data_(width * height, 0.0), width_(width), height_(height)
{
}

double Matrix::at(size_t row, size_t col) const
{
    return data_.at(width_ * row + col);
}
