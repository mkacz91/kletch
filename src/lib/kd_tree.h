#ifndef KLETCH_LIB_KD_TREE_H
#define KLETCH_LIB_KD_TREE_H

#include "prefix.h"
#include <stack>
#include <vector>

namespace kletch {

template <class T> struct KdTreeTraits { };

class KdTree
{
public:
    template <class T, class Traits = KdTreeTraits<T>> static
    void build(std::vector<T>& items);

    template <class T, class Traits = KdTreeTraits<T>> static
    T const& get_nearest(std::vector<T> const& items, typename Traits::vector_t const& p);

    template <class T, class Traits = KdTreeTraits<T>>
    static std::vector<float> get_lines(
        std::vector<T> const& items, typename Traits::scalar_t rel_margin = 0);

private:
    static constexpr int MAX_LEAF_SIZE = 4;

    template <class Range> static int size(Range const& r) { return r.end - r.begin; }
    template <class Range> static int mid(Range const& r) { return (r.begin + r.end) >> 1; }
};

template <class T> struct KdTreeTraits<vec2<T>>
{
    typedef typename vec2<T>::scalar_t scalar_t;
    typedef vec2<T> vector_t;
    static constexpr int COMPONENT_COUNT = vec2<T>::COMPONENT_COUNT;
    static scalar_t component(vec2<T> const& u, int i) { return u[i]; }
    static scalar_t dist_sq(vector_t const& u, vec2<T> const& v) { return vec2<T>::dist_sq(u, v); }
};

template <class T, class Traits> void KdTree::build(std::vector<T>& items)
{
    struct Range { int begin, end, level; };

    T* const data = items.data();
    std::stack<Range> rs;
    rs.push({ 0, (int)items.size(), 0 });
    while (!rs.empty())
    {
        auto r = rs.top(); rs.pop();
        if (size(r) > MAX_LEAF_SIZE)
        {
            int c = r.level % Traits::COMPONENT_COUNT;
            int mid = KdTree::mid(r);
            std::sort(data + r.begin, data + r.end, [c](T const& a, T const& b) {
                return Traits::component(a, c) < Traits::component(b, c); });
            rs.push({ mid + 1, r.end, r.level + 1 });
            rs.push({ r.begin,   mid, r.level + 1 });
        }
    }
}

template <class T, class Traits>
T const& KdTree::get_nearest(std::vector<T> const& items, typename Traits::vector_t const& p)
{
    struct Range { int begin, end, level; typename Traits::scalar_t delta; };

    auto best_dist_sq = inf<typename Traits::scalar_t>();
    int best_i = -1;
    auto try_item = [&](int i)
    {
        auto dist_sq = Traits::dist_sq(p, items[i]);
        if (dist_sq < best_dist_sq)
        {
            best_dist_sq = dist_sq;
            best_i = i;
        }
    };

    std::stack<Range> rs;
    rs.push({ 0, (int)items.size(), 0, 0 });
    while (!rs.empty())
    {
        auto r = rs.top(); rs.pop();
        if (r.delta <= 0 || sq(r.delta) < best_dist_sq)
        {
            if (size(r) > MAX_LEAF_SIZE)
            {
                int c = r.level % Traits::COMPONENT_COUNT;
                int mid = KdTree::mid(r);
                auto delta = Traits::component(p, c) - Traits::component(items[mid], c);
                rs.push({ mid + 1, r.end, r.level + 1, -delta });
                rs.push({ r.begin,   mid, r.level + 1,  delta });
                try_item(mid);
            }
            else
            {
                for (int i = r.begin; i < r.end; ++i)
                    try_item(i);
            }
        }
    }

    return items[best_i];
}

template <class T, class Traits> std::vector<float>
KdTree::get_lines(std::vector<T> const& items, typename Traits::scalar_t rel_margin)
{
    assert(Traits::COMPONENT_COUNT == 2);

    struct Range { int begin, end, level; box2f box; };

    std::vector<float> lines;
    auto add_hline = [&lines](float x0, float x1, float y)
    {
        lines.push_back(x0); lines.push_back(y);
        lines.push_back(x1); lines.push_back(y);
    };
    auto add_vline = [&lines](float y0, float y1, float x)
    {
        lines.push_back(x); lines.push_back(y0);
        lines.push_back(x); lines.push_back(y1);
    };

    box2f box = box2f::EMPTY;
    for (T const& item : items)
        box.expand(item[0], item[1]);
    float margin = 0.5 * rel_margin * (box.span_x() + box.span_y());
    box.x0 -= margin; box.x1 += margin;
    box.y0 -= margin; box.y1 += margin;

    add_hline(box.x0, box.x1, box.y0);
    add_hline(box.x0, box.x1, box.y1);
    add_vline(box.y0, box.y1, box.x0);
    add_vline(box.y0, box.y1, box.x1);

    std::stack<Range> rs;
    rs.push({ 0, (int)items.size(), 0, box });
    while (!rs.empty())
    {
        auto r = rs.top(); rs.pop();
        if (size(r) > MAX_LEAF_SIZE)
        {
            box2f box0 = r.box, box1 = r.box;
            int mid = KdTree::mid(r);
            int c = r.level % 2;
            T const& pivot = items[mid];
            switch (c) {
            case 0:
            {
                float x = pivot[0];
                add_vline(r.box.y0, r.box.y1, x);
                box0.x1 = box1.x0 = x;
                break;
            }
            case 1:
            {
                float y = pivot[1];
                add_hline(r.box.x0, r.box.x1, y);
                box0.y1 = box1.y0 = y;
                break;
            }}
            rs.push({ mid + 1, r.end, r.level + 1, box1 });
            rs.push({ r.begin,   mid, r.level + 1, box0 });
        }
    }

    return lines;
}

} // namespace kletch

#endif // KLETCH_LIB_QUAD_TREE_H
