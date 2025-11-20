#pragma once

void __cdecl CG_AddLagometerFrameInfo(const cg_s *cgameGlob);
void __cdecl CG_AddLagometerSnapshotInfo(snapshot_s *snap);
void __cdecl CG_DrawLagometer(int localClientNum);
void __cdecl CG_DrawDisconnect(int localClientNum);
void __cdecl CG_DrawEntityCount(int localClientNum);
void __cdecl CG_DrawServerBandwidth(int localClientNum);
void __cdecl CG_DrawServerKillcamData(int localClientNum);
