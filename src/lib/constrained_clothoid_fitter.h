#ifndef KLETCH_LIB_CONSTRAINED_CLOTHOID_FITTER_H
#define KLETCH_LIB_CONSTRAINED_CLOTHOID_FITTER_H

#include "prefix.h"
#include <vector>

#include "clothoid_aimer.h"

namespace kletch {

class ConstrainedClothoidFitter
{
public:
    struct Result
    {
        real k1;
        real s;
        real cost;
    };

    ConstrainedClothoidFitter(ClothoidAimer const* aimer);

    ClothoidAimer const* aimer() const { return m_aimer; }
    vec2r const& p0() const { return m_p0; }
    real th0() const { return m_th0; }
    real k0() const { return m_k0; }
    int point_count() const { return m_point_count; }
    ClothoidAimer::Result const& last_aim() const { return m_last_aim; }

    void init(vec2r p0, real th0, real k0);
    bool push(vec2r p);
    Result fit() const;

private:
    static constexpr real AIM_TOL = rl(1e-3);

    ClothoidAimer const* const m_aimer;
    ClothoidAimer::Result m_last_aim;
    vec2r m_p0;
    real m_th0;
    real m_k0;
    vec2r m_rotation;
    real m_s;
    real m_sum_w, m_sum_wk, m_sum_wkk;
    int m_point_count;
};

} // namespace kletch

#endif // KLETCH_LIB_CONSTRAINED_CLOTHOID_FITTER_H
