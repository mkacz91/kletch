#ifndef KLETCH_PREFIX_H
#define KLETCH_PREFIX_H

#include <iostream>
#include <string>
#include <cmath>

namespace kletch {

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using std::string;

using std::sin;
using std::cos;

#ifdef KLETCH_USE_DOUBLES
    typedef double real;
#   define rl(number) (number)
#else
    typedef float real;
#   define rl(number) (number##f)
#endif

} // namespace kletch

#endif // KLETCH_PREFIX_H
