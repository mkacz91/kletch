#ifndef KLETCH_RES_H
#define KLETCH_RES_H

#include <string>

namespace kletch {

struct Resource
{
    std::string name;
    const char* value;
    int length;

    Resource(const char* name, const char* value);
};

namespace res {
    namespace shaders {
        #include <gen/shaders.h>
    }
}

} // namespace kletch

#endif // KLETCH_RES_H
