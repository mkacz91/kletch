#include "clothoid_aimer.h"
#include "fresnel.h"

namespace kletch {

inline real ClothoidAimer::get_max_s(real kappa0, real a, real delta_theta)
{
    real two_a = real(2.0) * a;
    real theta = kappa0 * kappa0 / two_a + delta_theta;
    return a > rl(0.0) || theta <= rl(0.0)
        ? (sqrt(two_a * theta) - kappa0) / a
        : -(sqrt(-two_a * theta) + kappa0) / a;
}

vector<ClothoidAimer::Sample> ClothoidAimer::generate_samples(real kappa0, real delta_theta)
{
    // TODO: These constants should be configurable
    const int n = 100; // TODO: I don't even know how to interpret this
    const real min_a = rl(-3.0);
    const real max_a = rl(3.0);
    vector<Sample> result;
    result.reserve(n * n);
    for (int i = 0; i < n; ++i)
    {
        real a = lerp(min_a, max_a, rl(i) / (n - 1));
        real max_s = get_max_s(kappa0, a, delta_theta);
        for (int j = 0; j < n; ++j)
        {
            real s = lerp(rl(0), max_s, rl(j) / (n - 1));
            result.emplace_back(a, s, Fresnel::eval(rl(0), kappa0, a, s));
        }
    }
    return result;
}

} // namespace kletch
