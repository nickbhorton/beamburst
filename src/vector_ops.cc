#include <algorithm>
#include <cassert>

#include "vector_ops.h"

std::vector<double>
operator+(const std::vector<double>& v1, const std::vector<double>& v2)
{
    assert(v1.size() == v2.size());
    std::vector<double> result(v1.size(), 0.0);
    std::ranges::transform(v1, v2, result.begin(), std::plus<double>{});
    return result;
}

std::vector<double>
operator-(const std::vector<double>& v1, const std::vector<double>& v2)
{
    assert(v1.size() == v2.size());
    std::vector<double> result(v1.size(), 0.0);
    std::ranges::transform(v1, v2, result.begin(), std::minus<double>{});
    return result;
}
std::vector<double> operator*(const std::vector<double>& v, double s)
{
    std::vector<double> result(v.size(), 0.0);
    std::ranges::transform(v, result.begin(), [s](double e) { return e * s; });
    return result;
}
std::vector<double> operator*(double s, const std::vector<double>& v)
{
    std::vector<double> result(v.size(), 0.0);
    std::ranges::transform(v, result.begin(), [s](double e) { return e * s; });
    return result;
}
std::vector<double> operator-(const std::vector<double>& v)
{
    std::vector<double> result(v.size(), 0.0);
    std::ranges::transform(v, result.begin(), [](double e) { return -e; });
    return result;
}
