#ifndef KLETCH_ASSET_ASSET_PACK_H
#define KLETCH_ASSET_ASSET_PACK_H

#include "prefix.h"
#include <unordered_map>
#include <fstream>

#include "asset.h"

namespace kletch {

struct AssetHeader
{
    string const& name; // Asset name
    string path; // Path to the compiled asset (.ass) file
    string source_path; // Path to the asset source file. May be empty string.
    size_t size; // Asset data size.

    AssetHeader(string const& name) : name(name) { }
};

class AssetPackBase
{
protected:
    struct AssetStub
    {
        AssetHeader header;
        std::ifstream stream;

        AssetStub(string const& name) :  header(name) { }
    };

    AssetStub open_asset(string const& name);

private:
    string const m_root_dir;
};

template <class T>
class AssetPack
{
public:
    explicit AssetPack(string const& root_dir);
    AssetPack(AssetPack<T> const& other) = delete;

    Asset<T>& operator [] (string const& name);

protected:
    virtual T const* load(AssetHeader const& header, std::istream& stream) = 0;

private:
    std::unordered_map<string, Asset<T>> m_assets;
    string const m_root_dir;
};

template <class T>
AssetPack<T>::AssetPack(string const& root_dir) : m_root_dir(root_dir) { }

template <class T>
Asset<T>& AssetPack<T>::operator [] (string const& name)
{
    auto it = m_assets.find(name);
    if (it != m_assets.end() && it->second.has_data())
        return *it;
    AssetStub stub = open_asset(name);
    T const* data = load(stub.header, stub.stream);
    auto status = m_assets.emplace(name, data); // returns (iterator, bool)
    return status.fists->second;
}

} // namespace kletch

#endif // KLETCH_ASSET_ASSET_PACK_H
