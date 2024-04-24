#include <algorithm>
#include <cassert>
#include <numeric>

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

double dot(const std::vector<double>& v1, const std::vector<double>& v2)
{
    return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
}

std::vector<double>
cross(const std::vector<double>& v1, const std::vector<double>& v2)
{
    assert(v1.size() == 3);
    assert(v2.size() == 3);
    std::vector<double> result(3, 0.0);
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
    return result;
}

auto normalize(const std::vector<double>& v) -> std::vector<double>
{
    std::vector<double> result(v.size(), 0.0);
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    std::ranges::transform(v.begin(), v.end(), result.begin(), [sum](double e) {
        return e / sum;
    });
    return result;
}

