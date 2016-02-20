#include "fresnel.h"

namespace kletch {

vec2r Fresnel::eval(real theta0, real kappa0, real a, real s)
{
    real c0 = sqrt(abs(a) / PI);
    real c1 = kappa0 / a;
    real s0 = c0 * (s + c1);
    real s1 = c0 * c1;
    vec2r result = (eval(s0) - eval(s1)) / c0;
    if (a < 0)
        result.y = -result.y;
    result.rotate(theta0 - kappa0 * kappa0 / (rl(2.0) * a));
    return result;
}

} // namespace kletch
