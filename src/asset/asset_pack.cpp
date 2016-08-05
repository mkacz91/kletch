#include "asset_pack.h"
#include <sstream>

namespace kletch {

AssetPackBase::AssetStub AssetPackBase::open_asset(string const& name)
{
    AssetStub stub(name);
    stub.header.path = m_root_dir + PATH_SEPARATOR + name + ".ass";
    stub.stream.exceptions(std::ifstream::failbit);
    stub.stream.open(stub.header.path);

    // Read asset data size
    uint32_t size;
    stub.stream.get(reinterpret_cast<char*>(&size), 4);
    stub.header.size = size;

    // Read asset source file path
    try
    {
        std::ostringstream source_path;
        stub.stream.get(*source_path.rdbuf(), '\0');
        stub.header.source_path = source_path.str();
    }
    catch (std::ios_base::failure)
    {
        // stub.stream.get() will set failbit if it won't read any chars. An empty source_path is
        // a valid value, meaning that the asset has no corresponding source. Just ignore the
        // exception.
    }

    return stub;
}

} // namespace kletch
