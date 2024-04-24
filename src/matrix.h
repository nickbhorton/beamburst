#ifndef BEAMBURST_MATRIX_HEADER_
#define BEAMBURST_MATRIX_HEADER_

#include <vector>

#include "vector_ops.h"

class Matrix
{
    size_t width_;
    size_t height_;
    std::vector<double> data_;
    Matrix(size_t width, size_t height, std::vector<double> data);

public:
    Matrix(size_t width, size_t height);
    Matrix(const std::vector<std::vector<double>>& vv);
    double at(size_t row, size_t col) const;
    void set(size_t row, size_t col, double val);
    size_t get_width() const;
    size_t get_height() const;
    size_t size() const;
    friend Matrix operator+(const Matrix& m1, const Matrix& m2);
    friend Matrix operator-(const Matrix& m1, const Matrix& m2);
    friend Matrix operator-(const Matrix& m);
    friend Matrix operator*(const Matrix& m, double s);
    friend Matrix operator*(double s, const Matrix& m);
    friend Matrix operator*(const Matrix& A, const Matrix& B);
    friend bool operator==(const Matrix& A, const Matrix& B);
};

#endif
