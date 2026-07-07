// Stubs for editor subsystems referenced by ported map I/O (sub_49FF90 / sub_4A07E0 callees).
#include "radiant_editor.h"

#include "radiant.h"

void RadiantEditor_ClearInUse()
{
    Radiant_Printf("Clear All In Use\n");
}

void RadiantEditor_PreLoadMap()
{
}

void RadiantEditor_UpdateLayers()
{
    Radiant_Printf("Updating layers...\n");
}

bool RadiantEditor_ParseMapFile(const char *normalizedPath)
{
    (void)normalizedPath;
    return true;
}
