#include "line_fitter.h"

namespace kletch {

LineFitter::LineFitter()
{
    clear();
}

void LineFitter::clear()
{
    m_sx = m_sy = m_sxx = m_sxy = 0;
    m_sample_count = 0;
}

void LineFitter::push(real x, real y)
{
    m_sx += x;
    m_sy += y;
    m_sxx += x * x;
    m_sxy += x * y;
    ++m_sample_count;
}

LineFitter::Result LineFitter::fit(real sx, real sy, real sxx, real sxy, int n)
{
    if (n <= 0)
        throw std::exception("No samples.");
    real a = (n * sxy - sx * sy) / (n * sxx - sx * sx);
    real b = (sy - sx * a) / n;
    return { a, b };
}

} // namespace kletch
