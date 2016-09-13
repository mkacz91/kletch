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
        real k1;
        real s;
        bool success;
    };

    ClothoidAimer(real delta_theta = rl(3.14159265359));

    Result aim(real k0, vec2r target) const;

    int refine_steps() const { return m_refine_steps; }
    void set_refine_steps(int refine_steps) { m_refine_steps = refine_steps; }

    // Retrieves the sampled points used for the initial `aim` result guess. For debug and
    // visualization purposes.
    std::vector<vec2r> get_samples();

private:
public: // TODO: tmp
    static const int GRID_SIZE = 100;
    box2f m_grid_box;
private:
    struct Sample
    {
        real k1;
        real s;
        vec2r p;
    };
    Result m_grid[GRID_SIZE + 2][GRID_SIZE + 2];
    int m_refine_steps = 4;

    void init_grid(real delta_theta);
    vec2i to_grid(const vec2r& point) const;
    static int to_grid(real v0, real v1, real v);

    static real get_max_s(real k0, real k1, real delta_theta);
    static bool get_k1_range(real k0, real s, real delta_theta, real* k1_start, real* k1_end);

    static std::vector<Sample> generate_samples(real k0, real delta_theta);

    static mat2r eval_jacobian(real k0, real k1, real s);
    static void newton_refine(real k0, real* k1, real* s, vec2r target, int iter_count);
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_AIMER_H
