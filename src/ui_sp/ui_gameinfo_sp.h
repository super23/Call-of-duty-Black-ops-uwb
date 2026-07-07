#pragma once
#include <database/db_registry.h>
#include <universal/dvar.h>

extern const dvar_t *ui_browserMap;

void __cdecl UI_LoadMaps();
void __cdecl UI_LoadArenas();
void __cdecl UI_LoadCustomMatchGameTypes();
