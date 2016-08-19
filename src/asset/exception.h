#ifndef KLETCH_ASSET_EXCEPTION_H
#define KLETCH_ASSET_EXCEPTION_H

#include "prefix.h"

namespace kletch {

class asset_error : public std::runtime_error
{
public:
    asset_error(string const& name, string const& path, string const& message) :
        std::runtime_error("Asset error (" + name + "): " + message), m_name(name), m_path(path)
    { }

    string const& name() const { return m_name; }
    string const& path() const { return m_path; }

private:
    string m_name;
    string m_path;
};

class asset_not_found : public asset_error
{
public:
    asset_not_found(string const& name, string const& path) :
        asset_error(name, path, "can't open file " + path)
    { }
};

class asset_bad_format : public asset_error
{
public:
    asset_bad_format(string const& name, string const& path) :
        asset_error(name, path, "bad format")
    { }
};

} // namespace kletch

#endif // KLETCH_ASSET_EXCEPTION_H
