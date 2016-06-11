#include "reference_fresnel.h"

namespace kletch {

vec2r ReferenceFresnel::eval_standard(real s)
{
    // Mark A. Heald, Rational Approximations for the Fresnel Integrals,
    // Mathematics of Computation 44 (1985), 459-461.
    // (With corrigenda)

    // A numerator coefficients
    constexpr real a0 = rl(1.00000000);
    constexpr real a1 = rl(0.19451610);
    constexpr real a2 = rl(0.23636410);
    constexpr real a3 = rl(0.06832400);
    constexpr real a4 = rl(0.02412120);
    // A denominator coefficients
    constexpr real b0 = rl(2.00000000);
    constexpr real b1 = rl(2.93550410);
    constexpr real b2 = rl(2.75702460);
    constexpr real b3 = rl(1.87572100);
    constexpr real b4 = rl(0.97811300);
    constexpr real b5 = rl(0.35668100);
    constexpr real b6 = rl(0.11824700);
    // R numerator coefficients
    constexpr real c0 = rl(1.00000000);
    constexpr real c1 = rl(0.77695070);
    constexpr real c2 = rl(0.64601170);
    constexpr real c3 = rl(0.34605090);
    constexpr real c4 = rl(0.13392590);
    constexpr real c5 = rl(0.04339950);
    // R denominator coefficients
    constexpr real d0 = rl(1.41421356);
    constexpr real d1 = rl(2.51298060);
    constexpr real d2 = rl(2.71967410);
    constexpr real d3 = rl(1.98405240);
    constexpr real d4 = rl(1.09173250);
    constexpr real d5 = rl(0.42052170);
    constexpr real d6 = rl(0.13634704);

    real an = a0 + s * (a1 + s * (a2 + s * (a3 + s * a4)));
    real ad = b0 + s * (b1 + s * (b2 + s * (b3 + s * (b4 + s * (b5 + s * b6)))));
    real a = an / ad;
    real rn = c0 + s * (c1 + s * (c2 + s * (c3 + s * (c4 + s * c5))));
    real rd = d0 + s * (d1 + s * (d2 + s * (d3 + s * (d4 + s * (d5 + s * d6)))));
    real r = rn / rd;
    real t = HALF_PI * (a - s * s);
    return vec2r(rl(0.5) - r * sin(t), rl(0.5) - r * cos(t));
}

} // namespace kletch
