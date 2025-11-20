#include "deflate.h"

int __cdecl deflateInit2_(
        z_stream_s *strm,
        int level,
        int method,
        int windowBits,
        int memLevel,
        unsigned int strategy,
        const char *version,
        int stream_size)
{
  int wrap; // [esp+0h] [ebp-Ch]
  unsigned __int16 *overlay; // [esp+4h] [ebp-8h]
  internal_state *s; // [esp+8h] [ebp-4h]
  bool levela; // [esp+18h] [ebp+Ch]

  wrap = 1;
  if ( !version || *version != my_version[0] || stream_size != 52 )
    return -6;
  if ( !strm )
    return -2;
  strm->msg = 0;
  if ( !strm->zalloc )
  {
    strm->zalloc = zcalloc;
    strm->opaque = 0;
  }
  if ( !strm->zfree )
    strm->zfree = zcfree;
  levela = level != 0;
  if ( windowBits < 0 )
  {
    wrap = 0;
    windowBits = -windowBits;
  }
  if ( memLevel < 1 || memLevel > 9 || method != 8 || windowBits < 8 || windowBits > 15 || strategy > 4 )
    return -2;
  if ( windowBits == 8 )
    windowBits = 9;
  s = (internal_state *)strm->zalloc(strm->opaque, 1, 5824);
  if ( !s )
    return -4;
  strm->state = s;
  s->strm = strm;
  s->wrap = wrap;
  s->gzhead = 0;
  s->w_bits = windowBits;
  s->w_size = 1 << s->w_bits;
  s->w_mask = s->w_size - 1;
  s->hash_bits = memLevel + 7;
  s->hash_size = 1 << s->hash_bits;
  s->hash_mask = s->hash_size - 1;
  s->hash_shift = (s->hash_bits + 2) / 3;
  s->window = strm->zalloc(strm->opaque, s->w_size, 2);
  s->prev = (unsigned __int16 *)strm->zalloc(strm->opaque, s->w_size, 2);
  s->head = (unsigned __int16 *)strm->zalloc(strm->opaque, s->hash_size, 2);
  s->lit_bufsize = 1 << (memLevel + 6);
  overlay = (unsigned __int16 *)strm->zalloc(strm->opaque, s->lit_bufsize, 4);
  s->pending_buf = (unsigned __int8 *)overlay;
  s->pending_buf_size = 4 * s->lit_bufsize;
  if ( s->window && s->prev && s->head && s->pending_buf )
  {
    s->d_buf = &overlay[s->lit_bufsize >> 1];
    s->l_buf = &s->pending_buf[3 * s->lit_bufsize];
    s->level = levela;
    s->strategy = strategy;
    s->method = 8;
    return deflateReset(strm);
  }
  else
  {
    s->status = 666;
    strm->msg = (char *)z_errmsg[6];
    deflateEnd(strm);
    return -4;
  }
}

int __cdecl deflateReset(z_stream_s *strm)
{
  internal_state *s; // [esp+0h] [ebp-4h]

  if ( !strm || !strm->state || !strm->zalloc || !strm->zfree )
    return -2;
  strm->total_out = 0;
  strm->total_in = 0;
  strm->msg = 0;
  strm->data_type = 2;
  s = strm->state;
  s->pending = 0;
  s->pending_out = s->pending_buf;
  if ( s->wrap < 0 )
    s->wrap = -s->wrap;
  s->status = s->wrap != 0 ? 42 : 113;
  strm->adler = adler32(0, 0, 0);
  s->last_flush = 0;
  _tr_init(s);
  lm_init(s);
  return 0;
}

