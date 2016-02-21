#include "fresnel.h"

namespace kletch {

vec2r Fresnel::eval(real theta0, real kappa0, real a, real s)
{
    real c0 = sqrt(abs(a) / HALF_TAU);
    real c1 = kappa0 / a;
    real theta = theta0 - kappa0 * kappa0 / (rl(2.0) * a);
    if (isnan(c1) || isinf(c1) || isnan(theta) || isinf(theta)) // This is a circle or a line
    {
        real r = rl(1) / kappa0;
        if (isinf(r)) // This is a line
        {
            return vec2r(s * cos(theta0), s * sin(theta0));
        }
        else // This is a circle
        {
            // Note that negative kappa is handled magically by itself
            real angle = s * kappa0;
            vec2r result = { r * sin(angle), r - r * cos(angle) };
            result.rotate(theta0);
            return result;
        }
    }
    else // This is the normal case
    {
        real s0 = c0 * (s + c1);
        real s1 = c0 * c1;
        vec2r result = (eval(s0) - eval(s1)) / c0;
        if (a < 0)
            result.y = -result.y;
        result.rotate(theta0 - kappa0 * kappa0 / (rl(2.0) * a));
        return result;
    }
}

} // namespace kletch
