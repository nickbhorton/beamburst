/// Vector and Matrix operations but make it constexpr ;)
#ifndef BEAMBURST_ARRAY_OPS_HEADER_
#define BEAMBURST_ARRAY_OPS_HEADER_

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>

template <typename T, std::size_t N>
constexpr auto operator<<(std::ostream& os, const std::array<T, N>& a)
    -> std::ostream&
{
    std::ranges::for_each(a, [&os](T e) { os << e << " "; });
    return os;
}

template <typename T, std::size_t N>
constexpr auto operator+(const std::array<T, N>& a1, const std::array<T, N>& a2)
    -> std::array<T, N>
{
    std::array<T, N> result{};
    std::ranges::transform(a1, a2, result.begin(), std::plus<T>{});
    return result;
}

template <typename T, std::size_t N, std::size_t M>
constexpr auto operator+(
    const std::array<std::array<T, M>, N>& m1,
    const std::array<std::array<T, M>, N>& m2
) -> std::array<std::array<T, M>, N>
{
    std::array<std::array<T, M>, N> result{};
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            result[i][j] = m1[i][j] + m2[i][j];
        }
    }
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

template <typename T, std::size_t N, std::size_t M>
constexpr auto operator-(
    const std::array<std::array<T, M>, N>& m1,
    const std::array<std::array<T, M>, N>& m2
) -> std::array<std::array<T, M>, N>
{
    std::array<std::array<T, M>, N> result{};
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            result[i][j] = m1[i][j] - m2[i][j];
        }
    }
    return result;
}

template <typename T, std::size_t N>
constexpr auto operator-(const std::array<T, N>& a1) -> std::array<T, N>
{
    std::array<T, N> result{};
    std::ranges::transform(a1, result.begin(), [](T e) { return -e; });
    return result;
}

template <typename T, std::size_t N, std::size_t M>
constexpr auto operator-(const std::array<std::array<T, M>, N>& m)
    -> std::array<std::array<T, M>, N>
{
    std::array<std::array<T, M>, N> result{};
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            result[i][j] = -m[i][j];
        }
    }
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

template <typename T, std::size_t N, std::size_t M>
constexpr auto operator*(const std::array<std::array<T, M>, N>& m, const T& v)
    -> std::array<std::array<T, M>, N>
{
    std::array<std::array<T, M>, N> result{};
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            result[i][j] = v * m[i][j];
        }
    }
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

template <typename T, std::size_t N, std::size_t M>
constexpr auto operator*(const T& v, const std::array<std::array<T, M>, N>& m)
    -> std::array<std::array<T, M>, N>
{
    std::array<std::array<T, M>, N> result{};
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            result[i][j] = v * m[i][j];
        }
    }
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
constexpr auto magnitude(const std::array<T, N>& v) -> T
{
    return std::sqrt(std::accumulate(v.begin(), v.end(), 0.0, [](T acc, T e) {
        return e * e + acc;
    }));
}

template <typename T, std::size_t N>
constexpr auto normalize(const std::array<T, N>& v) -> std::array<T, N>
{
    std::array<T, N> result{};
    const double length = magnitude(v);
    /*
    if (length == static_cast<T>(0)) {
        std::cerr << "normalization length was zero\n";
    }
    */
    std::ranges::transform(
        v.begin(),
        v.end(),
        result.begin(),
        [length](double e) { return e / length; }
    );
    return result;
}

template <typename T, std::size_t N>
constexpr auto operator*(
    const std::array<std::array<T, N>, N>& mat,
    const std::array<T, N>& vec
) -> std::array<T, N>
{
    std::array<T, N> result{};
    for (size_t i = 0; i < N; i++) {
        T sum = static_cast<T>(0);
        for (size_t j = 0; j < N; j++) {
            sum += mat[i][j] * vec[j];
        }
        result[i] = sum;
    }
    return result;
}

template <typename T, std::size_t N, std::size_t M>
constexpr auto operator*(
    const std::array<std::array<T, N>, M>& m1,
    const std::array<std::array<T, M>, N>& m2
) -> std::array<std::array<T, M>, M>
{
    std::array<std::array<T, M>, M> result{};
    for (size_t i = 0; i < M; i++) {
        for (size_t j = 0; j < M; j++) {
            T sum = static_cast<T>(0);
            for (size_t k = 0; k < N; k++) {
                sum = sum + m1[i][k] * m2[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

#endif
