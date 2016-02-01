#include "resources.h"
#include <cstring>
#include <unordered_map>

namespace kletch {

resource_not_found::resource_not_found(const std::string& resource_name) :
    m_resource_name(resource_name),
    m_what("No such resource: '" + resource_name + "'")
{ }

std::unordered_map<std::string, Resource> g_resources;

const Resource& get_resource(const std::string& name)
{
    auto it = g_resources.find(name);
    if (it == g_resources.end())
        throw resource_not_found(name);
    return it->second;
}

void add_resource(const char* name, const char* value)
{
    Resource r = { name, value, (int)std::strlen(value) };
    g_resources[name] = r;
}

} // namespace kletch
