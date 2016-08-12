#ifndef KLETCH_LIB_FRESNEL_HPP
#define KLETCH_LIB_FRESNEL_HPP

namespace kletch {

struct FresnelThresholds;

// Various elementary Fresnel integral evaluation routines.
struct FresnelCore
{
    // Evaluates the general Fresnel integral `INT t = 0..s: exp(i (k0 t + 0.5 k1 t^2))`.
    template <class T, class StandardFresnel> static
    vec2<T> eval(T k0, T k1, T s, FresnelThresholds const& th);

    // Evaluates the general Fresnel integral `INT t = 0..s: exp(i (k0 t + 0.5 k1 t^2))` assuming
    // `k1` is reasonably large.
    template <class T, class StandardFresnel> static
    vec2<T> eval_bkg1(T k0, T k1, T s);

    // Evaluates the general Fresnel moment `INT t = 0..s: t^n exp(i (k0 t + 0.5 k1 t^2))` divided
    // by `s^n` assuming `k1` is very small. The `s^n` term is appended when `n` is statically
    // known.
    template <class T> static
    vec2<T> eval_smk1_core(T k0, T k1, T s, int n, T th0, T th1);
};

// TODO: doc
struct FresnelThresholds
{
    T th_n0 = 0;
    T th0_smk1_n0 = inf<T>();
    T th1_smk1_n0 = inf<T>();
};

template <class T, class StandardFresnel> inline
vec2<T> FresnelCore::eval(T k0, T k1, T s, FresnelThresholds const& th)
{
    // TODO
    return 0;
}

template <class T, class StandardFresnel> inline
vec2<T> FresnelCore::eval_bgk1(T k0, T k1, T s);
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

template <class T> inline
vec2<T> FresnelCore::eval_smk1_core(T k0, T k1, T s, int n, T th0, T th1)
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

    vec2<T> f = 0;
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
            T dg = bl / dl;
            T r;
            switch (l & 3)
            {
                case 0: r = g.y += dg; break;
                case 1: r = g.x -= dg; break;
                case 2: r = g.y -= dg; break;
                case 3: r = g.x += dg; break;
            }
            if (dg <= th1 * abs(r))
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
        if (abs(df.x) <= th0 * abs(f.x) && abs(df.y) <= th0 * abs(f.y))
            break;
        ++m;
        am *= a / m;
        dm += 2;
    }

    f *= s;
    return f;
}

} // namespace kletch

#endif // KLETCH_LIB_FRESNEL_HPP
