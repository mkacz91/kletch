#include "clothoid_aimer.h"

#include <algorithm>
#include <queue>
#include <stack>

#include "kd_tree.h"
#include "precise_fresnel.h"

namespace kletch {

template <> struct KdTreeTraits<ClothoidAimer::Result>
{
    typedef vec2r::scalar_t scalar_t;
    typedef vec2r vector_t;
    static constexpr int COMPONENT_COUNT = vec2r::COMPONENT_COUNT;
    static scalar_t component(ClothoidAimer::Result const& r, int i) { return r.eval[i]; }
    static scalar_t component(vector_t const& u, int i) { return u[i]; }
    static scalar_t dist_sq(vector_t const& u, ClothoidAimer::Result const& r) {
        return vec2r::dist_sq(u, r.eval); }
};

ClothoidAimer::ClothoidAimer(real delta_theta)
{
    init_samples(delta_theta);
    init_slope_buckets();
}

ClothoidAimer::Result ClothoidAimer::aim(real k0, vec2r target, int refine_steps) const
{
    real const target_dist = len(target);
    real const abs_k0 = abs(k0);

    // Determine seed

    real k1, s;
    vec2r eval;
    if (abs_k0 * target_dist < SLOPE_VS_SAMPLE_TH)
    // Initial curvature relatively small. Use slope buckets
    {
        float slope = abs(target.y / target.x);
        int bucket_index = map_slope_to_bucket_index(slope);
        if (target.x < 0 || slope > m_max_slope)
        {
            k1 = 0;
            s = 0;
        }
        else if (bucket_index == 0)
        {
            k1 = 0;
            s = target.x;
        }
        else
        {
            SlopeBucket bucket = m_slope_buckets[bucket_index];
            real scale = target_dist / bucket.distance;
            s = bucket.s * scale;
            k1 = copysign(pi<real>() / (scale * scale), target.y);
        }
        eval = PreciseFresnel::eval(k0, k1, s);
    }
    else
    // Use samples
    {
        Result const& nearest = KdTree::get_nearest(m_samples, abs_k0 * target);
        real inv_abs_k0 = rl(1) / abs_k0;
        k1 = nearest.k1 * abs_k0 * k0;
        s = nearest.s * inv_abs_k0;
        eval = nearest.eval * inv_abs_k0;
    }

    // Refine using Newton method

    while (refine_steps --> 0)
    {
        vec2r du = eval_jacobian(k0, k1, s).invert().transform(target - eval);
        if (du.any_broken())
            break;
        k1 += du[0]; s += du[1];
        eval = PreciseFresnel::eval(k0, k1, s);
    }

    return { eval, k1, s };
}

std::vector<vec2r> ClothoidAimer::get_samples() const
{
    std::vector<vec2r> samples;
    samples.reserve(m_samples.size());
    for (auto& sample : m_samples)
        samples.push_back(sample.eval);
    return samples;
}

inline real ClothoidAimer::get_max_s(real kappa0, real a, real delta_theta)
{
    real two_a = rl(2) * a;
    real theta = kappa0 * kappa0 / two_a + delta_theta;
    return a > rl(0) || theta <= rl(0)
        ? (sqrt(two_a * theta) - kappa0) / a
        : -(sqrt(-two_a * theta) + kappa0) / a;
}

bool ClothoidAimer::get_k1_range(real k0, real s, real delta_theta, real* k1_start, real* k1_end)
{
    assert(k0 >= 0);
    assert(s > 0);
    assert(delta_theta > 0);

    real c0 = k0 * s;
    real delta = c0 * (2 * delta_theta - c0) + sq(delta_theta);
    if (delta <= 0)
        return false;

    real c1 = -c0 - delta_theta;
    real sqrt_delta = sqrt(delta);
    real s_sq = sq(s);

    real b0 = (c1 - sqrt_delta) / s_sq;
    real b1 = (c1 + sqrt_delta) / s_sq;
    if (b0 > b1)
        swap(b0, b1);

    real b2 = 2 * (delta_theta - c0) / s_sq;

    *k1_start = b0;
    *k1_end = (b2 * s < -k0) ? b1 : b2;
    return true;
}

void ClothoidAimer::init_samples(real delta_theta)
{
    m_samples = generate_samples(1, delta_theta);
    KdTree::build(m_samples);
}

std::vector<ClothoidAimer::Result> ClothoidAimer::generate_samples(real k0, real delta_theta)
{
    constexpr real ds0 = rl(0.1);
    constexpr real ds_factor = rl(1.1);
    constexpr int initial_partition = 7;

    std::vector<Result> samples;
    samples.push_back({ 0, 0, 0 });

    real s = SLOPE_VS_SAMPLE_TH;
    real ds = ds0;
    real k1_start, k1_end;
    std::stack<tuple<int, int>> ranges;
    while (get_k1_range(k0, s, delta_theta, &k1_start, &k1_end))
    {
        for (int i = 0; i <= initial_partition; ++i)
        {
            real k1 = lerp(k1_start, k1_end, rl(i) / rl(initial_partition));
            samples.push_back({ PreciseFresnel::eval(k0, k1, s), k1, s });
        }
        for (int i = 1; i <= initial_partition; ++i)
            ranges.emplace(samples.size() - i, samples.size() - i - 1);
        const real two_ds_sq = 2 * sq(ds);
        while (!ranges.empty())
        {
            int j0 = get<0>(ranges.top()), j1 = get<1>(ranges.top());
            ranges.pop();
            Result const& s0 = samples[j0];
            Result const& s1 = samples[j1];
            if (two_ds_sq < dist_sq(s0.eval, s1.eval))
            {
                int j = samples.size();
                real k1 = rl(0.5) * (s0.k1 + s1.k1);
                real s = s0.s;
                assert(s == s1.s);
                samples.push_back({ PreciseFresnel::eval(k0, k1, s), k1, s });
                ranges.emplace(j0, j);
                ranges.emplace(j, j1);
            }
        }
        ds *= ds_factor;
        s += ds;
    }

    return samples;
}

void ClothoidAimer::init_slope_buckets()
{
    std::vector<bool> visited(SLOPE_BUCKET_COUNT, false); // Indicate whether bucket is initialized
    real const k1 = pi<real>(); // This is the standard slope
    real const max_s = sqrt(rl(2)); // Evaluate up to the point where the tangent angle is pi
    vec2r terminal_f = PreciseFresnel::eval(max_s);
    m_max_slope = terminal_f.y / terminal_f.x;

    // Sample the standard clothoid
    m_slope_buckets[0] = { 0, 0 };
    for (int i = 1; i <= 2 * SLOPE_BUCKET_COUNT; ++i)
    {
        real s = i * max_s / (2 * SLOPE_BUCKET_COUNT);
        vec2r f = PreciseFresnel::eval(s);
        int j = map_slope_to_bucket_index(f.y / f.x);
        if (!visited[j])
        {
            m_slope_buckets[j] = { s, len(f) };
            visited[j] = true;
        }
    }

    // Fill holes
    for (int j = SLOPE_BUCKET_COUNT - 1; j > 0; --j)
    {
        if (!visited[j])
            m_slope_buckets[j] = m_slope_buckets[j + 1];
    }
}

inline int ClothoidAimer::map_slope_to_bucket_index(real slope) const
{
    return min<int>(SLOPE_BUCKET_COUNT - 1, slope * SLOPE_BUCKET_COUNT / m_max_slope);
}

mat2r ClothoidAimer::eval_jacobian(real k0, real k1, real s)
{
    // The d/k1 gradient of `F(k0, k1; s)` is computed as
    //
    //     DER k1: F(k0, k1; s) = 0.5 i F_2(k0, k1; s),
    //
    // whereas the d/ds is yielded by
    //
    //     DER s: F(k0, k1; s) = exp(i (k0 s + 0.5 k1 s^2)).

    vec2r fk1 = rl(0.5) * lhp(PreciseFresnel::eval_m2(k0, k1, s));
    vec2r fs = dir(s * (k0 + rl(0.5) * k1 * s));
    return mat2r(
        fk1.x, fs.x,
        fk1.y, fs.y
    );
}

} // namespace kletch
