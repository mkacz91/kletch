#include "clothoid_fitter.h"

namespace kletch {

void ClothoidFitter::push(vec2r const& p)
{
    int i = m_samples.size();
    m_samples.push_back({ rl(0), p });
    Sample& s2 = m_samples[i];

    if (i >= 2)
    {
        Sample& s0 = m_samples[i - 2], s1 = m_samples[i - 1];
        vec2r v0 = span(s1.p, s0.p), v1 = span(s1.p, s2.p);
        vec2r u = bisector(v1, v0);
        real len0 = s1.s - s0.s, len1 = len(v1), lenu = len(u);
        s2.s = s1.s + len1;
        real k = rl(2) * per(v1, u) / (len1 * lenu * sqrt(len0 * len1));
        m_line_fitter.push(s1.s, k);
    }
    else if (i == 1)
    {
        Sample& s1 = m_samples[i - 1];
        s2.s = s1.s + dist(s1.p, s2.p);
    }
}

ClothoidFitter::Result ClothoidFitter::fit() const
{
    auto line = m_line_fitter.fit();
    real k0 = line.b;
    real k1 = line.a;
    real s = m_samples.back().s;

    return { k0, k1, s, eye23f() };
}

} // namespace kletch
