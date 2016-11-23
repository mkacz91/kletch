#include "clothoid_fitter.h"

#include "curve_window.h"
#include "precise_fresnel.h"
#include "printing.h"

namespace kletch {

ClothoidFitter::ClothoidFitter()
{

}

ClothoidFitter::Result ClothoidFitter::fit(CurveWindow const& curve, int start, int end)
{
    int count = end - start;
    if (count <= 1)
        return { 0, 0, 0, unx2r(), 0 };
    vec2r p0 = curve[start].p;
    auto const& last_sample = curve[end - 1];
    if (count == 2)
        return { 0, 0, last_sample.s, span(p0, last_sample.p) / last_sample.s, 0 };

    // Fit curvature line minimizing the vertical least square error. Only inner samples have valid
    // curvature estimate.
    int n = count - 2;
    real sum_s = 0, sum_k = 0, sum_ss = 0, sum_sk = 0;
    for (int i = start + 1; i < end - 1; ++i)
    {
        auto const& sample = curve[i];
        real k = sample.k;
        sum_s += sample.s;
        sum_k += sample.k;
        sum_ss += sample.s * sample.s;
        sum_sk += sample.s * sample.k;
    }
    real k1 = (n * sum_sk - sum_s * sum_k) / (n * sum_ss - sum_s * sum_s);
    real k0 = (sum_k - sum_s * k1) / n;
    if (isbroken(k1))
    {
        k0 = sum_k / n;
        k1 = 0;
    }

    // Find rotation of evals `u_i` optimizing the least squares error against samples `v_i`. The
    // rotation angle `alpha` yields optimal error when
    //
    //     sin(alpha) = Y / sqrt(X^2 + Y^2),  cos(alpha) = X / sqrt(X^2 + Y^2),
    //
    // where
    //
    //     X = SUM dot(u_i, v_i), Y = SUM per(u_i, v_i).
    //
    // The error itself is then
    //
    //     cost = SUM ||u_i||^2 + ||v_i||^2 - 2 sqrt(X^2 + Y^2).
    vec2r rotation = 0;
    real cost = 0;
    for (int i = start + 1; i < end; ++i)
    {
        auto const& sample = curve[i];
        vec2r u = PreciseFresnel::eval(k0, k1, sample.s), v = sample.p - p0;
        rotation.x += dot(u, v);
        rotation.y += per(u, v);
        cost += len_sq(u) + len_sq(v);
    }
    real rlen = len(rotation);
    rotation /= rlen;
    cost -= 2 * rlen;

    return { k0, k1, last_sample.s, rotation, cost };
}

} // namespace kletch
