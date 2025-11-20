#pragma once

void __cdecl Flame_Cull_Stream_Chunk(bool is_server, flameChunk_s *cull, flameChunk_s *keep);
void __cdecl Flame_Cull_Stream_Chunks(bool is_server, flameStream_s *stream);
