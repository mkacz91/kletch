#include "clothoid_aimer.h"

#include <queue>
#include <stack>

#include "precise_fresnel.h"

namespace kletch {

ClothoidAimer::ClothoidAimer(real delta_theta)
{
    init_grid(delta_theta);
}

ClothoidAimer::Result ClothoidAimer::aim(real k0, vec2r target) const
{
    target *= k0;
    vec2i grid_target = to_grid(target);
    Cell cell = m_grid[grid_target.y][grid_target.x];
    real k1 = cell.k1, s = cell.s;
    newton_refine1(target, &k1, &s, refine_steps());
    return { k1, s, true };
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

void ClothoidAimer::init_grid(real delta_theta)
{
    // Clear cells
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
            m_grid[i][j] = { -1, -1 };
    }

    // Generate samples and compute bounding box
    m_samples = generate_samples(1, delta_theta);
    m_grid_box = box2f::EMPTY;
    for (Sample& sample : m_samples)
        m_grid_box.expand(sample.p);

    // Distribute samples to cells
    std::queue<vec2i> frontier;
    for (Sample& sample : m_samples)
    {
        vec2i grid_p = to_grid(sample.p);
        int i = grid_p.y, j = grid_p.x;
        if (m_grid[i][j].empty())
        {
            m_grid[i][j] = { sample.k1, sample.s };
            frontier.push(grid_p);
            assert(!m_grid[i][j].empty());
        }
    }

    // Fill empty cells
    while (!frontier.empty())
    {
        int i = frontier.front().y, j = frontier.front().x;
        frontier.pop();
        assert(!m_grid[i][j].empty());
        if (0 < i && m_grid[i - 1][j].empty())
        {
            m_grid[i - 1][j] = m_grid[i][j];
            frontier.emplace(j, i - 1);
        }
        if (i < GRID_SIZE - 1 && m_grid[i + 1][j].empty())
        {
            m_grid[i + 1][j] = m_grid[i][j];
            frontier.emplace(j, i + 1);
        }
        if (0 < j && m_grid[i][j - 1].empty())
        {
            m_grid[i][j - 1] = m_grid[i][j];
            frontier.emplace(j - 1, i);
        }
        if (j < GRID_SIZE - 1 && m_grid[i][j - 1].empty())
        {
            m_grid[i][j + 1] = m_grid[i][j];
            frontier.emplace(j + 1, i);
        }
    }
}

std::vector<ClothoidAimer::Sample> ClothoidAimer::generate_samples(real k0, real delta_theta)
{
    const real ref_sample_dist = rl(0.1);
    const real ref_sample_dist_sq = sq(ref_sample_dist);
    const int initial_partition = 7;

    std::vector<Sample> samples;
    samples.push_back({ 0, 0, 0 });

    real s = rl(0.1);
    real k1_start, k1_end;
    std::stack<tuple<int, int>> ranges;
    while (get_k1_range(k0, s, delta_theta, &k1_start, &k1_end))
    {
        for (int i = 0; i <= initial_partition; ++i)
        {
            real k1 = lerp(k1_start, k1_end, rl(i) / rl(initial_partition));
            samples.push_back({ k1, s, PreciseFresnel::eval(k0, k1, s) });
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
                samples.push_back({ k1, s, PreciseFresnel::eval(k0, k1, s) });
                ranges.emplace(j0, j);
                ranges.emplace(j, j1);
            }
        }
        s += ref_sample_dist;
    }

    return samples;
}

vec2i ClothoidAimer::to_grid(const vec2r& point) const
{
    return vec2i(
        to_grid(GRID_SIZE, m_grid_box.y0, m_grid_box.y1, point.y),
        to_grid(GRID_SIZE, m_grid_box.x0, m_grid_box.x1, point.x)
    );
}

inline int ClothoidAimer::to_grid(int n, real v0, real v1, real v)
{
    if (v <= v0)
        return 0;
    if (v1 <= v)
        return n - 1;
    return min(n - 1, n * (v - v0) / (v1 - v0));
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

void ClothoidAimer::newton_refine1(vec2r p, real* k1, real* s, int iter_count)
{
    vec2r u = { *k1, *s };
    while (iter_count --> 0)
    {
        vec2r q = p - PreciseFresnel::eval(1, u.x, u.y);
        mat2r J = eval_jacobian(1, u.x, u.y);
        J.invert();
        u += J.transform(q);
    }
    *k1 = u.x, *s = u.y;
}

} // namespace kletch
