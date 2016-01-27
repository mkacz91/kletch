#ifndef KLETCH_PREFIX_H
#define KLETCH_PREFIX_H

#include <memory>
#include <cassert>
#include <iostream>
#include <string>

#include "math/math.h"

namespace kletch {

using std::unique_ptr;
template <typename T>
inline unique_ptr<T> make_unique(T* ptr) { return unique_ptr<T>(ptr); }

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using std::string;
using std::to_string;

} // namespace kletch

#endif // KLETCH_PREFIX_H
