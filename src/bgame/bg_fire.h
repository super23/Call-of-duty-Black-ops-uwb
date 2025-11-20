#pragma once

void __cdecl BG_Flame_RegisterDvars();
void __cdecl BG_InitFire();
void __cdecl BG_ShutdownFire();
void __cdecl BG_ResetFire();
bool __cdecl TerrainScorch_AllowSurfacesCallback(int surfIndex);
void __cdecl TerrainScorch(float *loresCellPos, bool instant);
unsigned __int16 __cdecl HalfFromFloat(float floatIn);
double __cdecl FloatFromHalf(unsigned __int16 halfIn);
void __cdecl CG_UpdateFire(int localClientNum);
void __cdecl CG_RenderFire();
double __cdecl GetFoliageBurnRadius(TerrainFireSource fireSrc);
void __cdecl CG_SetFireToWall(int localClientNum, float *pos, int *models, int models_count, TerrainFireSource fireSrc);
void __cdecl CG_SetFireToTerrain(
        float *pos,
        float radius,
        int fireStarterClientNum,
        int weaponUsed,
        float pSpread,
        int *models,
        int models_count,
        TerrainFireSource fireSrc);
void __cdecl AddBurnCell(float *pos);
void __cdecl SetTerrainScorchInitialState();
void __cdecl Sync_VisualBurn_To_FM_State();
void __cdecl MemFile_WriteInt(MemoryFile *memFile, int value);
void __cdecl CG_GenerateFireSounds(int localClientNum);
