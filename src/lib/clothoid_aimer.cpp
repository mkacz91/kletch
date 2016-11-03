#include "clothoid_aimer.h"

#include <algorithm>
#include <queue>
#include <stack>

#include "kd_tree.h"
#include "precise_fresnel.h"

namespace kletch {

ClothoidAimer::ClothoidAimer(real delta_theta)
{
    init_samples(delta_theta);
    init_slope_buckets();
}

ClothoidAimer::Result ClothoidAimer::aim(real k0, vec2r target) const
{
    real target_dist = len(target);
    if (target_dist < MIN_TARGET_DIST) // TODO
    // There is no work to do if the target is already practically at origin
        return { 0, 0, true };

    Result result = get_initial_aim_guess(k0, target);
    if (result.success)
        newton_refine(k0, &result.k1, &result.s, target, refine_steps());
    return result;
}

inline ClothoidAimer::Result ClothoidAimer::get_initial_aim_guess(real k0, vec2r target) const
{
    if (abs(k0) < SLOPE_VS_GRID_TH)
    // Almost nil initial curvature. Use the slope buckets.
    {
        float slope = abs(target.y / target.x);
        if (target.x < 0 || slope > m_max_slope)
            return { 0, 0, false };
        int bucket_index = map_slope_to_bucket_index(slope);
        if (bucket_index == 0)
        // The 0 bucket is bad for Newton refinement and scale correction. Here's a better guess.
            return { 0, target.x, true };
        SlopeBucket bucket = m_slope_buckets[bucket_index];
        // Scale correction
        real scale = len(target) / bucket.distance;
        real s = bucket.s * scale;
        real k1 = copysign(pi<real>() / (scale * scale), target.y);
        return { k1, s, true };
    }
    else
    // Non-negligable initial curvature. Use grid.
    {
        real abs_k0 = abs(k0);
        Sample const& nearest = KdTree::get_nearest<Sample>(m_samples, abs_k0 * target);
        Result result = { nearest.k1, nearest.s, true };
        if (result.s == 0) // TODO: This is poor man's check
            return { 0, target.x, true };
        // Scale correction
        result.k1 *= abs_k0 * k0;
        result.s /= abs_k0;
        return result;
    }
}

std::vector<vec2r> ClothoidAimer::get_samples() const
{
    std::vector<vec2r> samples;
    samples.reserve(m_samples.size());
    for (auto& sample : m_samples)
        samples.push_back(sample.p);
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

std::vector<ClothoidAimer::Sample> ClothoidAimer::generate_samples(real k0, real delta_theta)
{
    const real ref_sample_dist = rl(0.1);
    const real ref_sample_dist_sq = sq(ref_sample_dist);
    const int initial_partition = 7;

    std::vector<Sample> samples;
    samples.emplace_back(0, 0, 0);

    real s = rl(0.1);
    real k1_start, k1_end;
    std::stack<tuple<int, int>> ranges;
    while (get_k1_range(k0, s, delta_theta, &k1_start, &k1_end))
    {
        for (int i = 0; i <= initial_partition; ++i)
        {
            real k1 = lerp(k1_start, k1_end, rl(i) / rl(initial_partition));
            samples.emplace_back(k1, s, PreciseFresnel::eval(k0, k1, s));
        }
        for (int i = 1; i <= initial_partition; ++i)
            ranges.emplace(samples.size() - i, samples.size() - i - 1);
        while (!ranges.empty())
        {
            int j0 = get<0>(ranges.top()), j1 = get<1>(ranges.top());
            ranges.pop();
            const Sample& s0 = samples[j0];
            const Sample& s1 = samples[j1];
            if (2 * ref_sample_dist_sq < dist_sq(s0.p, s1.p))
            {
                int j = samples.size();
                real k1 = rl(0.5) * (s0.k1 + s1.k1);
                real s = s0.s;
                assert(s == s1.s);
                samples.emplace_back(k1, s, PreciseFresnel::eval(k0, k1, s));
                ranges.emplace(j0, j);
                ranges.emplace(j, j1);
            }
        }
        s += ref_sample_dist;
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

void ClothoidAimer::newton_refine(real k0, real* k1, real* s, vec2r target, int iter_count)
{
    vec2r u = { *k1, *s };
    while (iter_count --> 0)
    {
        mat2r inverse_jacobian = eval_jacobian(k0, u.x, u.y).invert();
        u += inverse_jacobian.transform(target - PreciseFresnel::eval(k0, u.x, u.y));
    }
    *k1 = u.x, *s = u.y;
}

} // namespace kletch
