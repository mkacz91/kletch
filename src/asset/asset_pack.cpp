#include "asset_pack.h"

#include <sstream>

#include "exception.h"

namespace kletch {

void AssetPackBase::set_root_dir(string const& root_dir)
{
    if (m_root_dir_set)
        throw std::logic_error("AssetPackBase::root_dir already set");
    m_root_dir = root_dir;
    m_root_dir_set = true;
}

AssetPackBase::AssetStub AssetPackBase::open_asset(string const& name)
{
    AssetStub stub(name);
    string const& path = stub.header.path = m_root_dir + "/" + name + ".ass";
    std::ifstream& stream = stub.stream;
    stream.open(path, std::ios_base::binary);
    if (!stream.is_open())
        throw asset_not_found(name, path);

    // Read asset data size
    uint32_t size;
    stream.read(reinterpret_cast<char*>(&size), 4);
    if (!stream.good())
        throw asset_bad_format(name, path);
    stub.header.size = size;

    // Read asset source file path
    std::ostringstream source_path;
    stream.get(*source_path.rdbuf(), '\0');
    if (stream.bad() || stream.eof())
        throw asset_bad_format(name, path);
    if (stream.fail())
        stream.clear(); // failbit is set when no chars were read, but that's ok
    if (stream.get() != 0)
        throw asset_bad_format(name, path);
    stub.header.source_path = source_path.str();

    // Move to the next 4-byte boundary
    if (!stream.ignore((4 - stream.tellg() % 4) % 4).good())
        throw asset_bad_format(name, path);

    return stub;
}

} // namespace kletch
