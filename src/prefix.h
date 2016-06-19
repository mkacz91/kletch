#ifndef KLETCH_PREFIX_H
#define KLETCH_PREFIX_H

#include <cassert>
#include <string>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace kletch {

using std::string;

using std::tuple;
using std::tie;
using std::make_tuple;
using std::get;

using std::swap;

inline void ignore() { }
template <class T, class... Ts> inline
void ignore(T const& x, Ts... xs) { (void)x; ignore(&xs...); }

} // namespace kletch

#endif // KLETCH_PREFIX_H
