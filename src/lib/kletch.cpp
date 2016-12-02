#include "kletch.h"
#include "precise_fresnel.h"
#include "printing.h" // tmp

namespace kletch {

Kletch::Kletch() : m_constrained_starter(&m_aimer)
{
    clear();
}

void Kletch::clear()
{
    m_curve.clear();
    m_free_start = true;
    m_progress = 0;
    m_last_segment = { 0, 0, 0, 0, 0, 0, 0 };
}

Kletch::Segment const* Kletch::try_advance()
{
    // TODO(mkc): THis is mega dirty. Effect of debugging
    if (m_curve.size() <= 2)
        return nullptr;
    if (m_progress == 0)
        init_advancement();

    // Step samples until horizon is reached
    while (m_progress < m_curve.size() && m_curve[m_progress].h < m_horizon)
    {
        // Return early if the curve shape makes it impossible to push a smooth spline through.
        if (!step_sample())
        {
            m_free_start = true; // TODO(mkc): This is wrong
            if (m_progress == 0)
                return nullptr;
            advance();
            m_progress = 0;
            return &m_last_segment;
        }
    }

    // Fail if the samples were exhausted before reaching the horizon
    if (m_progress == m_curve.size())
        return nullptr;

    // Advance and keep the adv
    advance();
    m_free_start = false;
    m_progress = 0;
    return &m_last_segment;
}

void Kletch::init_advancement()
{
    // TODO(mkc): THis is mega dirty. Effect of debuggin
    Segment& last = m_last_segment;
    if (m_free_start)
    {
        if (last.sample_count != 0) // The very first start does not enter this
        {
            m_curve[last.sample_count - 1].p = last.p1;
            m_curve.pop(last.sample_count - 1);
        }
        else
        {
            last.p1 = m_curve[0].p;
        }
    }
    else
    {
        m_curve.pop(last.sample_count);
        m_constrained_starter.init(
            last.p1,
            last.th0 + last.s * (last.k0 + rl(0.5) * last.s * last.k1),
            last.k0 + last.s * last.k1);
    }
}

bool Kletch::step_sample()
{
    // We determine the best spline ending at the given sample by extending the spline of every
    // previous point and choosing the one with lowest cost.

    // Enlarge the per-sample data collections if needed
    if (m_stubs.size() <= m_progress)
    {
        m_stubs.emplace_back();
        m_extensors.emplace_back(&m_aimer);
    }

    auto const& last = m_last_segment;
    auto const& sample = m_curve[m_progress];
    auto& stub = m_stubs[m_progress];
    auto& extensor = m_extensors[m_progress];
    real best_cost = inf<real>();

    if (m_free_start)
    {
        auto fit = m_free_starter.fit(m_curve, 0, m_progress + 1);
        best_cost = fit.cost;
        stub.th0 = fit.th0;
        stub.k0 = fit.k0;
        stub.k1 = fit.k1;
        stub.s = fit.s;
    }
    else if (m_constrained_starter.push(sample.p))
    {
        auto const& fitter = m_constrained_starter;
        auto fit = fitter.fit();
        best_cost = fit.cost;
        stub.th0 = fitter.th0();
        stub.k0 = fitter.k0();
        stub.k1 = fit.k1;
        stub.s = fit.s;
    }

    int best_stub;
    if (isinf(best_cost))
    {
        stub.quality = -inf<real>();
        best_stub = -1;
    }
    else
    {
        stub.quality = 0;
        // TODO(mkc): Provide suitable overload in PreciseFresnel
        stub.p1 = last.p1 + PreciseFresnel::eval(stub.k0, stub.k1, stub.s).rotate(stub.th0);
        best_stub = m_progress;
    }

    vec2r best_p0 = last.p1;
    real best_th0 = stub.th0;
    real best_k0 = stub.k0;
    real best_k1 = stub.k1;
    real best_s = stub.s;
    real best_base_cost = best_cost;
    real best_cost_multiplier = 1;

    for (int i = 0; i < m_progress; ++i)
    {
        auto& e = m_extensors[i];
        e.active = e.active && e.fitter.push(sample.p);
        if (!e.active)
            continue;
        auto fit = e.fitter.fit();
        real base_cost = e.base_cost + fit.cost;
        real cost = e.cost_multiplier * base_cost;
        if (cost < best_cost)
        {
            best_p0 = e.fitter.p0();
            best_th0 = e.fitter.th0();
            best_k0 = e.fitter.k0();
            best_k1 = fit.k1;
            best_s = fit.s;
            best_cost = cost;
            best_stub = e.stub;
            best_base_cost = base_cost;
            best_cost_multiplier = e.cost_multiplier;
        }
    }

    // In very rare situations no spline reaches the sample
    if (best_stub == -1)
    {
        extensor.active = false;
        return false;
    }

    // Cast a vote on the best stub. Good results for later samples give more value.
    cout << "best stub: " << best_stub << "\n";
    m_stubs[best_stub].quality += sample.h;

    // Initialize the new extensor that starts at the current sample
    extensor.active = true;
    extensor.fitter.init(
        best_p0 + PreciseFresnel::eval(best_k0, best_k1, best_s).rotate(best_th0),
        best_th0 + best_s * (best_k0 + rl(0.5) * best_s * best_k1),
        best_k0 + best_s * best_k1);
    cout << "th0: " << extensor.fitter.th0() / pi<real>() << endl;
    extensor.stub = best_stub;
    extensor.base_cost = best_base_cost;
    extensor.cost_multiplier = best_cost_multiplier + 0.5; // TODO(mkc): Other possibilities

    ++m_progress;
    return true;
}

void Kletch::advance()
{
    cout << "== advancing: ================================\n\n";
    int best_quality = -inf<real>();
    int best_i = -1;
    for (int i = 0; i < m_progress; ++i)
    {
        cout << i;
        real quality = m_stubs[i].quality;
        if (m_stubs[i].quality >= best_quality)
        {
            best_quality = quality;
            best_i = i;
            cout << " * ";
        }
        else
            cout << "   ";
        cout << quality << "\n";
    }
    cout << endl;
    SegmentStub const& stub = m_stubs[best_i];
    Segment& segment = m_last_segment;
    segment.p0 = segment.p1;
    segment.th0 = stub.th0;
    segment.k0 = stub.k0;
    segment.k1 = stub.k1;
    segment.s = stub.s;
    segment.p1 = stub.p1;
    segment.sample_count = best_i + 1; // As a side effect, `sample_count = 0` will indicate error

    assert(segment.sample_count <= m_curve.size());
}

} // namespace kletch
