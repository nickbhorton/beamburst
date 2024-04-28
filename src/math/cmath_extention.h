#ifndef BEAMBURST_CMATH_EXTENTION_HEADER_
#define BEAMBURST_CMATH_EXTENTION_HEADER_

#include <array>
double clamp(double x, double min, double max);
double smoothstep(double edge0, double edge1, double x);
double saturate(double in);
double mod(double in, double mod_by);
double fract(double in);
auto ucos(double x, double frequency) -> double;
auto gaussian(double x, double y, double s, double a, double x0, double y0)
    -> double;
auto gaussian_normal(
    double x,
    double y,
    double s,
    double a,
    double x0,
    double y0
) -> std::array<double, 3>;

#endif
