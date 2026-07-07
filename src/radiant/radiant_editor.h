#pragma once

// Editor internals ported from CoDBORadiant.decompile.c (brush/entity/layers TBD).
void RadiantEditor_ClearInUse();
void RadiantEditor_PreLoadMap();
void RadiantEditor_UpdateLayers();
bool RadiantEditor_ParseMapFile(const char *normalizedPath);
