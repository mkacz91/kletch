#ifndef KLETCH_STRING_ASSET_PACK_H
#define KLETCH_STRING_ASSET_PACK_H

#include "prefix.h"
#include "asset_pack.h"

namespace kletch {

class StringAssetPack : public AssetPack<string>
{
protected:
    string const* load(AssetHeader const& header, std::istream& stream) override;
};

} // namespace kletch

#endif // KLETCH_STRING_ASSET_PACK_H
