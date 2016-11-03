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

} // namespace kletch

#endif // KLETCH_LIB_QUAD_TREE_H
