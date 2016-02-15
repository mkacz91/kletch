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

    ClothoidAimer(real delta_theta = rl(3.14159265359));

    Result aim(const vec2r& p0, real theta0, real kappa0, const vec2r& p1) const;

private:
    static const int GRID_SIZE = 200;

    struct Cell
    {
        real a;
        real s;

        bool empty() { return a < 0; }
    };

    Cell m_grid[GRID_SIZE][GRID_SIZE];
    box2r m_grid_box;

    void init_grid(real delta_theta);
    vec2i to_grid(const vec2r& point) const;

    static int to_grid(int n, real v0, real v1, real v);

    static real get_max_s(real kappa0, real a, real delta_theta);

    static vector<Sample> generate_samples(real kappa0, real delta_theta);
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_AIMER_H
