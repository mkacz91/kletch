#ifndef KLETCH_LIB_CLOTHOID_AIMER_H
#define KLETCH_LIB_CLOTHOID_AIMER_H

#include "prefix.h"
#include <vector>

namespace kletch {

using std::vector;

class ClothoidAimer
{
public:
    struct Result
    {
        real a;
        real s;
        bool success;
    };

    struct Sample
    {
        real a;
        real s;
        vec2r p;

        Sample(real a, real s, vec2r const& p) : a(a), s(s), p(p) { }
    };

    ClothoidAimer(real delta_theta);

    Result aim(const vec2r& p0, real theta0, real kappa0, const vec2r& p1) const;

    const vector<Sample>& samples1() const { return m_samples1; }

private:
    const vector<Sample> m_samples1;

    static real get_max_s(real kappa0, real a, real delta_theta);

    static vector<Sample> generate_samples(real kappa0, real delta_theta);
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_AIMER_H
