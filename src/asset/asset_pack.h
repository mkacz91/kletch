#ifndef KLETCH_ASSET_ASSET_PACK_H
#define KLETCH_ASSET_ASSET_PACK_H

#include "prefix.h"
#include <unordered_map>
#include <fstream>

#include "asset.h"
#include "asset_header.h"

namespace kletch {

class AssetPackBase
{
public:
    void set_root_dir(string const& root_dir);

protected:
    struct AssetStub
    {
        AssetHeader header;
        std::ifstream stream;

        AssetStub(string const& name) :  header(name) { }
    };

    AssetStub open_asset(string const& name);

private:
    string m_root_dir;
    bool m_root_dir_set = false;
};

template <class T>
class AssetPack : public AssetPackBase
{
public:
    AssetPack() = default;
    AssetPack(AssetPack<T> const& other) = delete;

    Asset<T>& operator [] (string const& name);

protected:
    virtual T const* load(AssetHeader const& header, std::istream& stream) = 0;

private:
    std::unordered_map<string, Asset<T>> m_assets;
};

template <class T>
Asset<T>& AssetPack<T>::operator [] (string const& name)
{
    auto it = m_assets.find(name);
    if (it != m_assets.end() && it->second.has_data())
        return it->second;
    AssetStub stub = open_asset(name);
    T const* data = load(stub.header, stub.stream);
    return m_assets.emplace(name, Asset<T>(stub.header, data)).first->second;
}

} // namespace kletch

#endif // KLETCH_ASSET_ASSET_PACK_H
