// CoDBORadiant editor — shared declarations (GLUE + EDITOR_PORT stubs).
#pragma once

#ifndef KISAK_RADIANT_EDITOR
#error "src/radiant requires KISAK_RADIANT_EDITOR=1"
#endif

#include <stdbool.h>

#include "GfxWrapper.h"
#include "editor/map_io.h"

#ifdef __cplusplus
extern "C" {
#endif

void Radiant_Printf(const char *fmt, ...);
[[noreturn]] void Radiant_Error(const char *file, int line, int fatal, const char *fmt, ...);

void RadiantRemote_Init(void);
void RadiantRemote_Shutdown(void);
void RadiantRemote_Frame(void);

#ifdef __cplusplus
}
#endif
