#include "clothoid_fitter.h"

namespace kletch {

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
    vec2r p2 = p;

    if (i == 0)
        return;
    Sample& sample1 = m_samples[i - 1];

    // Compute arc length parameter of new sample.
    vec2r v1 = span(sample1.p, sample2.p);
    real len1 = len(v1);
    sample2.s = sample1.s + len1;

    if (i == 1)
        return;
    Sample& sample0 = m_samples[i - 2];

    // Estimate curvature at previous sample
    vec2r v0 = span(sample1.p, sample0.p);
    vec2r u = bisector(v1, v0);
    real len0 = sample1.s - sample0.s, lenu = len(u);
    sample1.k = rl(2) * per(v1, u) / (len1 * lenu * sqrt(len0 * len1));
}

ClothoidFitter::Result ClothoidFitter::fit() const
{
    real s = m_samples.back().s;

    // Fit curvature line. Only the inner samples have curvature estimate.
    int inner_count = m_samples.size() - 2;
    real sum_s = 0, sum_k = 0, sum_ss = 0, sum_sk = 0;
    for (int i = 1; i <= inner_count; ++i)
    {
        Sample const& sample = m_samples[i];
        sum_s += sample.s;
        sum_k += sample.k;
        sum_ss += sample.s * sample.s;
        sum_sk += sample.s * sample.k;
    }
    real k1 = (inner_count * sum_sk - sum_s * sum_k) / (inner_count * sum_ss - sum_s * sum_s);
    real k0 = (sum_k - sum_s * k1) / inner_count;

    return { k0, k1, s, eye23f() };
}

} // namespace kletch
