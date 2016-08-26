#ifndef KLETCH_LIB_CLOTHOID_AIMER_H
#define KLETCH_LIB_CLOTHOID_AIMER_H

#include "prefix.h"
#include <vector>

namespace kletch {

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

    int refine_steps() const { return m_refine_steps; }
    void set_refine_steps(int refine_steps) { m_refine_steps = refine_steps; }

    std::vector<Sample> m_samples; // TODO: temporary

private:
public: // TODO: tmp
    static const int GRID_SIZE = 100;
private:

    struct Cell
    {
        real a;
        real s;

        bool empty() { return s < 0; }
    };

    Cell m_grid[GRID_SIZE][GRID_SIZE];
public:// TODO: tmp
    box2f m_grid_box;
private:

    int m_refine_steps = 4;

    void init_grid(real delta_theta);
    vec2i to_grid(const vec2r& point) const;

    static int to_grid(int n, real v0, real v1, real v);

    static real get_max_s(real kappa0, real a, real delta_theta);
    static bool get_a_range(real kappa0, real s, real delta_theta, real* a0, real* a1);

    static std::vector<Sample> generate_samples(real kappa0, real delta_theta);

    static mat2r eval_jacobian(real k0, real k1, real s);
    static void newton_refine1(vec2r p, real* a, real* s, int iter_count = 4);
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_AIMER_H
