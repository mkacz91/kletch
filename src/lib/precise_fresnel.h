#ifndef KLETCH_LIB_PRECISE_FRESNEL_H
#define KLETCH_LIB_PRECISE_FRESNEL_H

#include "prefix.h"

namespace kletch {

// High precision evaluation of Fresnel-related integrals used by core algorithms
struct PreciseFresnel
{
    // Evaluates the general Fresnel integral `INT t = 0..s: exp(i (k0 t + 0.5 k1 t^2))`.
    static vec2r eval(real k0, real k1, real s);

    // Evaluates the second Fresnel moment `INT t = 0..s: t^2 exp(i (k0 t + 0.5 k1 t^2))`.
    static vec2r eval_m2(real k0, real k1, real s);
};

} // namespace kletch

#endif // KLETCH_LIB_PRECISE_FRESNEL_H
