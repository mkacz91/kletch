#ifndef KLETCH_LIB_CURVE_WINDOW_H
#define KLETCH_LIB_CURVE_WINDOW_H

#include "prefix.h"
#include <vector>

namespace kletch {

struct CurveSample
{
    real k;  // Discrete curvbature estimator
    real s;  // Arc length parameterr
    real h;  // Curvature adjusted parameter
    vec2r p; // Position
};

class CurveWindow
{
public:
    int size() const { return m_samples.size(); }
    bool empty() const { return m_samples.empty(); }
    real distance_eps() const { return m_distance_eps; }
    void set_distance_eps(real value) { m_distance_eps = value; }

    CurveSample& operator [] (int i) { return m_samples[i]; }
    CurveSample const& operator [] (int i) const { return m_samples[i]; }
    CurveSample& back() { return m_samples.back(); }
    CurveSample const& back() const { return m_samples.back(); }

    CurveSample& push(vec2r const& p);
    int pop(int count);
    void clear();

private:
    static constexpr real DEFAULT_DISTANCE_EPS = rl(1e-4);

    std::vector<CurveSample> m_samples;
    real m_distance_eps = DEFAULT_DISTANCE_EPS;
};

} // namespace kletch

#endif // KLETCH_LIB_CURVE_WINDOW_H
