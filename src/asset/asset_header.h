#ifndef KLETCH_ASSET_ASSET_HEADER_H
#define KLETCH_ASSET_ASSET_HEADER_H

namespace kletch {

struct AssetHeader
{
    string const& name; // Asset name
    string path; // Path to the compiled asset (.ass) file
    string source_path; // Path to the asset source file. May be empty string.
    size_t size; // Asset data size.

    AssetHeader(string const& name) : name(name) { }
};

} // namespace kletch

#endif // KLETCH_ASSET_ASSET_HEADER_H
