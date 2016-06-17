#include "fresnel.h"

namespace kletch {

vec2r Fresnel::eval(real theta0, real kappa0, real a, real s)
{
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
            result.rot(theta0);
            return result;
        }
    }
    else // This is the normal case
    {
        real c0 = sqrt(abs(a) / PI);
        real s0 = c0 * (s + c1);
        real s1 = c0 * c1;
        vec2r result = (eval(s0) - eval(s1)) / c0;
        if (a < 0)
            result.y = -result.y;
        result.rot(theta0 - kappa0 * kappa0 / (rl(2.0) * a));
        return result;
    }
}

vec2r Fresnel::eval(real a, real s)
{
    const real c0 = sqrt(abs(a) / PI);
    return eval(c0 * s) / c0; // sgn(a)?
}

vec2r Fresnel::eval1(real a, real s)
{
    const real c0 = sqrt(abs(a) / PI);
    const real c1 = rl(1) / a;
    if (isinf(c1)) // This is circle
        return vec2r(sin(s), 1 - cos(s));
    real s0 = c0 * (s + c1);
    real s1 = c0 * c1;
    vec2r result = (eval(s0) - eval(s1)) / c0;
    if (a < 0)
        result.y = -result.y;
    result.rot(-rl(1) / (rl(2) * a));
    return result;
}

mat2r Fresnel::jacobian1(real a, real s)
{
    //  d          d
    // -- C(a,s)  -- C(a,s)
    // da         ds
    //
    //  d          d
    // -- S(a,s)  -- S(a,s)
    // da         ds

    const real theta = s * (rl(1) + rl(0.5) * a * s);
    const real c0 = a * s - 1;
    const real c1 = rl(1) / (rl(2) * a * a); // TODO: This is disastrous

    vec2r f = eval1(a, s); // = [ C(a, s), S(a, s) ]
    real cas = f.x, sas = f.y;

    real cos_theta = cos(theta), sin_theta = sin(theta);

    real dC_da = c1 * (rl(1) + c0 * cos_theta - sas - a * cas);
    real dS_da = c1 * (        c0 * sin_theta + cas - a * sas);
    real dC_ds = cos_theta - rl(1);
    real dS_ds = sin_theta;

    return mat2r(
        dC_da, dC_ds,
        dS_da, dS_ds
    );
}

} // namespace kletch
