#ifndef KLETCH_LIB_FRESNEL_HPP
#define KLETCH_LIB_FRESNEL_HPP

#include "prefix.h"
#include <vector>

namespace kletch {

template <class T>
struct FresnelThresholds;

// Various elementary Fresnel integral evaluation routines.
template <class T>
struct FresnelCore
{
    struct NopLog;

    // Evaluates the general Fresnel integral `INT t = 0..s: exp(i (k0 t + 0.5 k1 t^2))`.
    template <class StandardFresnel> static
    vec2<T> eval(T k0, T k1, T s, FresnelThresholds<T> const& th);

    template <class StandardFresnel> static
    vec2<T> eval_m2(T k0, T k1, T s, FresnelThresholds<T> const& th);

    // Evaluates the general Fresnel integral `INT t = 0..s: exp(i (k0 t + 0.5 k1 t^2))` assuming
    // `k1` is reasonably large.
    template <class StandardFresnel> static
    vec2<T> eval_bgk1(T k0, T k1, T s);

    // Evaluates the general Fresnel moment `INT t = 0..s: t^n exp(i (k0 t + 0.5 k1 t^2))` divided
    // by `s^n` assuming `k1` is very small. The `s^n` term is appended when `n` is statically
    // known.
    template <class Log = NopLog> static
    vec2<T> eval_smk1_core(T k0, T k1, T s, int n, T th);

    template <class StandardFresnel> static
    vec2<T> eval_m2_bgk1(T k0, T k1, T s);

    static vec2<T> eval_m2_smk1(T k0, T k1, T s, T th);

    // Zero overhead log that does nothing.
    struct NopLog
    {
        struct eval_smk1_core_log
        {
            void intermediate_result(vec2<T> const& value) { }
            void nested_iteration() { }
        };

        static eval_smk1_core_log* eval_smk1_core(T k0, T k1, T s, int n, T th) { return nullptr; }
    };
};

// TODO: doc
template <class T>
struct FresnelThresholds
{
    T th_n0 = 0;
    T th_smk1_n0 = inf<T>();
};

template <class T> template <class StandardFresnel> inline
vec2<T> FresnelCore<T>::eval(T k0, T k1, T s, FresnelThresholds<T> const& th)
{
    // TODO: find out best condition on k1
    return abs(k1) > th.th_n0
        ? eval_bgk1<StandardFresnel>(k0, k1, s)
        : eval_smk1_core(k0, k1, s, 0, th.th_smk1_n0);
}

template <class T> template <class StandardFresnel> inline
vec2<T> FresnelCore<T>::eval_m2(T k0, T k1, T s, FresnelThresholds<T> const& th)
{
    // TODO: Use separate threshold for n = 2. Or not?
    return abs(k1) > th.th_n0
        ? eval_m2_bgk1<StandardFresnel>(k0, k1, s)
        : eval_m2_smk1(k0, k1, s, th.th_smk1_n0);
}

template <class T> template <class StandardFresnel> inline
vec2<T> FresnelCore<T>::eval_bgk1(T k0, T k1, T s)
{
    T c0 = sqrt(abs(k1 / PI));
    T c1 = k0 / k1;
    T c2 = T(-0.5) * k0 * c1;
    T s0 = c0 * c1;
    T s1 = c0 * (s + c1);
    vec2<T> f0 = s0 >= 0 ? StandardFresnel::eval(s0) : -StandardFresnel::eval(-s0);
    vec2<T> f1 = s1 >= 0 ? StandardFresnel::eval(s1) : -StandardFresnel::eval(-s1);
    vec2<T> f = (f1 - f0) / c0;
    if (k1 < 0)
        f.y = -f.y;
    f.rot(c2);
    return f;
}

template <class T> template <class Log> inline
vec2<T> FresnelCore<T>::eval_smk1_core(T k0, T k1, T s, int n, T th)
{
    // We compute the integral
    //
    //     INT t = 0..s: t^n exp(i (k0 t + 0.5 k1 t^2)),
    //
    // using the following series expansion:
    //
    //     s^(n + 1) SUM m = 0..inf:
    //         1/m! (i k1 s^2 / 2)^m SUM l = 0..inf:
    //            (i k0 s)^l / (l! (n + 2 m + l + 1))
    //
    // and omit initial `s^n` term (as specified at declaration site).

    auto* log = Log::eval_smk1_core(k0, k1, s, n, th);

    vec2<T> f = 0;
    log->intermediate_result(f);
    T a = T(0.5) * k1 * s * s;
    T b = k0 * s;

    int m = 0;
    T am = 1;
    T dm = n + 1;
    while (true)
    {
        vec2<T> g = 0;
        int l = 0;
        T bl = 1;
        T dl = dm;
        while (true)
        {
            log->nested_iteration();
            T dg = bl / dl;
            T r;
            switch (l & 3)
            {
                case 0: r = g.x += dg; break;
                case 1: r = g.y += dg; break;
                case 2: r = g.x -= dg; break;
                case 3: r = g.y -= dg; break;
            }
            if (abs(dg) <= th * abs(r))
                break;
            ++l;
            bl *= b / l;
            dl += 1;
        }
        vec2r df;
        switch (m & 3)
        {
            case 0: df = {  am * g.x,  am * g.y }; break;
            case 1: df = { -am * g.y,  am * g.x }; break;
            case 2: df = { -am * g.x, -am * g.y }; break;
            case 3: df = {  am * g.y, -am * g.x }; break;
        }
        f += df;
        log->intermediate_result(f);

        if (abs(df.x) <= th * abs(f.x) && abs(df.y) <= th * abs(f.y))
            break;
        ++m;
        am *= a / m;
        dm += 2;
    }

    f *= s;
    return f;
}

template <class T> template <class StandardFresnel> inline
vec2<T> FresnelCore<T>::eval_m2_bgk1(T k0, T k1, T s)
{
    vec2<T> e = lhp(dir(s * (k0 + T(0.5) * k1 * s)));
    vec2<T> f0 = eval_bgk1<StandardFresnel>(k0, k1, s);
    vec2<T> f1 = (uny2<T>() - e - k0 * f0) / k1;
    return (lhp(f0) - k0 * f1 - s * e) / k1;
}

template <class T>
vec2<T> FresnelCore<T>::eval_m2_smk1(T k0, T k1, T s, T th)
{
    return s * s * eval_smk1_core(k0, k1, s, 2, th);
}

} // namespace kletch

#endif // KLETCH_LIB_FRESNEL_HPP
