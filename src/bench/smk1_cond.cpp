#include "prefix.h"

#include <lib/fresnel_core.hpp>
#include <lib/heald.h>
#include <math/printing.h>

using namespace kletch;

inline vec2r fresnel_bgk1(real k0, real k1, real s)
{
    return FresnelCore<real>::eval_bgk1<Heald5>(k0, k1, s);
}

inline vec2r fresnel_smk1(real k0, real k1, real s)
{
    return FresnelCore<real>::eval_smk1_core(k0, k1, s, 0, rl(0));
}

int smk1_cond(int argc, char** argv)
{
    int const n = 1000;
    real const k1_start = rl(1e-3);
    real const k1_end = rl(1e-2);
    real const k0 = 1;
    real const s = 3;
    real const k1exp_start = log(k1_start);
    real const k1exp_end = log(k1_end);
    for (int i = 0; i <= n; ++i)
    {
        real k1exp = lerp(k1exp_start, k1exp_end, rl(i) / rl(n));
        real k1 = exp(k1exp);
        vec2r fbg = fresnel_bgk1(k0, k1, s);
        vec2r fsm = fresnel_smk1(k0, k1, s);
        cout << k1 << " " << 2 * dist(fbg, fsm) / (len(fbg) + len(fsm)) << "\n";
    }
    return 0;
}
