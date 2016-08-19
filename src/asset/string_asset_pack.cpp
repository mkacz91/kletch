#include "string_asset_pack.h"

namespace kletch {

string const* StringAssetPack::load(AssetHeader const& header, std::istream& stream)
{
    char* c_data = new char[header.size];
    stream.read(c_data, header.size);
    if (!stream || stream.gcount() != header.size)
        throw asset_bad_format(header.name, header.path);
    string* data = new string(c_data, header.size);
    delete[] c_data;
    return data;
}

} // namespace kletch
