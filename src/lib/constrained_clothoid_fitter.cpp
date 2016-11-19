#include "constrained_clothoid_fitter.h"
#include "math/fuzzy.h"
#include "precise_fresnel.h"

namespace kletch {

ConstrainedClothoidFitter::ConstrainedClothoidFitter(ClothoidAimer const* aimer) : m_aimer(aimer)
{
    if (aimer == nullptr)
        throw std::invalid_argument("aimer is null");
    init(0, 0, 0);
}

void ConstrainedClothoidFitter::init(vec2r p0, real th0, real k0)
{
    m_p0 = p0;
    m_th0 = th0;
    m_k0 = k0;
    m_rotation = dir(-th0);
    m_s = 0;
    m_sum_w = m_sum_wk = m_sum_wkk = 0;
}

bool ConstrainedClothoidFitter::push(vec2r p)
{
    p -= m_p0;
    p.rotate(m_rotation);

    m_last_aim = m_aimer->aim(m_k0, p);
    if (!equal(m_last_aim.eval, p, AIM_TOL))
        return false;

    real k1 = m_last_aim.k1;
    real s = m_last_aim.s;
    real w = PreciseFresnel::eval_m2(m_k0, k1, s).len_sq();
    m_s = max(m_s, s);
    m_sum_w += w;
    m_sum_wk += w * k1;
    m_sum_wkk += w * k1 * k1;

    return true;
}

ConstrainedClothoidFitter::Result ConstrainedClothoidFitter::fit() const
{
    real k1 = m_sum_wk / m_sum_w;
    real s = m_s;
    real penalty = m_sum_wkk - k1 * m_sum_wk;
    return { k1, s, penalty };
}

} // namespace kletch