int __cdecl deflate(z_stream_s *strm, unsigned int flush)
{
  block_state bstate; // [esp+0h] [ebp-14h]
  unsigned int header; // [esp+4h] [ebp-10h]
  unsigned int level_flags; // [esp+8h] [ebp-Ch]
  int old_flush; // [esp+Ch] [ebp-8h]
  internal_state *s; // [esp+10h] [ebp-4h]

  if ( !strm || !strm->state || flush > 4 )
    return -2;
  s = strm->state;
  if ( !strm->next_out || !strm->next_in && strm->avail_in || s->status == 666 && flush != 4 )
  {
    strm->msg = (char *)z_errmsg[4];
    return -2;
  }
  if ( !strm->avail_out )
  {
    strm->msg = (char *)z_errmsg[7];
    return -5;
  }
  s->strm = strm;
  old_flush = s->last_flush;
  s->last_flush = flush;
  if ( s->status == 42 )
  {
    if ( s->strategy < 2 && s->level >= 2 )
    {
      if ( s->level >= 6 )
      {
        if ( s->level == 6 )
          level_flags = 2;
        else
          level_flags = 3;
      }
      else
      {
        level_flags = 1;
      }
    }
    else
    {
      level_flags = 0;
    }
    header = ((16 * (s->w_bits - 8) + 8) << 8) | (level_flags << 6);
    if ( s->strstart )
      header |= 0x20u;
    s->status = 113;
    putShortMSB(s, header + 31 - header % 0x1F);
    if ( s->strstart )
    {
      putShortMSB(s, HIWORD(strm->adler));
      putShortMSB(s, strm->adler);
    }
    strm->adler = adler32(0, 0, 0);
  }
  if ( s->pending )
  {
    flush_pending(strm);
    if ( !strm->avail_out )
    {
      s->last_flush = -1;
      return 0;
    }
  }
  else if ( !strm->avail_in && (int)flush <= old_flush && flush != 4 )
  {
    strm->msg = (char *)z_errmsg[7];
    return -5;
  }
  if ( s->status == 666 && strm->avail_in )
  {
    strm->msg = (char *)z_errmsg[7];
    return -5;
  }
  if ( strm->avail_in || s->lookahead || flush && s->status != 666 )
  {
    bstate = configuration_table[s->level].func(s, flush);
    if ( bstate == finish_started || bstate == finish_done )
      s->status = 666;
    if ( bstate == need_more || bstate == finish_started )
    {
      if ( !strm->avail_out )
        s->last_flush = -1;
      return 0;
    }
    if ( bstate == block_done )
    {
      if ( flush == 1 )
      {
        _tr_align(s);
      }
      else
      {
        _tr_stored_block(s, 0, 0, 0);
        if ( flush == 3 )
        {
          s->head[s->hash_size - 1] = 0;
          memset((unsigned __int8 *)s->head, 0, 2 * s->hash_size - 2);
        }
      }
      flush_pending(strm);
      if ( !strm->avail_out )
      {
        s->last_flush = -1;
        return 0;
      }
    }
  }
  if ( flush != 4 )
    return 0;
  if ( s->wrap <= 0 )
    return 1;
  putShortMSB(s, HIWORD(strm->adler));
  putShortMSB(s, strm->adler);
  flush_pending(strm);
  if ( s->wrap > 0 )
    s->wrap = -s->wrap;
  return s->pending == 0;
}

void __cdecl putShortMSB(internal_state *s, __int16 b)
{
  s->pending_buf[s->pending++] = HIBYTE(b);
  s->pending_buf[s->pending++] = b;
}

void __cdecl flush_pending(z_stream_s *strm)
{
  unsigned int len; // [esp+0h] [ebp-4h]

  len = strm->state->pending;
  if ( len > strm->avail_out )
    len = strm->avail_out;
  if ( len )
  {
    memcpy(strm->next_out, strm->state->pending_out, len);
    strm->next_out += len;
    strm->state->pending_out += len;
    strm->total_out += len;
    strm->avail_out -= len;
    strm->state->pending -= len;
    if ( !strm->state->pending )
      strm->state->pending_out = strm->state->pending_buf;
  }
}

unsigned int __cdecl deflateEnd(z_stream_s *strm)
{
  int status; // [esp+0h] [ebp-4h]

  if ( !strm || !strm->state )
    return -2;
  status = strm->state->status;
  if ( status != 42 && status != 69 && status != 73 && status != 91 && status != 103 && status != 113 && status != 666 )
    return -2;
  if ( strm->state->pending_buf )
    strm->zfree(strm->opaque, strm->state->pending_buf);
  if ( strm->state->head )
    strm->zfree(strm->opaque, (unsigned __int8 *)strm->state->head);
  if ( strm->state->prev )
    strm->zfree(strm->opaque, (unsigned __int8 *)strm->state->prev);
  if ( strm->state->window )
    strm->zfree(strm->opaque, strm->state->window);
  strm->zfree(strm->opaque, (unsigned __int8 *)strm->state);
  strm->state = 0;
  return status != 113 ? 0 : 0xFFFFFFFD;
}

