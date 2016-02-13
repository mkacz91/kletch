#ifndef KLETCH_DEMO_INT_RANGE_H
#define KLETCH_DEMO_INT_RANGE_H

namespace kletch {

struct int_range
{
    int first;
    int count;

    int end() const { return first + count; }

    int_range() = default;

    int_range(int first, int count) : first(first), count(count) { }
};

} // namespace kletch

#endif // KLETCH_DEMO_INT_RANGE_H
