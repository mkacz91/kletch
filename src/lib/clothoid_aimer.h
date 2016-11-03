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
    std::vector<vec2r> get_samples() const;

private:
public: // TODO: tmp3
    static const int SLOPE_BUCKET_COUNT = 100;
private:
    static constexpr real SLOPE_VS_GRID_TH = rl(2e-2);
    static constexpr real MIN_TARGET_DIST = rl(1e-6);

    struct Sample
    {
        static constexpr int COMPONENT_COUNT = 2;
        typedef real scalar_t;
        typedef real float_t;

        vec2r p; // Value. Keep it at the beginning to save on arithmetic in [] operator.
        real k1; // Initial curvature
        real s;  // Arc length

        Sample(real k1, real s, vec2r p) : p(p), k1(k1), s(s) { }
        Sample(vec2r const& p) : p(p) { }

        real operator [] (int c) const { return *(reinterpret_cast<real const*>(&p) + c); }
        static real dist_sq(Sample const& a, Sample const& b) { return vec2r::dist_sq(a.p, b.p); }
    };

    struct SlopeBucket
    {
        real s;        // Arc length parameter value
        real distance; // Distance of the avaluated point to the origin
    };

    std::vector<Sample> m_samples;
    SlopeBucket m_slope_buckets[SLOPE_BUCKET_COUNT];
    real m_max_slope;
    int m_refine_steps = 4;

    Result get_initial_aim_guess(real k0, vec2r target) const;

    void init_samples(real delta_theta);

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
