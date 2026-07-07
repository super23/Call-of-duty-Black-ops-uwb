// EDITOR_PORT — map load/save from CoDBORadiant.decompile.c sub_49FF90 / sub_4A07E0.
#include "map_io.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../radiant.h"
#include "../radiant_editor.h"

char g_RadiantMapFilename[MAX_PATH] = "unnamed.map";

static void NormalizeMapPath(char *dst, size_t dstSize, const char *src)
{
    if (!dst || dstSize == 0)
        return;

    dst[0] = '\0';
    if (!src)
        return;

    size_t n = 0;
    for (; src[n] && n + 1 < dstSize; ++n)
        dst[n] = (src[n] == '\\') ? '/' : src[n];
    dst[n] = '\0';
}

static void MakeSiblingExtension(char *out, size_t outSize, const char *path, const char *fromExt, const char *toExt)
{
    std::strncpy(out, path, outSize - 1);
    out[outSize - 1] = '\0';

    const size_t len = std::strlen(out);
    const size_t fromLen = std::strlen(fromExt);
    if (len >= fromLen && std::strcmp(out + len - fromLen, fromExt) == 0)
        std::strcpy(out + len - fromLen, toExt);
    else
        std::strncat(out, toExt, outSize - len - 1);
}

static bool CopyFileBinary(const char *src, const char *dst)
{
    FILE *in = std::fopen(src, "rb");
    if (!in)
        return false;

    std::fseek(in, 0, SEEK_END);
    const long size = std::ftell(in);
    std::fseek(in, 0, SEEK_SET);
    if (size < 0)
    {
        std::fclose(in);
        return false;
    }

    char *buf = static_cast<char *>(std::malloc(static_cast<size_t>(size)));
    if (!buf)
    {
        std::fclose(in);
        return false;
    }

    const size_t readBytes = std::fread(buf, 1, static_cast<size_t>(size), in);
    std::fclose(in);
    if (readBytes != static_cast<size_t>(size))
    {
        std::free(buf);
        return false;
    }

    FILE *out = std::fopen(dst, "wb");
    if (!out)
    {
        std::free(buf);
        return false;
    }

    const size_t wrote = std::fwrite(buf, 1, static_cast<size_t>(size), out);
    std::fclose(out);
    std::free(buf);
    return wrote == static_cast<size_t>(size);
}

static void MaybeRestoreAutosave(const char *mapPath)
{
    char bakPath[MAX_PATH];
    char autosavePath[MAX_PATH];
    MakeSiblingExtension(bakPath, sizeof(bakPath), mapPath, ".map", ".bak");
    MakeSiblingExtension(autosavePath, sizeof(autosavePath), bakPath, ".bak", ".autosave1");

  if (std::fopen(autosavePath, "rb") == nullptr)
        return;

    const int choice = MessageBoxA(
        GetActiveWindow(),
        "Radiant found an autosave file for the current map. Do you want to open it instead "
        "(a backup of your map will be saved in a .bak file)?",
        "Radiant",
        MB_YESNO | MB_ICONQUESTION);

    if (choice == IDYES)
    {
        std::remove(mapPath);
        std::rename(autosavePath, mapPath);
    }
    else
    {
        std::remove(autosavePath);
    }
}

bool Map_LoadFile(const char *filename)
{
    if (!filename || !filename[0])
        return false;

    char normalized[MAX_PATH];
    NormalizeMapPath(normalized, sizeof(normalized), filename);
    Radiant_Printf("Map_LoadFile: %s\n", normalized);

    RadiantEditor_PreLoadMap();
    RadiantEditor_ClearInUse();

    std::strncpy(g_RadiantMapFilename, filename, MAX_PATH - 1);
    g_RadiantMapFilename[MAX_PATH - 1] = '\0';

    if (std::strcmp(filename, "unnamed.map") != 0)
    {
        char bakPath[MAX_PATH];
        MakeSiblingExtension(bakPath, sizeof(bakPath), filename, ".map", ".bak");
        CopyFileBinary(filename, bakPath);
        MaybeRestoreAutosave(filename);
    }

    RadiantEditor_UpdateLayers();
    return RadiantEditor_ParseMapFile(normalized);
}

bool Map_SaveFile(const char *filename, bool autosave)
{
    if (!filename || !filename[0])
        return false;

    char normalized[MAX_PATH];
    NormalizeMapPath(normalized, sizeof(normalized), filename);
    Radiant_Printf("Map_SaveFile: %s%s\n", normalized, autosave ? " (autosave)" : "");

    std::strncpy(g_RadiantMapFilename, filename, MAX_PATH - 1);
    g_RadiantMapFilename[MAX_PATH - 1] = '\0';

    // Full Quake3 .map writer still lives in decomp sub_4A07E0 — hook when brush/entity port lands.
    (void)autosave;
    return true;
}
