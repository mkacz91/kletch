#ifndef KLETCH_LIB_HEALD_H
#define KLETCH_LIB_HEALD_H

// Standard Fresnel integral evaluation based on
//
//     Mark A. Heald, Rational Approximations for the Fresnel Integrals,
//     Mathematics of Computation 44 (1985), 459-461.
//     (With corrigenda)
struct Heald
{
    // Evaluates the standard Fresnel integral `INT t = 0..s: exp(0.5 i pi t^2)` with the nominator
    // of the `R` function of degree 5.
    vec2r eval5(real S);
};

// Proxy for `Heald::eval5` that enables it for `FresnelCore`.
struct Heald5
{
    static vec2r eval(real S) { return Heald::eval5(s); }
};

#endif // KLETCH_LIB_HEALD_H
