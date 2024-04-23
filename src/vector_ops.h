#ifndef BEAMBURST_VECTOR_OPS_HEADER_
#define BEAMBURST_VECTOR_OPS_HEADER_

#include <vector>

std::vector<double>
operator+(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double>
operator-(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> operator-(const std::vector<double>& v);
std::vector<double> operator*(const std::vector<double>& v, double s);
std::vector<double> operator*(double s, const std::vector<double>& v);

#endif
