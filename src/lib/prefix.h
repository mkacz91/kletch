#ifndef KLETCH_LIB_PREFIX_H
#define KLETCH_LIB_PREFIX_H

#include "../prefix.h"
#include <math/math.h>

namespace kletch {

template <typename T> static constexpr
real resolve_th(T user_th, float float_th, double double_th)
{
    return rl(user_th) < rl(0) ? rl(user_th) : rl(float_th, double_th);
}

} // namespace kletch

#endif // KLETCH_LIB_PREFIX_H
