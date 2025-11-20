#pragma once

int __cdecl deflateInit2_(
        z_stream_s *strm,
        int level,
        int method,
        int windowBits,
        int memLevel,
        unsigned int strategy,
        const char *version,
        int stream_size);
int __cdecl deflateReset(z_stream_s *strm);
int __cdecl deflate(z_stream_s *strm, unsigned int flush);
void __cdecl putShortMSB(internal_state *s, __int16 b);
void __cdecl flush_pending(z_stream_s *strm);
unsigned int __cdecl deflateEnd(z_stream_s *strm);
void __cdecl lm_init(internal_state *s);
int __cdecl deflate_stored(internal_state *s, int flush);
void __cdecl fill_window(internal_state *s);
unsigned int __cdecl read_buf(z_stream_s *strm, unsigned __int8 *buf, unsigned int size);
unsigned int __cdecl ClampShortNonNegative(__int16 value);
int __cdecl deflate_fast(internal_state *s, int flush);
unsigned int __cdecl longest_match_fast(internal_state *s, unsigned int cur_match);
