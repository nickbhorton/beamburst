#ifndef BEAMBURST_MATRIX_HEADER_
#define BEAMBURST_MATRIX_HEADER_

#include <vector>

class Matrix
{
    size_t width_;
    size_t height_;
    std::vector<double> data_;

public:
    Matrix(size_t width, size_t height); 
    double at(size_t row, size_t col) const;
};

#endif
