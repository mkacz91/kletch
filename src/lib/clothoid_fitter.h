#ifndef KLETCH_LIB_CLOTHOID_FITTER_H
#define KLETCH_LIB_CLOTHOID_FITTER_H

#include "prefix.h"
#include <vector>

namespace kletch {

class ClothoidFitter
{
public:
    struct Result
    {
        real k0;
        real k1;
        real s;
        mat2r matrix;
    };

    ClothoidFitter();

    void clear();
    void push(vec2r const& p);
    Result fit() const;

private:
    struct Sample { real k, s; vec2r p; };
    static Sample ORIGIN_SAMPLE; // This is a de facto constant but can't be marked as such

    std::vector<Sample> m_samples;
};

} // namespace kletch

#endif // KLETCH_LIB_CLOTHOID_FITTER_H
