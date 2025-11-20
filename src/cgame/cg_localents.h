#pragma once

void __cdecl CG_InitLocalEntities(int localClientNum);
localEntity_s *__cdecl CG_AllocLocalEntity(int localClientNum);
void __cdecl CG_FreeLocalEntity(int localClientNum, localEntity_s *le);
void __cdecl CG_AddLocalEntityTracerBeams(int localClientNum);
void __cdecl CG_AddMovingTracer(const cg_s *cgameGlob, localEntity_s *le, const refdef_s *refdef);
