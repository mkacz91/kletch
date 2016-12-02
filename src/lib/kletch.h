#ifndef KLETCH_LIB_KLETCH_H
#define KLETCH_LIB_KLETCH_H

#include "prefix.h"
#include <queue>
#include <vector>

#include "clothoid_aimer.h"
#include "clothoid_fitter.h"
#include "constrained_clothoid_fitter.h"
#include "curve_window.h"

namespace kletch {

class Kletch
{
public:
    struct Segment
    {
        vec2r p0;         // Starting point
        real th0;         // Initial tangent angle
        real k0;          // Initial curvature
        real k1;          // Curvature slope
        real s;           // Arc length
        vec2r p1;         // End point
        int sample_count; // Number of samples covered
    };

    Kletch();

    real get_horizon() const { return m_horizon; }
    void set_horizon(real value) { m_horizon = value; }
    Segment const* last_segment() const { return &m_last_segment; }

    void clear();
    void push(vec2r const& p) { m_curve.push(p); }
    Segment const* try_advance();

private:
    struct SegmentStub
    {
        real quality;
        real th0;
        real k0;
        real k1;
        real s;
        vec2r p1;
    };

    struct Extensor
    {
        bool active;
        int stub;
        ConstrainedClothoidFitter fitter;
        real base_cost, cost_multiplier;

        Extensor(ClothoidAimer const* aimer) : fitter(aimer) { }
    };

    static constexpr real DEFAULT_HORIZON = rl(3);

    CurveWindow m_curve;
    ClothoidAimer m_aimer;
    ClothoidFitter m_free_starter;
    ConstrainedClothoidFitter m_constrained_starter;
    std::vector<Extensor> m_extensors;
    std::vector<SegmentStub> m_stubs;
    bool m_free_start;
    int m_progress;
    real m_horizon = DEFAULT_HORIZON;
    Segment m_last_segment;

    void init_advancement();
    bool step_sample();
    void advance();
};

} // namespace kletch

#endif // KLETCH_LIB_KLETCH_H
