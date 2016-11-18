#include "clothoid_fitter.h"
#include "printing.h"

#include <lib/precise_fresnel.h>

namespace kletch {

ClothoidFitter::Sample ClothoidFitter::ORIGIN_SAMPLE = { 0, 0, 0 };

ClothoidFitter::ClothoidFitter()
{
    clear();
}

void ClothoidFitter::clear()
{
    m_samples.clear();
}

void ClothoidFitter::push(vec2r const& p)
{
    int i = m_samples.size();
    m_samples.push_back({ nan<real>(), 0, p });
    Sample& sample2 = m_samples[i];
    Sample& sample1 = i == 0 ? ORIGIN_SAMPLE : m_samples[i - 1];

    // Compute arc length parameter of new sample
    vec2r v1 = span(sample1.p, sample2.p);
    real len_sq1 = len_sq(v1);
    real len1 = sqrt(len_sq1);
    sample2.s = sample1.s + len1;

    if (i == 0)
        return;
    Sample const& sample0 = i == 1 ? ORIGIN_SAMPLE : m_samples[i - 2];

    // Estimate curvature at previous sample. The curvature estimate at point `p_i` in the
    // direction of the subsequent point `p_j` is given by
    //
    //     k_i = 2 * dot(n_i, p_i - p_j) / dist_sq(p_i, p_j).
    //
    // Here `n_i` is the normal vector at `p_i`, which is estimated by the bisector of three
    // consecutive points below.
    // TODO(mkc): Handle degenerate input
    vec2r u0 = dir(sample1.p, sample0.p), u1 = v1 / len1;
    vec2r n = dot(u0, u1) >= 0 ? u0 + u1 : rhp(u0) + lhp(u1);
    sample1.k = copysign(2 * dot(n, v1) / (len(n) * len_sq1), per(u1, u0));
}

ClothoidFitter::Result ClothoidFitter::fit() const
{
    if (m_samples.size() == 0)
        return { 0, 0, 0, unx2r() };
    real s = m_samples.back().s;
    if (m_samples.size() == 1)
        return { 0, 0, s, (m_samples[0].p / s).fix(unx2r()) };

    // Fit curvature line minimizing the vertical least square error. Last sample has no curvature
    // estimate.
    int n = m_samples.size() - 1;
    real sum_s = 0, sum_k = 0, sum_ss = 0, sum_sk = 0;
    for (int i = 0; i < n; ++i)
    {
        Sample const& sample = m_samples[i];
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
    // rotation angle `alpha` yields local extrema when
    //
    //     X sin(alpha) = Y cos(alpha), X = SUM dot(u_i, v_i), Y = SUM per(u_i, v_i).
    //
    // We solve for `sin(alpha)` and `cos(alpha)` directly by noting they lie on unit circle.
    vec2r rotation = 0;
    for (Sample const& sample : m_samples)
    {
        vec2r u = PreciseFresnel::eval(k0, k1, sample.s), v = sample.p;
        rotation.x += dot(u, v);
        rotation.y += per(u, v);
    }
    rotation.normalize();

    return { k0, k1, s, rotation };
}

} // namespace kletch
