#pragma once

void __cdecl Flame_Class_Chunk_Init();
flameChunk_s *__cdecl Flame_Class_Chunk_Alloc(bool is_server);
void __cdecl Flame_Class_Chunk_Free(bool is_server, flameChunk_s *chunk);
void __cdecl Flame_Phys_Spawn_Items(bool is_server, int curTime, flameChunk_s *trav);
void __cdecl Flame_Class_Chunk_Age(bool is_server, int time);
flameChunk_s *__cdecl Flame_Class_Chunk_Spawn(
        bool is_server,
        flameChunkSpawnVars_t *spawnVars,
        flameGeneric_s **chunkList,
        float *velocityAdd,
        flameTable *fTable);
void __cdecl Flame_Class_Chunk_Render_Item(int localClientNum, flameChunk_s *chunk);
void __cdecl Flame_Class_Chunk_Render_All(int localClientNum);
