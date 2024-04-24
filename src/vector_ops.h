#ifndef BEAMBURST_VECTOR_OPS_HEADER_
#define BEAMBURST_VECTOR_OPS_HEADER_

#include <vector>

auto operator+(const std::vector<double>& v1, const std::vector<double>& v2)
    -> std::vector<double>;

auto operator-(const std::vector<double>& v1, const std::vector<double>& v2)
    -> std::vector<double>;

auto operator-(const std::vector<double>& v) -> std::vector<double>;

auto operator*(const std::vector<double>& v, double s) -> std::vector<double>;

auto operator*(double s, const std::vector<double>& v) -> std::vector<double>;

auto dot(const std::vector<double>& v1, const std::vector<double>& v2)
    -> double;

auto cross(const std::vector<double>& v1, const std::vector<double>& v2)
    -> std::vector<double>;

auto normalize(const std::vector<double>& v) -> std::vector<double>;

#endif
