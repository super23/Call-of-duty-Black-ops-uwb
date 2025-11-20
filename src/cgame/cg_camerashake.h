#pragma once

void __cdecl CG_StartShakeCamera(int localClientNum, float p, int duration, float *src, float radius);
int __cdecl CG_UpdateCameraShake(const cg_s *cgameGlob, CameraShake *shake);
void __cdecl CG_ShakeCamera(int localClientNum);
void __cdecl MemFile_WriteData(MemoryFile *memFile, unsigned int byteCount, unsigned __int8 *p);
void __cdecl CG_ClearCameraShakes(int localClientNum);
