#ifndef KLETCH_ASSET_PREFIX_H
#define KLETCH_ASSET_PREFIX_H

#include "../prefix.h"

#ifdef KLETCH_WINDOWS
    constexpr char PATH_SEPARATOR = '\\';
#else
    constexpr char PATH_SEPARATOR = '/';
#endif

#endif // KLETCH_ASSET_PREFIX_H
