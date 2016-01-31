#include "resources.h"

#include <cstring>

namespace kletch {

Resource::Resource(const char* name, const char* value) :
    name(name),
    value(value),
    length(std::strlen(value))
{ }

namespace res {
    namespace shaders {
        #include <gen/shaders.cpp>
    }
}

} // namespace kletch
