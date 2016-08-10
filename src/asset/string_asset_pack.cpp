#include "string_asset_pack.h"

namespace kletch {

StringAssetPack::StringAssetPack(string const& root_dir) : AssetPack(root_dir) { }

string const* StringAssetPack::load(AssetHeader const& header, std::istream& stream) override
{
    char* c_data = new char[header.size];
    stream.read(c_data, header.size);
    string* data = new string(c_data, header.size);
    delete[] c_data;
    return data;
}

} // namespace kletch
