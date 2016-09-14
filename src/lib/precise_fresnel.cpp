#include "precise_fresnel.h"

#include "fresnel_core.hpp"
#include "heald.h"

namespace kletch {

vec2r PreciseFresnel::eval(real s)
{
    return Heald::eval5(s);
}

vec2r PreciseFresnel::eval(real k0, real k1, real s)
{
    // TODO: Find proper thresholds
    // TODO: Make sure the threshold indirection is optimized out by the compiler
    FresnelThresholds<real> th;
    th.th_n0 = rl(1e-2);
    th.th_smk1_n0 = rl(1e-7);
    return FresnelCore<real>::eval<Heald5>(k0, k1, s, th);
}

vec2r PreciseFresnel::eval_m2(real k0, real k1, real s)
{
    FresnelThresholds<real> th;
    th.th_n0 = rl(1e-2);
    th.th_smk1_n0 = rl(1e-7);
    return FresnelCore<real>::eval_m2<Heald5>(k0, k1, s, th);
}

} // namespace kletch
