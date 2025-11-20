#pragma once

void __cdecl CG_ClearEntityCollWorld(int localClientNum);
void __cdecl CG_UnlinkEntityColl(int localClientNum, unsigned int entIndex);
void __cdecl CG_LinkEntityColl(int localClientNum, unsigned int entIndex, const float *absMins, const float *absMaxs);
void __cdecl CG_AddEntityToCollSector(int localClientNum, unsigned int entIndex, unsigned __int16 sectorIndex);
void __cdecl CG_SortEntityCollSector(
        int localClientNum,
        unsigned __int16 sectorIndex,
        const float *mins,
        const float *maxs);
unsigned __int16 __cdecl CG_AllocEntityCollSector(int localClientNum, const float *mins, const float *maxs);
