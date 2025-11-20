#include "vpx.h"

void __cdecl vpx_init(const char *filename, int width, int height)
{
  if ( !vpx_globals.init )
  {
    vpx_globals.width = width;
    vpx_globals.height = height;
    vpx_globals.frame_cnt = 0;
    vpx_globals.flags = 0;
    if ( vpx_img_alloc((int *)&vpx_globals.raw, 258, width, height, 1) )
    {
      vpx_globals.outfile = fopen(filename, "wb");
      if ( vpx_globals.outfile )
      {
        if ( !vpx_codec_enc_config_default((int)&vpx_codec_vp8_cx_algo, &vpx_globals.cfg.g_usage, 0) )
        {
          vpx_globals.cfg.rc_target_bitrate = vpx_globals.cfg.rc_target_bitrate
                                            * vpx_globals.height
                                            * vpx_globals.width
                                            / vpx_globals.cfg.g_w
                                            / vpx_globals.cfg.g_h;
          vpx_globals.cfg.g_w = width;
          vpx_globals.cfg.g_h = height;
          write_ivf_file_header(vpx_globals.outfile, &vpx_globals.cfg, 0);
          if ( !vpx_codec_enc_init_ver(
                  &vpx_globals.codec.name,
                  (int)&vpx_codec_vp8_cx_algo,
                  (int)&vpx_globals.cfg,
                  0,
                  5) )
            vpx_globals.init = 1;
        }
      }
    }
  }
}

void __cdecl write_ivf_file_header(_iobuf *outfile, const vpx_codec_enc_cfg *cfg, unsigned int frame_cnt)
{
  char header[32]; // [esp+0h] [ebp-24h] BYREF

  if ( cfg->g_pass == VPX_RC_ONE_PASS || cfg->g_pass == VPX_RC_LAST_PASS )
  {
    memcpy(header, "DKIF", 4);
    mem_put_le16(&header[4], 0);
    mem_put_le16(&header[6], 32);
    mem_put_le32(&header[8], 0x30385056u);
    mem_put_le16(&header[12], cfg->g_w);
    mem_put_le16(&header[14], cfg->g_h);
    mem_put_le32(&header[16], cfg->g_timebase.den);
    mem_put_le32(&header[20], cfg->g_timebase.num);
    mem_put_le32(&header[24], frame_cnt);
    mem_put_le32(&header[28], 0);
    fwrite(header, 1u, 0x20u, outfile);
  }
}

void __cdecl mem_put_le16(char *mem, __int16 val)
{
  *(_WORD *)mem = val;
}

void __cdecl mem_put_le32(char *mem, unsigned int val)
{
  *(unsigned int *)mem = val;
}

void __cdecl vpx_encode_frame(unsigned __int8 *y, unsigned __int8 *u, unsigned __int8 *v, bool eof)
{
  const void *iter; // [esp+0h] [ebp-Ch] BYREF
  const vpx_codec_cx_pkt *pkt; // [esp+4h] [ebp-8h]
  int got_data; // [esp+8h] [ebp-4h]

  do
  {
    iter = 0;
    if ( !eof )
    {
      memcpy(vpx_globals.raw.planes[0], y, vpx_globals.height * vpx_globals.width);
      memcpy(vpx_globals.raw.planes[1], u, vpx_globals.height * vpx_globals.width / 4);
      memcpy(vpx_globals.raw.planes[2], v, vpx_globals.height * vpx_globals.width / 4);
    }
    if ( vpx_codec_encode(
           &vpx_globals.codec.name,
           !eof ? (unsigned int)&vpx_globals.raw : 0,
           vpx_globals.frame_cnt,
           vpx_globals.frame_cnt >> 31,
           1,
           vpx_globals.flags,
           1) )
    {
      break;
    }
    got_data = 0;
    while ( 1 )
    {
      pkt = (const vpx_codec_cx_pkt *)vpx_codec_get_cx_data(&vpx_globals.codec.name, (int)&iter);
      if ( !pkt )
        break;
      got_data = 1;
      if ( pkt->kind == VPX_CODEC_CX_FRAME_PKT )
      {
        write_ivf_frame_header(vpx_globals.outfile, pkt);
        fwrite(pkt->data.frame.buf, 1u, pkt->data.frame.sz, vpx_globals.outfile);
      }
    }
    ++vpx_globals.frame_cnt;
    if ( !got_data )
      break;
  }
  while ( eof );
}

void __cdecl write_ivf_frame_header(_iobuf *outfile, const vpx_codec_cx_pkt *pkt)
{
  char header[12]; // [esp+0h] [ebp-18h] BYREF
  __int64 pts; // [esp+10h] [ebp-8h]

  if ( pkt->kind == VPX_CODEC_CX_FRAME_PKT )
  {
    pts = pkt->data.frame.pts;
    mem_put_le32(header, pkt->data.frame.sz);
    mem_put_le32(&header[4], pts);
    mem_put_le32(&header[8], HIDWORD(pts));
    fwrite(header, 1u, 0xCu, outfile);
  }
}

void __cdecl vpx_shutdown()
{
  vpx_encode_frame(0, 0, 0, 1);
  if ( !vpx_codec_destroy(&vpx_globals.codec.name) )
  {
    if ( !fseek(vpx_globals.outfile, 0, 0) )
      write_ivf_file_header(vpx_globals.outfile, &vpx_globals.cfg, vpx_globals.frame_cnt - 1);
    fclose(vpx_globals.outfile);
    vpx_img_free((void **)&vpx_globals.raw);
    vpx_globals.init = 0;
  }
}

