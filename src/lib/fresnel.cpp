#include "fresnel.h"

namespace kletch {

vec2r Fresnel::eval(real theta0, real kappa0, real a, real s)
{
    real c0 = sqrt(abs(a) / PI);
    real c1 = kappa0 / a;
    real s0 = c0 * (s + c1);
    real s1 = c0 * c1;
    real theta = theta0 - kappa0 * kappa0 / (rl(2.0) * a);
    real sin_theta = sin(theta);
    real cos_theta = cos(theta);
    vec2r result = (eval(s0) - eval(s1)) / c0;
    result.y = copysign(result.y, a);
    result.x = cos_theta * result.x - sin_theta * result.y;
    result.y = sin_theta * result.x + cos_theta * result.y;
    return result;
}

} // namespace kletch
