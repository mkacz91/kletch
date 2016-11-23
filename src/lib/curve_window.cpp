#include "curve_window.h"
#include <math/fuzzy.h>

namespace kletch {

CurveSample& CurveWindow::push(vec2r const& p)
{
    auto push = [this](CurveSample const& sample) -> CurveSample&
    {
        m_samples.push_back(sample);
        return m_samples.back();
    };

    // Adding a new sample involves computation on the two last samples as well. For that purpose
    // local indexing is introduced on samples: #2 is the current sample, #1 is the previous
    // sample, and #0 is the one before. Two initial samples must be handeled separately. We also
    // reject duplicate samples.
    int i = m_samples.size();
    if (i == 0)
        return push({ 0, 0, 0, p });
    CurveSample& sample1 = m_samples[i - 1];
    if (equal(p, sample1.p, m_distance_eps))
        return sample1;
    if (i == 1)
        return push({ 0, dist(p, sample1.p), 0, p });
    CurveSample& sample0 = m_samples[i - 2];

    // Compute arc length parameter increment ds for the new sample
    vec2r dp2 = span(sample1.p, p);
    real ds2_sq = len_sq(dp2);
    real ds2 = sqrt(ds2_sq);

    // Estimate curvature at the previoud sample. The curvature at point `p_i` in the direction of
    // the subsequent point `p_j` is given by
    //
    //     k_i = 2 * dot(n_i, p_i - p_j) / dist_sq(p_i, p_j).
    //
    // Here `n_i` is the normal vector at `p_i`, which the code below estimates with the bisector
    // of three consecutive points.
    vec2r u1 = span(sample0.p, sample1.p) / (sample1.s - sample0.s), u2 = dp2 / ds2;
    vec2r n1 = dot(u1, u2) < 0 ? u2 - u1 : lhp(u1) + lhp(u2);
    real k1 = copysign(2 * dot(n1, dp2) / (len(n1) * ds2_sq), per(u1, u2));
    sample1.k = copysign(2 * dot(n1, dp2) / (len(n1) * ds2_sq), per(u1, u2));

    // The feature adjusted parameter increment is determined by the curvature at previous sample.
    // The curvature at current sample is not known yet.
    real dh2 = abs(sample1.k) * ds2;
    return push({ 0, sample1.s + ds2, sample1.h + dh2, p });
}

int CurveWindow::pop(int count)
{
    // Pop the given amount of initial points by moving the subsequent ones in their place and
    // cutting of the extent. Move the parameter origin to the first remaining sample.
    count = min(count, m_samples.size());
    int left = m_samples.size() - count;
    if (left != 0)
    {
        auto const& sample0 = m_samples[count];
        real s0 = sample0.s, h0 = sample0.h;
        for (int i = 0; i < left; ++i)
        {
            auto& sample = m_samples[i];
            sample = m_samples[i + count];
            sample.s -= s0;
            sample.h -= h0;
        }
    }
    m_samples.resize(left);
    return count;
}

void CurveWindow::clear()
{
    m_samples.clear();
}

} // namespace kletch