void __cdecl lm_init(internal_state *s)
{
  s->window_size = 2 * s->w_size;
  s->head[s->hash_size - 1] = 0;
  memset((unsigned __int8 *)s->head, 0, 2 * s->hash_size - 2);
  s->max_lazy_match = configuration_table[s->level].max_lazy;
  s->good_match = configuration_table[s->level].good_length;
  s->nice_match = configuration_table[s->level].nice_length;
  s->max_chain_length = configuration_table[s->level].max_chain;
  s->strstart = 0;
  s->block_start = 0;
  s->lookahead = 0;
  s->prev_length = 2;
  s->match_length = 2;
  s->match_available = 0;
  s->ins_h = 0;
}

int __cdecl deflate_stored(internal_state *s, int flush)
{
  unsigned int max_block_size; // [esp+Ch] [ebp-8h]
  unsigned int max_start; // [esp+10h] [ebp-4h]

  max_block_size = 0xFFFF;
  if ( s->pending_buf_size - 5 < 0xFFFF )
    max_block_size = s->pending_buf_size - 5;
  while ( 1 )
  {
    if ( s->lookahead <= 1 )
    {
      fill_window(s);
      if ( !s->lookahead && !flush )
        return 0;
      if ( !s->lookahead )
        break;
    }
    s->strstart += s->lookahead;
    s->lookahead = 0;
    max_start = max_block_size + s->block_start;
    if ( !s->strstart || s->strstart >= max_start )
    {
      s->lookahead = s->strstart - max_start;
      s->strstart = max_start;
      if ( s->block_start < 0 )
        _tr_flush_block(s, 0, s->strstart - s->block_start, 0);
      else
        _tr_flush_block(s, (char *)&s->window[s->block_start], s->strstart - s->block_start, 0);
      s->block_start = s->strstart;
      flush_pending(s->strm);
      if ( !s->strm->avail_out )
        return 0;
    }
    if ( s->strstart - s->block_start >= s->w_size - 262 )
    {
      if ( s->block_start < 0 )
        _tr_flush_block(s, 0, s->strstart - s->block_start, 0);
      else
        _tr_flush_block(s, (char *)&s->window[s->block_start], s->strstart - s->block_start, 0);
      s->block_start = s->strstart;
      flush_pending(s->strm);
      if ( !s->strm->avail_out )
        return 0;
    }
  }
  if ( s->block_start < 0 )
    _tr_flush_block(s, 0, s->strstart - s->block_start, flush == 4);
  else
    _tr_flush_block(s, (char *)&s->window[s->block_start], s->strstart - s->block_start, flush == 4);
  s->block_start = s->strstart;
  flush_pending(s->strm);
  if ( s->strm->avail_out )
    return 2 * (flush == 4) + 1;
  else
    return flush != 4 ? 0 : 2;
}

void __cdecl fill_window(internal_state *s)
{
  unsigned __int16 result; // [esp+0h] [ebp-20h]
  unsigned __int16 result_2; // [esp+2h] [ebp-1Eh]
  unsigned __int16 result_4; // [esp+4h] [ebp-1Ch]
  unsigned __int16 result_6; // [esp+6h] [ebp-1Ah]
  unsigned int wsize; // [esp+Ch] [ebp-14h]
  unsigned int more; // [esp+10h] [ebp-10h]
  unsigned int n; // [esp+14h] [ebp-Ch]
  unsigned int count; // [esp+18h] [ebp-8h]
  unsigned __int16 *p; // [esp+1Ch] [ebp-4h]

  wsize = s->w_size;
  do
  {
    more = s->window_size - s->lookahead - s->strstart;
    if ( s->strstart >= wsize + s->w_size - 262 )
    {
      memcpy(s->window, &s->window[wsize], wsize);
      s->match_start -= wsize;
      s->strstart -= wsize;
      s->block_start -= wsize;
      p = s->head;
      if ( s->hash_size % 4
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\zlib\\deflate.cpp",
              1370,
              0,
              "%s",
              "!(s->hash_size % 4)") )
      {
        __debugbreak();
      }
      count = s->hash_size >> 2;
      for ( n = 0; n < count; ++n )
      {
        result = ClampShortNonNegative(*p - wsize);
        result_2 = ClampShortNonNegative(p[1] - wsize);
        result_4 = ClampShortNonNegative(p[2] - wsize);
        result_6 = ClampShortNonNegative(p[3] - wsize);
        *p = result;
        p[1] = result_2;
        p[2] = result_4;
        p[3] = result_6;
        p += 4;
      }
      more += wsize;
    }
    if ( !s->strm->avail_in )
      break;
    s->lookahead += read_buf(s->strm, &s->window[s->strstart + s->lookahead], more);
    if ( s->lookahead >= 3 )
    {
      s->ins_h = s->window[s->strstart];
      s->ins_h = s->hash_mask & (s->window[s->strstart + 1] ^ (s->ins_h << s->hash_shift));
    }
  }
  while ( s->lookahead < 0x106 && s->strm->avail_in );
}

