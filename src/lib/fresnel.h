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
    // Threshold determining which evaluation algorithm is used depending on the `k1` parameter
    // magnitude.
    static constexpr real K1_THRESHOLD = rl(0); // TODO

    // Evaluates the standard Fresnel integral `int(0, s, exp(0.5 * i * pi * t^2) dt)`.
    static vec2r eval_standard(real s);

    // Evaluate the general clothoid integral `int(0, s, exp{i * (k0 * t + 0.5 * k1 * t^2)} dt)`.
    static vec2r eval(real k0, real k1, real s);

    // Evaluates the general clothoid integral `int(0, s, exp{i * (k0 * t + 0.5 * k1 * t^2)} dt)`
    // assuming `abs(k1) >= K1_THRESHOLD`.
    static vec2r eval_bgk1(real k0, real k1, real s);

    // Evaluates the general clothoid integral `int(0, s, exp{i * (k0 * t + 0.5 * k1 * t^2)} dt)`
    // assuming `abs(k1) < K1_THRESHOLD`.
    static vec2r eval_smk1(real k0, real k1, real s);

private:

    template <int n>
    static void eval_momenta(real k0, real k1, real s, vec2r* f);
};

inline vec2r ReferenceFresnel::eval(real k0, real k1, real s)
{
    return abs(k1) >= K1_THRESHOLD ? eval_bgk1(k0, k1, s) : eval_smk1(k0, k1, s);
}

}

#endif // KLETCH_REFERENCE_FRESNEL_H
