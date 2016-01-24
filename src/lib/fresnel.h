#ifndef KLETCH_FRESNEL_H
#define KLETCH_FRESNEL_H

#include "prefix.h"

namespace kletch {

class Fresnel
{
public:
    static real eval_c(real s);

    static real eval_s(real s);

    static vec2r eval(real s);

    static vec2r eval(real theta0, real kappa0, real a, real s);

private:
    static constexpr real PI = rl(3.14159265359);
    static constexpr real PI_OVER_2 = rl(1.57079632679);
    static constexpr real SQRT_2 = rl(1.41421356237);

    static real eval_a(real s);

    static real eval_r(real s);
};

inline real Fresnel::eval_c(real s)
{
    return rl(0.5) - eval_r(s) * sin(PI_OVER_2 * (eval_a(s) - s * s));
}

inline real Fresnel::eval_s(real s)
{
    return rl(0.5) - eval_r(s) * cos(PI_OVER_2 * (eval_a(s) - s * s));
}

inline vec2r Fresnel::eval(real s)
{
    real r = eval_r(s);
    real u = PI_OVER_2 * (eval_a(s) - s * s);
    return vec2r(rl(0.5) - r * sin(u), rl(0.5) - r * cos(u));
}

inline real Fresnel::eval_a(real s)
{
    real ntor = rl(1.0);
    real dtor = rl(2.0) + s * (rl(2.524) + s * (rl(1.886) + s * rl(0.803)));
    return ntor / dtor;
}

inline real Fresnel::eval_r(real s)
{
    real ntor = rl(0.506) * s + rl(1.0);
    real dtor = SQRT_2 + s * (rl(2.054) + s * rl(1.790));
    return ntor / dtor;
}

} // namespace kletch

#endif // KLETCH_FRESNEL_H
