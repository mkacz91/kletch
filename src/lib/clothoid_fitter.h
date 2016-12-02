#ifndef KLETCH_LIB_CLOTHOID_FITTER_H
#define KLETCH_LIB_CLOTHOID_FITTER_H

#include "prefix.h"
#include <vector>

#include "curve_window.h"

namespace kletch {

class ClothoidFitter
{
public:
    struct Result
    {
        real th0;
        real k0;
        real k1;
        real s;
        real cost;
    };

    ClothoidFitter();

    Result fit(CurveWindow const& curve, int start, int end);
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_FITTER_H
