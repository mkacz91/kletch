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
    static const int SLOPE_BUCKET_COUNT = 100;
    box2f m_grid_box;
private:
    static constexpr real SLOPE_VS_GRID_TH = rl(2e-2);
    static constexpr real MIN_TARGET_DIST = rl(1e-6);

    struct Sample
    {
        real k1;
        real s;
        vec2r p;
    };

    struct SlopeBucket
    {
        real s;        // Arc length parameter value
        real distance; // Distance of the avaluated point to the origin
    };

    Result m_grid[GRID_SIZE + 2][GRID_SIZE + 2];
    SlopeBucket m_slope_buckets[SLOPE_BUCKET_COUNT];
    real m_max_slope;
    int m_refine_steps = 4;

    Result get_initial_aim_guess(real k0, vec2r target) const;

    void init_grid(real delta_theta);
    vec2i to_grid(real x, real y) const;
    vec2i to_grid(vec2r const& point) const { return to_grid(point.x, point.y); }
    static int to_grid(real v0, real v1, real v);

    void init_slope_buckets();
    int map_slope_to_bucket_index(real slope) const;

    static real get_max_s(real k0, real k1, real delta_theta);
    static bool get_k1_range(real k0, real s, real delta_theta, real* k1_start, real* k1_end);

    static std::vector<Sample> generate_samples(real k0, real delta_theta);

    static mat2r eval_jacobian(real k0, real k1, real s);
    static void newton_refine(real k0, real* k1, real* s, vec2r target, int iter_count);
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_AIMER_H
