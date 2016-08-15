#ifndef KLETCH_DEMO_ASSETS_H
#define KLETCH_DEMO_ASSETS_H

#include <asset/all.h>

namespace kletch {

typedef StringAssetPack ShaderAssetPack;
typedef Asset<string> ShaderAsset;

struct Assets
{
    static ShaderAssetPack shaders;
};

} // namespace kletch

#endif // KLETCH_DEMO_ASSETS_H
