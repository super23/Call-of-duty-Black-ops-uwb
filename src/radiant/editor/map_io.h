#pragma once

#include <windows.h>

extern char g_RadiantMapFilename[MAX_PATH];

bool Map_LoadFile(const char *filename);
bool Map_SaveFile(const char *filename, bool autosave);
