#ifndef KLETCH_LIB_LINE_FITTER_H
#define KLETCH_LIB_LINE_FITTER_H

#include "prefix.h"

namespace kletch {

class LineFitter
{
public:
    struct Result { real a, b; };

    LineFitter();

    void clear();
    void push(real x, real y);
    void push(vec2r const& p) { return push(p.x, p.y); }
    Result fit() const { return fit(m_sx, m_sy, m_sxx, m_sxy, m_sample_count); }

private:
    int m_sample_count;
    real m_sx, m_sy, m_sxx, m_sxy;

    static Result fit(real sx, real sy, real sxx, real sxy, int n);
};

} // namespace kletch

#endif
