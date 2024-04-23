#include "matrix.h"
#include <cassert>
Matrix::Matrix(size_t width, size_t height, std::vector<double> data)
    : data_(data), width_(width), height_(height)
{
    assert(width_ * height_ == data_.size());
}

Matrix::Matrix(size_t width, size_t height)
    : data_(width * height, 0.0), width_(width), height_(height)
{
}

Matrix::Matrix(const std::vector<std::vector<double>>& vv)
{
    assert(vv.size() >= 1);
    width_ = vv[0].size();
    height_ = vv.size();
    for (auto const& row : vv) {
        assert(row.size() == width_);
        for (auto const& e : row) {
            data_.push_back(e);
        }
    }
}

double Matrix::at(size_t row, size_t col) const
{
    return data_.at(width_ * row + col);
}

void Matrix::set(size_t row, size_t col, double val)
{
    data_[width_ * row + col] = val;
}
size_t Matrix::get_width() const { return width_; }
size_t Matrix::get_height() const { return height_; }
size_t Matrix::size() const { return data_.size(); }

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
    assert(m1.size() == m2.size());
    assert(m1.get_width() == m2.get_width());
    assert(m1.get_height() == m2.get_height());
    return Matrix(m1.width_, m1.height_, m1.data_ + m2.data_);
}

Matrix operator-(const Matrix& m1, const Matrix& m2) {
    assert(m1.size() == m2.size());
    assert(m1.get_width() == m2.get_width());
    assert(m1.get_height() == m2.get_height());
    return Matrix(m1.width_, m1.height_, m1.data_ - m2.data_);
}

Matrix operator-(const Matrix& m) {
    return Matrix(m.width_, m.height_,  -m.data_);
}

Matrix operator*(const Matrix& m, double s) {
    return Matrix(m.width_, m.height_,  s * m.data_);
}

Matrix operator*(double s, const Matrix& m) {
    return Matrix(m.width_, m.height_,  s * m.data_);
}
