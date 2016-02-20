#include "clothoid_aimer.h"

#include <queue>

#include "fresnel.h"

namespace kletch {

ClothoidAimer::ClothoidAimer(real delta_theta)
{
    init_grid(delta_theta);
}

ClothoidAimer::Result ClothoidAimer::aim(
    const vec2r& p0,
    real theta0,
    real kappa0,
    const vec2r& p1
) const
{
    vec2r p = kappa0 * (p1 - p0).rotated(-theta0);
    cout << p << " ";
    vec2i grid_p = to_grid(p);
    cout << grid_p << endl;
    Cell cell = m_grid[grid_p.y][grid_p.x];
    return { cell.a / kappa0, cell.s / kappa0, true };
}

inline real ClothoidAimer::get_max_s(real kappa0, real a, real delta_theta)
{
    real two_a = rl(2) * a;
    real theta = kappa0 * kappa0 / two_a + delta_theta;
    return a > rl(0) || theta <= rl(0)
        ? (sqrt(two_a * theta) - kappa0) / a
        : -(sqrt(-two_a * theta) + kappa0) / a;
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

    cout << m_grid_box << endl;

    // Distribute samples to cells
    std::queue<vec2i> frontier;
    for (Sample& sample : m_samples)
    {
        vec2i grid_p = to_grid(sample.p);
        int i = grid_p.y, j = grid_p.x;
        if (m_grid[i][j].empty())
        {
            m_grid[i][j] = { sample.a, sample.s };
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

vector<ClothoidAimer::Sample> ClothoidAimer::generate_samples(real kappa0, real delta_theta)
{
    // TODO: These constants should be configurable
    const int n = 100; // TODO: I don't even know how to interpret this
    const real min_a = rl(-3);
    const real max_a = rl(3);
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

} // namespace kletch
