#ifndef KLETCH_LIB_KD_TREE_H
#define KLETCH_LIB_KD_TREE_H

#include "prefix.h"
#include <stack>
#include <vector>

namespace kletch {

class KdTree
{
public:
    template <class T> static void build(std::vector<T>& items);
    template <class T> static T const& get_nearest(std::vector<T> const& items, T const& p);

    template <class T>
    static std::vector<float> get_lines(std::vector<T> const& items, float rel_margin = 0);

private:
    static constexpr int MAX_LEAF_SIZE = 4;

    template <class T> struct CompareComponent
    {
        int index;
        CompareComponent(int index) : index(index) { }
        bool operator () (T const& a, T const& b) { return a[index] < b[index]; }
    };

    struct Range
    {
        int begin, end, level;
        Range(int begin, int end, int level) : begin(begin), end(end), level(level) { }
        int mid() { return (begin + end) / 2; }
    };
};

template <class T> void KdTree::build(std::vector<T>& items)
{
    T* const data = items.data();
    std::stack<Range> s;
    s.emplace(0, items.size(), 0);
    while (!s.empty())
    {
        Range r = s.top(); s.pop();
        if (r.end - r.begin > MAX_LEAF_SIZE)
        {
            int c = r.level % T::COMPONENT_COUNT;
            std::sort(data + r.begin, data + r.end, CompareComponent<T>(c));
            int mid = r.mid();
            s.emplace(mid + 1,   r.end, r.level + 1);
            s.emplace(r.begin, mid - 1, r.level + 1);
        }
    }
}

template <class T> T const& KdTree::get_nearest(std::vector<T> const& items, T const& item)
{
    auto best_dist_sq = inf<typename T::scalar_t>();
    int best_i = -1;
    auto try_item = [&](int i)
    {
        auto dist_sq = T::dist_sq(item, items[i]);
        if (dist_sq < best_dist_sq)
        {
            best_dist_sq = dist_sq;
            best_i = i;
        }
    };

    std::stack<Range> s;
    s.emplace(0, items.size(), 0);
    while (!s.empty())
    {
        Range r = s.top(); s.pop();
        if (r.end - r.begin > MAX_LEAF_SIZE)
        {
            int mid = r.mid();
            int c = r.level % T::COMPONENT_COUNT;
            auto c_dist = item[c] - items[mid][c];
            if (c_dist >= 0 || sq(c_dist) < best_dist_sq)
                s.emplace(mid + 1,   r.end, r.level + 1);
            if (c_dist <= 0 || sq(c_dist) < best_dist_sq)
                s.emplace(r.begin, mid - 1, r.level + 1);
            try_item(mid);
        }
        else
        {
            for (int i = r.begin; i < r.end; ++i)
                try_item(i);
        }
    }

    return items[best_i];
}

template <class T> std::vector<float>
KdTree::get_lines(std::vector<T> const& items, float rel_margin)
{
    assert(T::COMPONENT_COUNT == 2);

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

    std::stack<Range> ranges;
    std::stack<box2f> boxes;
    ranges.emplace(0, items.size(), 0);
    boxes.push(box);
    while (!ranges.empty())
    {
        Range r = ranges.top(); ranges.pop();
        box2f b = boxes.top(); boxes.pop();
        if (r.end - r.begin > MAX_LEAF_SIZE)
        {
            box2f b0 = b, b1 = b;
            int mid = r.mid();
            int c = r.level % 2;
            T const& pivot = items[mid];
            switch (c) {
            case 0:
            {
                float x = pivot[0];
                add_vline(b.y0, b.y1, x);
                b0.x1 = b1.x0 = x;
                break;
            }
            case 1:
            {
                float y = pivot[1];
                add_hline(b.x0, b.x1, y);
                b0.y1 = b1.y0 = y;
                break;
            }}
            ranges.emplace(mid + 1,   r.end, r.level + 1);
            ranges.emplace(r.begin, mid - 1, r.level + 1);
            boxes.push(b1);
            boxes.push(b0);
        }
    }

    return lines;
}

} // namespace kletch

#endif // KLETCH_LIB_QUAD_TREE_H
