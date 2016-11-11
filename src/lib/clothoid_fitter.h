#ifndef KLETCH_LIB_CLOTHOID_FITTER_H
#define KLETCH_LIB_CLOTHOID_FITTER_H

#include "prefix.h"
#include <vector>

namespace kletch {

class ClothoidFitter
{
public:
    struct Result
    {
        real k0;
        real k1;
        real s;
        mat23r matrix;
    };

    ClothoidFitter();

    void clear();
    void push(vec2r const& p);
    Result fit() const;

private:
    struct Sample { real s; vec2r p; };

    std::vector<Sample> m_samples;
    real m_sum_k, m_sum_s, m_sum_kk, m_sum_ks; // Curvature line fitting accumulators
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_FITTER_H
