#ifndef KLETCH_REFERENCE_FRESNEL_H
#define KLETCH_REFERENCE_FRESNEL_H

#include "prefix.h"

namespace kletch {

// Evaluation of the Fresnel momenta with the highest precision possible, not bothering about the
// performance. This is by no means to be used in the actual algorithms, but rather as a reference
// and a placeholder in the early stages of development.
class ReferenceFresnel
{
public:
    // Evaluates the standard Fresnel integral `int(0, s, exp(0.5 * i * pi * t^2) dt)`.
    static vec2r eval_standard(real s);
};

}

#endif // KLETCH_REFERENCE_FRESNEL_H
