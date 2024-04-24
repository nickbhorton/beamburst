#ifndef BEAMBURST_ARRAY_OPS_HEADER_
#define BEAMBURST_ARRAY_OPS_HEADER_

#include <algorithm>
#include <array>
#include <functional>
#include <numeric>

template <typename T, std::size_t N>
constexpr auto operator+(const std::array<T, N>& a1, const std::array<T, N>& a2)
    -> std::array<T, N>
{
    std::array<T, N> result{};
    std::ranges::transform(a1, a2, result.begin(), std::plus<T>{});
    return result;
}

template <typename T, std::size_t N>
constexpr auto operator-(const std::array<T, N>& a1, const std::array<T, N>& a2)
    -> std::array<T, N>
{
    std::array<T, N> result{};
    std::ranges::transform(a1, a2, result.begin(), std::minus<T>{});
    return result;
}

template <typename T, std::size_t N>
constexpr auto operator-(const std::array<T, N>& a1) -> std::array<T, N>
{
    std::array<T, N> result{};
    std::ranges::transform(a1, result.begin(), [](T e) { return -e; });
    return result;
}

template <typename T, std::size_t N>
constexpr auto operator*(const std::array<T, N>& a, const T v)
    -> std::array<T, N>
{
    std::array<T, N> result{};
    std::ranges::transform(a, result.begin(), [v](T e) { return v * e; });
    return result;
}

template <typename T, std::size_t N>
constexpr auto operator*(const T v, const std::array<T, N>& a)
    -> std::array<T, N>
{
    std::array<T, N> result{};
    std::ranges::transform(a, result.begin(), [v](T e) { return v * e; });
    return result;
}

template <typename T, std::size_t N>
constexpr auto dot(const std::array<T, N>& a1, const std::array<T, N>& a2) -> T
{
    return std::inner_product(
        a1.begin(),
        a1.end(),
        a2.begin(),
        static_cast<T>(0)
    );
}

template <typename T>
constexpr auto cross(const std::array<T, 3>& v1, const std::array<T, 3>& v2)
    -> std::array<T, 3>
{
    std::array<T, 3> result{};
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
    return result;
}

template <typename T, std::size_t N>
constexpr auto normalize(const std::array<T, N>& v) -> std::array<T, N>
{
    std::array<T, N> result{};
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    std::ranges::transform(v.begin(), v.end(), result.begin(), [sum](double e) {
        return e / sum;
    });
    return result;
}

#endif
