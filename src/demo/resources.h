#ifndef KLETCH_RESOURCES_H
#define KLETCH_RESOURCES_H

#include <string>
#include <exception>

namespace kletch {

class resource_not_found : public std::exception
{
public:
    resource_not_found(const std::string& resource_name);
    const std::string& resource_name() const noexcept { return m_resource_name; }
    virtual const char* what() const noexcept override { return m_what.c_str(); }

private:
    std::string m_resource_name;
    std::string m_what;
};

struct Resource
{
    std::string name;
    const char* value;
    int length;
};

const Resource& get_resource(const std::string& name);

} // namespace kletch

#endif // KLETCH_RESOURCES_H
