#include "clothoid_fitter.h"

namespace kletch {

ClothoidFitter::ClothoidFitter()
{
    clear();
}

void ClothoidFitter::clear()
{
    m_samples.clear();
    m_sum_k = m_sum_s = m_sum_kk = m_sum_ks = 0;
}

void ClothoidFitter::push(vec2r const& p)
{
    // Add sample
    int i = m_samples.size();
    m_samples.push_back({ rl(0), p });
    Sample& sample2 = m_samples[i];
    vec2r p2 = p;

    if (i == 0)
        return;
    Sample& sample1 = m_samples[i - 1];
    real s1 = sample1.s; vec2r p1 = sample1.p;

    // Compute arc length parameter of the new sample
    real ds = dist(p1, p2);
    real s2 = sample2.s = s1 + ds;

    if (i == 1)
        return;
    Sample& sample0 = m_samples[i - 2];
    real s0 = sample0.s; vec2r p0 = sample0.p;

    // Estimate curvature at previous sample
    vec2r v0 = span(p1, p0), v1 = span(p1, p2);
    vec2r u = bisector(v1, v0);
    real len0 = s1 - s0, len1 = ds, lenu = len(u);
    real k1 = rl(2) * per(v1, u) / (len1 * lenu * sqrt(len0 * len1));

    // Update curvature line fitting accumulators
    m_sum_k += k1;
    m_sum_s += s1;
    m_sum_kk += k1 * k1;
    m_sum_ks += k1 * s1;
}

ClothoidFitter::Result ClothoidFitter::fit() const
{
    real s = m_samples.back().s;
    
    // Fit curvature line
    real inner_count = rl(m_samples.size() - 2);
    real k1 = (inner_count * m_sum_ks - m_sum_k * m_sum_s) / (inner_count * m_sum_kk - sq(m_sum_k));
    real k0 = (m_sum_s - m_sum_k * k1) / inner_count;

    return { k0, k1, s, eye23f() };
}

} // namespace kletch
