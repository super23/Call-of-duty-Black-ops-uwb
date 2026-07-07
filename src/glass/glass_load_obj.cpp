#include "glass_load_obj.h"

#include "glass.h"

#include <cstring>

#include <database/database.h>
#include <qcommon/common.h>
#include <universal/com_files.h>
#include <universal/com_memory.h>

namespace
{
constexpr unsigned int kMaxGlasses = 0x3E8u;
constexpr unsigned int kGlassesArrayBytes = kMaxGlasses * sizeof(Glass); // retail: operator new[](0x1E460u)
} // namespace

bool glassesInited;
Glasses glasses;

// Decomp: CoDMPServer.c:855693 — Retail: BlackOpsMP.retail.c:sub_498620 @ 0x498620
Glasses *__cdecl GetGlasses_LoadObj()
{
    if (!glassesInited)
    {
        glassesInited = true;
        memset(&glasses, 0, sizeof(glasses));
        glasses.glasses = reinterpret_cast<Glass *>(operator new[](kGlassesArrayBytes));
        glasses.numGlasses = 0;
        glasses.name = nullptr;
    }
    return &glasses;
}

// Decomp: CoDMPServer.c:855707 — Retail: BlackOpsMP.retail.c:sub_4AE1E0 @ 0x4AE1E0 (fastcall; header via DB)
Glasses *__cdecl GetGlasses_FastFile()
{
    XAssetHeader header;

    if (DB_GetAllXAssetOfType(ASSET_TYPE_GLASSES, &header, 1) <= 0)
        return nullptr;

    return reinterpret_cast<Glasses *>(header.xmodelPieces);
}

// Decomp: CoDMPServer.c:855721 — Retail: BlackOpsMP.retail.c:sub_4BCE70 @ 0x4BCE70
Glasses *__cdecl GetGlasses()
{
    Glasses *(__cdecl *getGlasses)();
    const bool usingMods = fs_gameDirVar && *reinterpret_cast<const unsigned char *>(fs_gameDirVar->current.integer);

    if (usingMods || !useFastFile->current.enabled)
        getGlasses = GetGlasses_LoadObj;
    else
        getGlasses = GetGlasses_FastFile;

    return getGlasses();
}
