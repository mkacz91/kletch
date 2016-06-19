#ifndef KLETCH_PRINTING_H
#define KLETCH_PRINTING_H

#include <iostream>
#include <sstream>

namespace kletch
{
    using std::cin;
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::flush;

    inline string quote(const string& s) { return "\"" + s + "\""; }
    inline string squote(const string& s) { return "'" + s + "'"; }

    template <typename T>
    std::string to_string(const T& t)
    {
        std::ostringstream sout;
        sout << t;
        return sout.str();
    }
}

#endif // KLETCH_PRINTING_H
