#ifndef KLETCH_PREFIX_H
#define KLETCH_PREFIX_H

#include <memory>
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <sstream>

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
inline string quote(const string& s) { return "\"" + s + "\""; }
inline string squote(const string& s) { return "'" + s + "'"; }

template <typename T>
std::string to_string(const T& t)
{
    std::ostringstream sout;
    sout << t;
    return sout.str();
}

using std::tuple;
using std::tie;
using std::make_tuple;
using std::get;

using std::swap;

template <typename T>
void unused(T const& arg) { (void)arg; }
template <typename T1, typename T2>
void unused(T1 const& arg1, T2 const& arg2) { (void)arg1; (void)arg2; }

} // namespace kletch

#endif // KLETCH_PREFIX_H
