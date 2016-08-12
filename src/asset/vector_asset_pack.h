#ifndef KLETCH_ASSET_VECTOR_ASSET_PACK_H
#define KLETCH_ASSET_VECTOR_ASSET_PACK_H

#include "prefix.h"
#include <vector>

#include "asset_pack.h"

namespace kletch {

template <class T>
class VectorAssetPack : public AssetPack<std::vector<T>>
{
protected:
    std::vector<T> const* load(AssetHeader const& header, std::istream& stream) override;
};

template <class T>
using VectorAsset = Asset<std::vector<T>>;

template <class T>
std::vector<T> const* VectorAssetPack<T>::load(AssetHeader const& header, std::istream& stream)
{
    int count = (header.size + sizeof(T) - 1) / sizeof(T); // Take ceil to avoid acces violation
    std::vector<T>* data = new std::vector<T>(count);
    stream.read(reinterpret_cast<char*>(data->data()), header.size);
    return data;
}

} // namespace kletch

#endif // KLETCH_ASSET_VECTOR_ASSET_PACK_H