unsigned int __cdecl read_buf(z_stream_s *strm, unsigned __int8 *buf, unsigned int size)
{
  unsigned int len; // [esp+0h] [ebp-4h]

  len = strm->avail_in;
  if ( len > size )
    len = size;
  if ( !len )
    return 0;
  strm->avail_in -= len;
  if ( strm->state->wrap == 1 )
    strm->adler = adler32(strm->adler, strm->next_in, len);
  memcpy(buf, strm->next_in, len);
  strm->next_in += len;
  strm->total_in += len;
  return len;
}

unsigned int __cdecl ClampShortNonNegative(__int16 value)
{
  return value & ~((unsigned int)value >> 16);
}

int __cdecl deflate_fast(internal_state *s, int flush)
{
  unsigned __int8 v3; // [esp+Ah] [ebp-12h]
  unsigned __int8 cc; // [esp+Bh] [ebp-11h]
  unsigned __int16 dista; // [esp+Ch] [ebp-10h]
  unsigned __int16 dist; // [esp+Ch] [ebp-10h]
  unsigned __int8 len; // [esp+13h] [ebp-9h]
  unsigned int hash_head; // [esp+14h] [ebp-8h]
  bool bflush; // [esp+18h] [ebp-4h]

  hash_head = 0;
  while ( 1 )
  {
    if ( s->lookahead < 0x106 )
    {
      fill_window(s);
      if ( s->lookahead < 0x106 && !flush )
        return 0;
      if ( !s->lookahead )
        break;
    }
    if ( s->lookahead >= 3 )
    {
      s->ins_h = s->hash_mask & (s->window[s->strstart + 2] ^ (s->ins_h << s->hash_shift));
      hash_head = s->head[s->ins_h];
      s->head[s->ins_h] = s->strstart;
    }
    if ( hash_head
      && s->strstart - hash_head <= s->w_size - 262
      && (s->strategy != 2 && s->strategy != 3 || s->strategy == 3 && s->strstart - hash_head == 1) )
    {
      s->match_length = longest_match_fast(s, hash_head);
    }
    if ( s->match_length < 3 )
    {
      cc = s->window[s->strstart];
      s->d_buf[s->last_lit] = 0;
      s->l_buf[s->last_lit++] = cc;
      ++s->dyn_ltree[cc].fc.freq;
      bflush = s->last_lit == s->lit_bufsize - 1;
      --s->lookahead;
      ++s->strstart;
    }
    else
    {
      len = s->match_length - 3;
      dista = LOWORD(s->strstart) - LOWORD(s->match_start);
      s->d_buf[s->last_lit] = dista;
      s->l_buf[s->last_lit++] = len;
      dist = dista - 1;
      ++s->dyn_ltree[_length_code[len] + 257].fc.freq;
      if ( dist >= 0x100u )
        v3 = byte_D147B8[(int)dist >> 7];
      else
        v3 = _dist_code[dist];
      ++s->dyn_dtree[v3].fc.freq;
      bflush = s->last_lit == s->lit_bufsize - 1;
      s->lookahead -= s->match_length;
      s->strstart += s->match_length;
      s->match_length = 0;
      s->ins_h = s->window[s->strstart];
      s->ins_h = s->hash_mask & (s->window[s->strstart + 1] ^ (s->ins_h << s->hash_shift));
    }
    if ( bflush )
    {
      if ( s->block_start < 0 )
        _tr_flush_block(s, 0, s->strstart - s->block_start, 0);
      else
        _tr_flush_block(s, (char *)&s->window[s->block_start], s->strstart - s->block_start, 0);
      s->block_start = s->strstart;
      flush_pending(s->strm);
      if ( !s->strm->avail_out )
        return 0;
    }
  }
  if ( s->block_start < 0 )
    _tr_flush_block(s, 0, s->strstart - s->block_start, flush == 4);
  else
    _tr_flush_block(s, (char *)&s->window[s->block_start], s->strstart - s->block_start, flush == 4);
  s->block_start = s->strstart;
  flush_pending(s->strm);
  if ( s->strm->avail_out )
    return 2 * (flush == 4) + 1;
  else
    return flush != 4 ? 0 : 2;
}

