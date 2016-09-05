#ifndef KLETCH_DISPLAY_FRESNEL_H
#define KLETCH_DISPLAY_FRESNEL_H

#include "prefix.h"

namespace kletch {

class DisplayFresnel
{
public:
    static real eval_c(real s);

    static real eval_s(real s);

    static vec2r eval(real s);

    static vec2r eval(real theta0, real kappa0, real a, real s);

    static vec2r eval(real a, real s);

    static vec2r eval1(real a, real s);

    static mat2r jacobian1(real a, real s);

private:
    static constexpr real TWO_PI = rl(6.28318530718);
    static constexpr real PI = rl(3.14159265359);
    static constexpr real HALF_PI = rl(1.57079632679);
    static constexpr real SQRT_2 = rl(1.41421356237);

    static real eval_a(real s);

    static real eval_r(real s);
};

inline real DisplayFresnel::eval_c(real s)
{
    return rl(0.5) - eval_r(s) * sin(HALF_PI * (eval_a(s) - s * s));
}

inline real DisplayFresnel::eval_s(real s)
{
    return rl(0.5) - eval_r(s) * cos(HALF_PI * (eval_a(s) - s * s));
}

inline vec2r DisplayFresnel::eval(real s)
{
    real sgn_s = sgn(s);
    s = abs(s);
    real r = eval_r(s);
    real u = HALF_PI * (eval_a(s) - s * s);
    return sgn_s * vec2r(rl(0.5) - r * sin(u), rl(0.5) - r * cos(u));
}

inline real DisplayFresnel::eval_a(real s)
{
    real ntor = rl(1.0);
    real dtor = rl(2.0) + s * (rl(2.524) + s * (rl(1.886) + s * rl(0.803)));
    return ntor / dtor;
}

inline real DisplayFresnel::eval_r(real s)
{
    real ntor = rl(0.506) * s + rl(1.0);
    real dtor = SQRT_2 + s * (rl(2.054) + s * rl(1.790));
    return ntor / dtor;
}

} // namespace kletch

#endif // KLETCH_DISPLAY_FRESNEL_H