unsigned int __cdecl longest_match_fast(internal_state *s, unsigned int cur_match)
{
  int v3; // ecx
  int v4; // eax
  int v5; // ecx
  int v6; // eax
  int v7; // ecx
  int v8; // eax
  int v9; // ecx
  int v10; // eax
  int v11; // ecx
  int v12; // eax
  int v13; // ecx
  int v14; // eax
  int v15; // ecx
  int v16; // eax
  unsigned __int8 *match; // [esp+24h] [ebp-10h]
  unsigned __int8 *matcha; // [esp+24h] [ebp-10h]
  unsigned __int8 *matchb; // [esp+24h] [ebp-10h]
  unsigned __int8 *matchc; // [esp+24h] [ebp-10h]
  unsigned __int8 *matchd; // [esp+24h] [ebp-10h]
  unsigned __int8 *matche; // [esp+24h] [ebp-10h]
  unsigned __int8 *matchf; // [esp+24h] [ebp-10h]
  unsigned __int8 *matchg; // [esp+24h] [ebp-10h]
  unsigned __int8 *matchh; // [esp+24h] [ebp-10h]
  unsigned __int8 *strend; // [esp+28h] [ebp-Ch]
  unsigned __int8 *scan; // [esp+2Ch] [ebp-8h]
  unsigned __int8 *scana; // [esp+2Ch] [ebp-8h]
  signed int len; // [esp+30h] [ebp-4h]

  scan = &s->window[s->strstart];
  strend = scan + 258;
  match = &s->window[cur_match];
  if ( *match != *scan || match[1] != scan[1] )
    return 2;
  scana = scan + 3;
  matcha = match + 3;
  while ( *(unsigned int *)scana == *(unsigned int *)matcha )
  {
    if ( *((unsigned int *)scana + 1) != *((unsigned int *)matcha + 1) )
      break;
    scana += 8;
    matcha += 8;
    if ( *(unsigned int *)scana != *(unsigned int *)matcha )
      break;
    if ( *((unsigned int *)scana + 1) != *((unsigned int *)matcha + 1) )
      break;
    scana += 8;
    matcha += 8;
    if ( *(unsigned int *)scana != *(unsigned int *)matcha )
      break;
    if ( *((unsigned int *)scana + 1) != *((unsigned int *)matcha + 1) )
      break;
    scana += 8;
    matcha += 8;
    if ( *(unsigned int *)scana != *(unsigned int *)matcha || *((unsigned int *)scana + 1) != *((unsigned int *)matcha + 1) )
      break;
    scana += 8;
    matcha += 8;
    if ( scana >= strend )
      goto full_match;
  }
  v3 = *matcha;
  v4 = *scana;
  matchb = matcha + 1;
  ++scana;
  if ( v4 == v3 )
  {
    v5 = *matchb;
    v6 = *scana;
    matchc = matchb + 1;
    ++scana;
    if ( v6 == v5 )
    {
      v7 = *matchc;
      v8 = *scana;
      matchd = matchc + 1;
      ++scana;
      if ( v8 == v7 )
      {
        v9 = *matchd;
        v10 = *scana;
        matche = matchd + 1;
        ++scana;
        if ( v10 == v9 )
        {
          v11 = *matche;
          v12 = *scana;
          matchf = matche + 1;
          ++scana;
          if ( v12 == v11 )
          {
            v13 = *matchf;
            v14 = *scana;
            matchg = matchf + 1;
            ++scana;
            if ( v14 == v13 )
            {
              v15 = *matchg;
              v16 = *scana;
              matchh = matchg + 1;
              ++scana;
              if ( v16 == v15 )
              {
                if ( *scana == *matchh
                  && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\zlib\\deflate.cpp",
                        1254,
                        0,
                        "%s",
                        "*scan != *match") )
                {
                  __debugbreak();
                }
                ++scana;
              }
            }
          }
        }
      }
    }
  }
full_match:
  len = 258 - (strend - (scana - 1));
  if ( len < 3 )
    return 2;
  s->match_start = cur_match;
  if ( len > s->lookahead )
    return s->lookahead;
  else
    return 258 - (strend - (scana - 1));
}

