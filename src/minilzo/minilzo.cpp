#include "minilzo.h"

int __cdecl _lzo_config_check()
{
  return 0;
}

int __cdecl __lzo_init_v2(unsigned int v, int s1, int s2, int s3, int s4, int s5, int s6, int s7, int s8, int s9)
{
  int r; // [esp+4h] [ebp-4h]

  if ( !v )
    return -1;
  if ( s1 != -1 && s1 != 2
    || s2 != -1 && s2 != 4
    || s3 != -1 && s3 != 4
    || s4 != -1 && s4 != 4
    || s5 != -1 && s5 != 4
    || s6 != -1 && s6 != 4
    || s7 != -1 && s7 != 4
    || s8 != -1 && s8 != 4
    || s9 != -1 && s9 != 24 )
  {
    return -1;
  }
  r = _lzo_config_check();
  if ( r )
    return r;
  else
    return 0;
}

int __cdecl __lzopro_lzo_init_v2(
        unsigned int v,
        int s1,
        int s2,
        int s3,
        int s4,
        int s5,
        int s6,
        int s7,
        int s8,
        int s9)
{
  return __lzo_init_v2(v, s1, s2, s3, s4, s5, s6, s7, s8, s9);
}

int __cdecl lzopro_lzo1x_1_11_compress(
        const unsigned __int8 *in,
        unsigned int in_len,
        unsigned __int8 *out,
        unsigned int *out_len,
        _WORD *wrkmem)
{
  unsigned int tt; // [esp+0h] [ebp-10h]
  const unsigned __int8 *ii; // [esp+4h] [ebp-Ch]
  unsigned __int8 *op; // [esp+8h] [ebp-8h]
  unsigned __int8 *opa; // [esp+8h] [ebp-8h]
  unsigned __int8 *opb; // [esp+8h] [ebp-8h]
  unsigned int t; // [esp+Ch] [ebp-4h]

  op = out;
  if ( in_len > 0xD )
  {
    t = lzopro_lzo1x_1_11_compress_core(in, in_len, out, out_len, wrkmem);
    op = &out[*out_len];
  }
  else
  {
    t = in_len;
  }
  if ( t )
  {
    ii = &in[in_len - t];
    if ( op == out && t <= 0xEE )
    {
      *op++ = t + 17;
    }
    else if ( t > 3 )
    {
      if ( t > 0x12 )
      {
        tt = t - 18;
        *op = 0;
        opa = op + 1;
        while ( tt > 0xFF )
        {
          tt -= 255;
          *opa++ = 0;
        }
        *opa = tt;
        op = opa + 1;
      }
      else
      {
        *op++ = t - 3;
      }
    }
    else
    {
      *(op - 2) |= t;
    }
    do
    {
      *op++ = *ii++;
      --t;
    }
    while ( t );
  }
  *op = 17;
  opb = op + 1;
  *opb++ = 0;
  *opb = 0;
  *out_len = opb + 1 - out;
  return 0;
}

int __cdecl lzopro_lzo1x_1_11_compress_core(
        const unsigned __int8 *in,
        unsigned int in_len,
        unsigned __int8 *out,
        unsigned int *out_len,
        _WORD *wrkmem)
{
  int v5; // ecx
  int v6; // ecx
  int v7; // ecx
  int v8; // ecx
  int v9; // ecx
  int v10; // ecx
  const unsigned __int8 *m; // [esp+0h] [ebp-38h]
  unsigned int v13; // [esp+8h] [ebp-30h]
  unsigned int v14; // [esp+8h] [ebp-30h]
  unsigned int dindex; // [esp+Ch] [ebp-2Ch]
  unsigned int dindexa; // [esp+Ch] [ebp-2Ch]
  char m_len; // [esp+10h] [ebp-28h]
  unsigned int m_lena; // [esp+10h] [ebp-28h]
  unsigned int m_lenb; // [esp+10h] [ebp-28h]
  const unsigned __int8 *m_pos; // [esp+14h] [ebp-24h]
  unsigned int m_off; // [esp+18h] [ebp-20h]
  unsigned int m_offb; // [esp+18h] [ebp-20h]
  unsigned int m_offa; // [esp+18h] [ebp-20h]
  const unsigned __int8 *in_end; // [esp+20h] [ebp-18h]
  unsigned __int8 *op; // [esp+24h] [ebp-14h]
  unsigned __int8 *opa; // [esp+24h] [ebp-14h]
  unsigned __int8 *opb; // [esp+24h] [ebp-14h]
  unsigned __int8 *opc; // [esp+24h] [ebp-14h]
  const unsigned __int8 *v29; // [esp+28h] [ebp-10h]
  const unsigned __int8 *v30; // [esp+28h] [ebp-10h]
  const unsigned __int8 *ii; // [esp+2Ch] [ebp-Ch]
  const unsigned __int8 *ip_end; // [esp+34h] [ebp-4h]

  in_end = &in[in_len];
  ip_end = &in[in_len - 13];
  op = out;
  ii = in;
  v29 = in + 3;
literal:
  if ( (unsigned int)(v29 - ii) >= 0x12 )
  {
    if ( (unsigned int)(v29 - ii) >= 0x1F )
      ++v29;
    ++v29;
  }
  if ( ++v29 < ip_end )
  {
    while ( 1 )
    {
      dindex = ((33 * ((v29[2] << 8) ^ (8 * v29[1]) ^ (unsigned int)*v29)) >> 5) & 0xFFF;
      m_off = (unsigned __int16)((_WORD)v29 - wrkmem[dindex]);
      wrkmem[dindex] = (_WORD)v29;
      if ( !m_off )
        goto literal;
      if ( m_off > 0xBFFF )
        goto literal;
      if ( m_off > v29 - in )
        goto literal;
      m_pos = &v29[-m_off];
      if ( m_off > 0x800 && m_pos[3] != v29[3] )
      {
        dindexa = dindex ^ 0xFFF;
        m_off = (unsigned __int16)((_WORD)v29 - wrkmem[dindexa]);
        wrkmem[dindexa] = (_WORD)v29;
        if ( !m_off )
          goto literal;
        if ( m_off > 0xBFFF )
          goto literal;
        if ( m_off > v29 - in )
          goto literal;
        m_pos = &v29[-m_off];
        if ( m_off > 0x800 && m_pos[3] != v29[3] )
          goto literal;
      }
      if ( *(unsigned __int16 *)m_pos != *(unsigned __int16 *)v29 || m_pos[2] != v29[2] )
        goto literal;
      v13 = v29 - ii;
      if ( v29 != ii )
      {
        if ( v13 > 3 )
        {
          if ( v13 > 0x12 )
          {
            v14 = v13 + 237;
            do
            {
              *op++ = 0;
              v14 -= 255;
            }
            while ( v14 > 0xFF );
            *op = v14;
            opa = op + 1;
          }
          else
          {
            *op = v13 - 3;
            opa = op + 1;
          }
          do
          {
            *(unsigned int *)opa = *(unsigned int *)ii;
            opa += 4;
            ii += 4;
          }
          while ( ii < v29 );
          op = &opa[-(ii - v29)];
          ii = v29;
        }
        else
        {
          *(op - 2) |= v13;
          *(unsigned int *)op = *(unsigned int *)ii;
          op += v13;
          ii = v29;
        }
      }
      v30 = v29 + 3;
      v5 = *v30;
      v29 = v30 + 1;
      if ( m_pos[3] == v5 )
      {
        v6 = *v29++;
        if ( m_pos[4] == v6 )
        {
          v7 = *v29++;
          if ( m_pos[5] == v7 )
          {
            v8 = *v29++;
            if ( m_pos[6] == v8 )
            {
              v9 = *v29++;
              if ( m_pos[7] == v9 )
              {
                v10 = *v29++;
                if ( m_pos[8] == v10 )
                  break;
              }
            }
          }
        }
      }
      m_len = (_BYTE)--v29 - (_BYTE)ii;
      if ( m_off > 0x800 )
      {
        if ( m_off > 0x4000 )
        {
          m_offa = m_off - 0x4000;
          *op = (m_len - 2) | ((unsigned __int16)(m_offa & 0x4000) >> 11) | 0x10;
        }
        else
        {
          m_offa = m_off - 1;
          *op = (m_len - 2) | 0x20;
        }
        opb = op + 1;
        goto m3_m4_offset;
      }
      m_offb = m_off - 1;
      *op = (4 * (m_offb & 7)) | (32 * (m_len - 1));
      op[1] = m_offb >> 3;
      op += 2;
LABEL_64:
      ii = v29;
      if ( v29 >= ip_end )
        goto done_2;
    }
    for ( m = m_pos + 9; v29 < in_end && *m == *v29; ++m )
      ++v29;
    m_lena = v29 - ii;
    if ( m_off > 0x4000 )
    {
      m_offa = m_off - 0x4000;
      if ( m_lena <= 9 )
      {
        *op = (m_lena - 2) | ((unsigned __int16)(m_offa & 0x4000) >> 11) | 0x10;
        opb = op + 1;
        goto m3_m4_offset;
      }
      m_lenb = m_lena - 9;
      *op = ((unsigned __int16)(m_offa & 0x4000) >> 11) | 0x10;
      opc = op + 1;
    }
    else
    {
      m_offa = m_off - 1;
      if ( m_lena <= 0x21 )
      {
        *op = (m_lena - 2) | 0x20;
        opb = op + 1;
m3_m4_offset:
        *opb = 4 * (m_offa & 0x3F);
        opb[1] = m_offa >> 6;
        op = opb + 2;
        goto LABEL_64;
      }
      m_lenb = m_lena - 33;
      *op = 32;
      opc = op + 1;
    }
    while ( m_lenb > 0xFF )
    {
      m_lenb -= 255;
      *opc++ = 0;
    }
    *opc = m_lenb;
    opb = opc + 1;
    goto m3_m4_offset;
  }
done_2:
  *out_len = op - out;
  return in_end - ii;
}

int __cdecl lzopro_lzo1x_decompress(
        const unsigned __int8 *in,
        unsigned int in_len,
        unsigned __int8 *out,
        unsigned int *out_len)
{
  int v5; // ecx
  int v6; // ecx
  int v7; // ecx
  int v8; // ecx
  int v12; // ecx
  int v13; // ecx
  int v14; // ecx
  int v15; // ecx
  int v16; // ecx
  int v17; // ecx
  int v18; // ecx
  int v19; // ecx
  int v20; // ecx
  int v21; // ecx
  int v22; // ecx
  int v23; // ecx
  int v24; // ecx
  int v25; // ecx
  int v26; // ecx
  int v27; // ecx
  int v28; // ecx
  int v29; // ecx
  int v30; // ecx
  int v31; // ecx
  int v32; // ecx
  int v33; // ecx
  int v34; // ecx
  int v35; // ecx
  int v36; // ecx
  int v37; // ecx
  int v38; // ecx
  int v39; // ecx
  int v40; // ecx
  int v41; // ecx
  int v42; // ecx
  int v43; // ecx
  int v44; // ecx
  int v45; // ecx
  int v46; // ecx
  int v47; // ecx
  int v48; // ecx
  int v49; // ecx
  int v50; // ecx
  int v51; // ecx
  int v52; // ecx
  int v53; // ecx
  int v54; // ecx
  int v55; // ecx
  int v56; // ecx
  int v57; // ecx
  int v58; // ecx
  int v59; // ecx
  int v60; // edx
  int v61; // edx
  int v62; // edx
  int v63; // edx
  int v64; // edx
  int v65; // edx
  int v66; // edx
  int v67; // edx
  int v68; // edx
  int v69; // edx
  int v70; // edx
  int v71; // edx
  int v72; // eax
  int v73; // eax
  int v74; // eax
  int v75; // eax
  int v76; // eax
  int v77; // eax
  int v78; // eax
  int v79; // eax
  int v80; // eax
  int v81; // eax
  int v82; // eax
  int v83; // eax
  int v84; // ecx
  int v85; // ecx
  int v86; // ecx
  int v87; // ecx
  int v88; // ecx
  int v89; // ecx
  int v90; // ecx
  int v91; // ecx
  int v92; // ecx
  int v93; // ecx
  int v94; // ecx
  int v95; // ecx
  int v96; // edx
  int v97; // edx
  int v98; // edx
  int v99; // edx
  int v100; // edx
  int v101; // edx
  int v102; // edx
  int v103; // edx
  int v104; // edx
  int v105; // edx
  int v106; // edx
  int v107; // edx
  int v110; // [esp+8h] [ebp-1Ch]
  unsigned int v111; // [esp+8h] [ebp-1Ch]
  int v112; // [esp+8h] [ebp-1Ch]
  unsigned int v113; // [esp+8h] [ebp-1Ch]
  int v114; // [esp+8h] [ebp-1Ch]
  unsigned int v115; // [esp+8h] [ebp-1Ch]
  int v116; // [esp+8h] [ebp-1Ch]
  unsigned int v117; // [esp+8h] [ebp-1Ch]
  int v118; // [esp+8h] [ebp-1Ch]
  const unsigned __int8 *m_posea; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poseb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posec; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posed; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posee; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posef; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poseg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poseh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posei; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posej; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posek; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posel; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posem; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posen; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poseo; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posep; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_pos; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posa; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poseq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poser; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poses; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poset; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poseu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posev; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posew; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posex; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posey; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posez; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfa; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_pose; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfe; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posff; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfi; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfo; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posi; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poso; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poss; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_post; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posba; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbe; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbi; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbo; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfs; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posft; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posfz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posga; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posge; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgi; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgo; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgs; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgt; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbs; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbt; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posby; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posbz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posca; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posgz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posha; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshe; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshi; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posce; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posch; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posci; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posck; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posco; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posho; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshs; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posht; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poshz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posia; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posib; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posic; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posid; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posie; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posif; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posig; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posih; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscs; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posct; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poscz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posda; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posii; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posij; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posik; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posil; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posim; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posin; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posio; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posip; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posiq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posir; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posis; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posit; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posiu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posiv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posiw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posix; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posiy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posiz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posja; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posde; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdi; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posje; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posji; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjo; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjs; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjt; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posju; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdo; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdp; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdq; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdr; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posds; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdt; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdu; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdv; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posdz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjw; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjx; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjy; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posjz; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poska; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskb; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskc; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskd; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poske; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskf; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskg; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskh; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poski; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskj; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskk; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskl; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskm; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskn; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_posko; // [esp+Ch] [ebp-18h]
  const unsigned __int8 *m_poskp; // [esp+Ch] [ebp-18h]
  unsigned int t; // [esp+10h] [ebp-14h]
  unsigned int ta; // [esp+10h] [ebp-14h]
  unsigned int state; // [esp+14h] [ebp-10h]
  unsigned __int8 *op; // [esp+18h] [ebp-Ch]
  unsigned __int8 *opa; // [esp+18h] [ebp-Ch]
  unsigned __int8 *opb; // [esp+18h] [ebp-Ch]
  unsigned __int8 *opc; // [esp+18h] [ebp-Ch]
  const unsigned __int8 *v403; // [esp+1Ch] [ebp-8h]
  const unsigned __int8 *v404; // [esp+1Ch] [ebp-8h]
  const unsigned __int8 *v405; // [esp+1Ch] [ebp-8h]
  const unsigned __int8 *v406; // [esp+1Ch] [ebp-8h]
  const unsigned __int8 *ip_end; // [esp+20h] [ebp-4h]

  ip_end = &in[in_len];
  *out_len = 0;
  op = out;
  v403 = in;
  state = 256;
  t = *in;
  if ( t > 0x11 )
  {
    v403 = in + 1;
    ta = t - 17;
    for ( state = ta < 4 ? 0 : 512; ta >= 4; ta -= 4 )
    {
      *(unsigned int *)op = *(unsigned int *)v403;
      op += 4;
      v403 += 4;
    }
    for ( ; ta; --ta )
      *op++ = *v403++;
  }
  while ( 2 )
  {
    v110 = *v403++;
    switch ( v110 + state )
    {
      case 0u:
        v5 = 4 * *v403;
        m_pos = &op[-v5 - 1];
        ++v403;
        if ( v5 + 1 < 4 )
        {
          *op = *m_pos;
          op[1] = op[-v5];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_pos;
        }
        op += 2;
        goto next_state_0;
      case 1u:
        v6 = 4 * *v403;
        m_posa = &op[-v6 - 1];
        v404 = v403 + 1;
        if ( v6 + 1 < 4 )
        {
          *op = *m_posa;
          op[1] = op[-v6];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posa;
        }
        opa = op + 2;
        goto next_state_1;
      case 2u:
        v7 = 4 * *v403;
        m_posb = &op[-v7 - 1];
        v405 = v403 + 1;
        if ( v7 + 1 < 4 )
        {
          *op = *m_posb;
          op[1] = op[-v7];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posb;
        }
        opb = op + 2;
        goto next_state_2;
      case 3u:
        v8 = 4 * *v403;
        m_posc = &op[-v8 - 1];
        v406 = v403 + 1;
        if ( v8 + 1 < 4 )
        {
          *op = *m_posc;
          op[1] = op[-v8];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posc;
        }
        opc = op + 2;
        goto next_state_3;
      case 4u:
        m_poseq = &op[-4 * *v403++ - 2];
        *(_WORD *)op = *(_WORD *)m_poseq;
        op += 2;
        goto next_state_0;
      case 5u:
        m_poser = &op[-4 * *v403 - 2];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_poser;
        opa = op + 2;
        goto next_state_1;
      case 6u:
        m_poses = &op[-4 * *v403 - 2];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_poses;
        opb = op + 2;
        goto next_state_2;
      case 7u:
        m_poset = &op[-4 * *v403 - 2];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_poset;
        opc = op + 2;
        goto next_state_3;
      case 8u:
        m_poseu = &op[-4 * *v403++ - 3];
        *(_WORD *)op = *(_WORD *)m_poseu;
        op += 2;
        goto next_state_0;
      case 9u:
        m_posev = &op[-4 * *v403 - 3];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posev;
        opa = op + 2;
        goto next_state_1;
      case 0xAu:
        m_posew = &op[-4 * *v403 - 3];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posew;
        opb = op + 2;
        goto next_state_2;
      case 0xBu:
        m_posex = &op[-4 * *v403 - 3];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posex;
        opc = op + 2;
        goto next_state_3;
      case 0xCu:
        m_posey = &op[-4 * *v403++ - 4];
        *(_WORD *)op = *(_WORD *)m_posey;
        op += 2;
        goto next_state_0;
      case 0xDu:
        m_posez = &op[-4 * *v403 - 4];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posez;
        opa = op + 2;
        goto next_state_1;
      case 0xEu:
        m_posfa = &op[-4 * *v403 - 4];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posfa;
        opb = op + 2;
        goto next_state_2;
      case 0xFu:
        m_posfb = &op[-4 * *v403 - 4];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posfb;
        opc = op + 2;
        goto next_state_3;
      case 0x10u:
      case 0x110u:
      case 0x210u:
        v112 = 0;
        do
          v112 += 255;
        while ( !*v403++ );
        v113 = v112 + *(v403 - 1) - 255;
        m_posfc = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x4000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfc;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posfc + 1);
        op[8] = m_posfc[8];
        op += 9;
        for ( m_posd = m_posfc + 9; v113 >= 4; v113 -= 4 )
        {
          *(unsigned int *)op = *(unsigned int *)m_posd;
          op += 4;
          m_posd += 4;
        }
        for ( ; v113; --v113 )
          *op++ = *m_posd++;
        goto next_state;
      case 0x11u:
      case 0x111u:
      case 0x211u:
        m_posf = &op[-((int)*(unsigned __int16 *)v403 >> 2)];
        v403 += 2;
        if ( m_posf != op )
        {
          m_posfe = m_posf - 0x4000;
          *(_WORD *)op = *(_WORD *)m_posfe;
          op[2] = m_posfe[2];
          op += 3;
next_state:
          state = 256;
          v118 = *(v403 - 2) & 3;
          if ( (*(v403 - 2) & 3) != 0 )
          {
            do
            {
              *op++ = *v403++;
              --v118;
            }
            while ( v118 );
            state = 0;
          }
          continue;
        }
        *out_len = op - out;
        if ( v403 == ip_end )
          return 0;
        else
          return v403 < ip_end ? -8 : -4;
      case 0x12u:
      case 0x112u:
      case 0x212u:
        m_posff = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x4000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posff;
        op += 4;
        goto next_state;
      case 0x13u:
      case 0x113u:
      case 0x213u:
        m_posfg = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x4000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfg;
        op[4] = m_posfg[4];
        op += 5;
        goto next_state;
      case 0x14u:
      case 0x114u:
      case 0x214u:
        m_posfh = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x4000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfh;
        *((_WORD *)op + 2) = *((_WORD *)m_posfh + 2);
        op += 6;
        goto next_state;
      case 0x15u:
      case 0x115u:
      case 0x215u:
        m_posfi = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x4000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfi;
        *((_WORD *)op + 2) = *((_WORD *)m_posfi + 2);
        op[6] = m_posfi[6];
        op += 7;
        goto next_state;
      case 0x16u:
      case 0x116u:
      case 0x216u:
        m_posfj = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x4000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfj;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posfj + 1);
        op += 8;
        goto next_state;
      case 0x17u:
      case 0x117u:
      case 0x217u:
        m_posfk = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x4000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfk;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posfk + 1);
        op[8] = m_posfk[8];
        op += 9;
        goto next_state;
      case 0x18u:
      case 0x118u:
      case 0x218u:
        v114 = 0;
        do
          v114 += 255;
        while ( !*v403++ );
        v115 = v114 + *(v403 - 1) - 255;
        m_posfd = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfd;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posfd + 1);
        op[8] = m_posfd[8];
        op += 9;
        for ( m_pose = m_posfd + 9; v115 >= 4; v115 -= 4 )
        {
          *(unsigned int *)op = *(unsigned int *)m_pose;
          op += 4;
          m_pose += 4;
        }
        for ( ; v115; --v115 )
          *op++ = *m_pose++;
        goto next_state;
      case 0x19u:
      case 0x119u:
      case 0x219u:
        m_posfl = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(_WORD *)op = *(_WORD *)m_posfl;
        op[2] = m_posfl[2];
        op += 3;
        goto next_state;
      case 0x1Au:
      case 0x11Au:
      case 0x21Au:
        m_posfm = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfm;
        op += 4;
        goto next_state;
      case 0x1Bu:
      case 0x11Bu:
      case 0x21Bu:
        m_posfn = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfn;
        op[4] = m_posfn[4];
        op += 5;
        goto next_state;
      case 0x1Cu:
      case 0x11Cu:
      case 0x21Cu:
        m_posfo = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfo;
        *((_WORD *)op + 2) = *((_WORD *)m_posfo + 2);
        op += 6;
        goto next_state;
      case 0x1Du:
      case 0x11Du:
      case 0x21Du:
        m_posfp = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfp;
        *((_WORD *)op + 2) = *((_WORD *)m_posfp + 2);
        op[6] = m_posfp[6];
        op += 7;
        goto next_state;
      case 0x1Eu:
      case 0x11Eu:
      case 0x21Eu:
        m_posfq = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfq;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posfq + 1);
        op += 8;
        goto next_state;
      case 0x1Fu:
      case 0x11Fu:
      case 0x21Fu:
        m_posfr = &op[-((int)*(unsigned __int16 *)v403 >> 2) - 0x8000];
        v403 += 2;
        *(unsigned int *)op = *(unsigned int *)m_posfr;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posfr + 1);
        op[8] = m_posfr[8];
        op += 9;
        goto next_state;
      case 0x20u:
      case 0x120u:
      case 0x220u:
        v116 = 0;
        do
          v116 += 255;
        while ( !*v403++ );
        v117 = v116 + *(v403 - 1) - 255;
        v12 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posg = &op[-v12 - 1];
        v403 += 2;
        if ( v12 + 1 < 4 )
        {
          *op = *m_posg;
          op[1] = op[-v12];
          op[2] = m_posg[2];
          op[3] = m_posg[3];
          op[4] = m_posg[4];
          op[5] = m_posg[5];
          op[6] = m_posg[6];
          op[7] = m_posg[7];
          op[8] = m_posg[8];
          op[9] = m_posg[9];
          op[10] = m_posg[10];
          op[11] = m_posg[11];
          op[12] = m_posg[12];
          op[13] = m_posg[13];
          op[14] = m_posg[14];
          op[15] = m_posg[15];
          op[16] = m_posg[16];
          op[17] = m_posg[17];
          op[18] = m_posg[18];
          op[19] = m_posg[19];
          op[20] = m_posg[20];
          op[21] = m_posg[21];
          op[22] = m_posg[22];
          op[23] = m_posg[23];
          op[24] = m_posg[24];
          op[25] = m_posg[25];
          op[26] = m_posg[26];
          op[27] = m_posg[27];
          op[28] = m_posg[28];
          op[29] = m_posg[29];
          op[30] = m_posg[30];
          op[31] = m_posg[31];
          op[32] = m_posg[32];
          op += 33;
          m_posi = m_posg + 33;
          do
          {
            *op++ = *m_posi++;
            --v117;
          }
          while ( v117 );
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posg;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posg + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posg + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posg + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posg + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posg + 5);
          *((unsigned int *)op + 6) = *((unsigned int *)m_posg + 6);
          *((unsigned int *)op + 7) = *((unsigned int *)m_posg + 7);
          op[32] = m_posg[32];
          op += 33;
          for ( m_posh = m_posg + 33; v117 >= 4; v117 -= 4 )
          {
            *(unsigned int *)op = *(unsigned int *)m_posh;
            op += 4;
            m_posh += 4;
          }
          for ( ; v117; --v117 )
            *op++ = *m_posh++;
        }
        goto next_state;
      case 0x21u:
      case 0x121u:
      case 0x221u:
        v13 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posj = &op[-v13 - 1];
        v403 += 2;
        if ( v13 + 1 < 4 )
        {
          *op = *m_posj;
          op[1] = op[-v13];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posj;
        }
        op[2] = m_posj[2];
        op += 3;
        goto next_state;
      case 0x22u:
      case 0x122u:
      case 0x222u:
        v14 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posk = &op[-v14 - 1];
        v403 += 2;
        if ( v14 + 1 < 4 )
        {
          *op = *m_posk;
          op[1] = op[-v14];
          op[2] = m_posk[2];
          op[3] = m_posk[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posk;
        }
        op += 4;
        goto next_state;
      case 0x23u:
      case 0x123u:
      case 0x223u:
        v15 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posl = &op[-v15 - 1];
        v403 += 2;
        if ( v15 + 1 < 4 )
        {
          *op = *m_posl;
          op[1] = op[-v15];
          op[2] = m_posl[2];
          op[3] = m_posl[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posl;
        }
        op[4] = m_posl[4];
        op += 5;
        goto next_state;
      case 0x24u:
      case 0x124u:
      case 0x224u:
        v16 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posm = &op[-v16 - 1];
        v403 += 2;
        if ( v16 + 1 < 4 )
        {
          *op = *m_posm;
          op[1] = op[-v16];
          op[2] = m_posm[2];
          op[3] = m_posm[3];
          op[4] = m_posm[4];
          op[5] = m_posm[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posm;
          *((_WORD *)op + 2) = *((_WORD *)m_posm + 2);
        }
        op += 6;
        goto next_state;
      case 0x25u:
      case 0x125u:
      case 0x225u:
        v17 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posn = &op[-v17 - 1];
        v403 += 2;
        if ( v17 + 1 < 4 )
        {
          *op = *m_posn;
          op[1] = op[-v17];
          op[2] = m_posn[2];
          op[3] = m_posn[3];
          op[4] = m_posn[4];
          op[5] = m_posn[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posn;
          *((_WORD *)op + 2) = *((_WORD *)m_posn + 2);
        }
        op[6] = m_posn[6];
        op += 7;
        goto next_state;
      case 0x26u:
      case 0x126u:
      case 0x226u:
        v18 = (int)*(unsigned __int16 *)v403 >> 2;
        m_poso = &op[-v18 - 1];
        v403 += 2;
        if ( v18 + 1 < 4 )
        {
          *op = *m_poso;
          op[1] = op[-v18];
          op[2] = m_poso[2];
          op[3] = m_poso[3];
          op[4] = m_poso[4];
          op[5] = m_poso[5];
          op[6] = m_poso[6];
          op[7] = m_poso[7];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poso;
          *((unsigned int *)op + 1) = *((unsigned int *)m_poso + 1);
        }
        op += 8;
        goto next_state;
      case 0x27u:
      case 0x127u:
      case 0x227u:
        v19 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posp = &op[-v19 - 1];
        v403 += 2;
        if ( v19 + 1 < 4 )
        {
          *op = *m_posp;
          op[1] = op[-v19];
          op[2] = m_posp[2];
          op[3] = m_posp[3];
          op[4] = m_posp[4];
          op[5] = m_posp[5];
          op[6] = m_posp[6];
          op[7] = m_posp[7];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posp;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posp + 1);
        }
        op[8] = m_posp[8];
        op += 9;
        goto next_state;
      case 0x28u:
      case 0x128u:
      case 0x228u:
        v20 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posq = &op[-v20 - 1];
        v403 += 2;
        if ( v20 + 1 < 4 )
        {
          *op = *m_posq;
          op[1] = op[-v20];
          op[2] = m_posq[2];
          op[3] = m_posq[3];
          op[4] = m_posq[4];
          op[5] = m_posq[5];
          op[6] = m_posq[6];
          op[7] = m_posq[7];
          op[8] = m_posq[8];
          op[9] = m_posq[9];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posq;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posq + 1);
          *((_WORD *)op + 4) = *((_WORD *)m_posq + 4);
        }
        op += 10;
        goto next_state;
      case 0x29u:
      case 0x129u:
      case 0x229u:
        v21 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posr = &op[-v21 - 1];
        v403 += 2;
        if ( v21 + 1 < 4 )
        {
          *op = *m_posr;
          op[1] = op[-v21];
          op[2] = m_posr[2];
          op[3] = m_posr[3];
          op[4] = m_posr[4];
          op[5] = m_posr[5];
          op[6] = m_posr[6];
          op[7] = m_posr[7];
          op[8] = m_posr[8];
          op[9] = m_posr[9];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posr;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posr + 1);
          *((_WORD *)op + 4) = *((_WORD *)m_posr + 4);
        }
        op[10] = m_posr[10];
        op += 11;
        goto next_state;
      case 0x2Au:
      case 0x12Au:
      case 0x22Au:
        v22 = (int)*(unsigned __int16 *)v403 >> 2;
        m_poss = &op[-v22 - 1];
        v403 += 2;
        if ( v22 + 1 < 4 )
        {
          *op = *m_poss;
          op[1] = op[-v22];
          op[2] = m_poss[2];
          op[3] = m_poss[3];
          op[4] = m_poss[4];
          op[5] = m_poss[5];
          op[6] = m_poss[6];
          op[7] = m_poss[7];
          op[8] = m_poss[8];
          op[9] = m_poss[9];
          op[10] = m_poss[10];
          op[11] = m_poss[11];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poss;
          *((unsigned int *)op + 1) = *((unsigned int *)m_poss + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_poss + 2);
        }
        op += 12;
        goto next_state;
      case 0x2Bu:
      case 0x12Bu:
      case 0x22Bu:
        v23 = (int)*(unsigned __int16 *)v403 >> 2;
        m_post = &op[-v23 - 1];
        v403 += 2;
        if ( v23 + 1 < 4 )
        {
          *op = *m_post;
          op[1] = op[-v23];
          op[2] = m_post[2];
          op[3] = m_post[3];
          op[4] = m_post[4];
          op[5] = m_post[5];
          op[6] = m_post[6];
          op[7] = m_post[7];
          op[8] = m_post[8];
          op[9] = m_post[9];
          op[10] = m_post[10];
          op[11] = m_post[11];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_post;
          *((unsigned int *)op + 1) = *((unsigned int *)m_post + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_post + 2);
        }
        op[12] = m_post[12];
        op += 13;
        goto next_state;
      case 0x2Cu:
      case 0x12Cu:
      case 0x22Cu:
        v24 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posu = &op[-v24 - 1];
        v403 += 2;
        if ( v24 + 1 < 4 )
        {
          *op = *m_posu;
          op[1] = op[-v24];
          op[2] = m_posu[2];
          op[3] = m_posu[3];
          op[4] = m_posu[4];
          op[5] = m_posu[5];
          op[6] = m_posu[6];
          op[7] = m_posu[7];
          op[8] = m_posu[8];
          op[9] = m_posu[9];
          op[10] = m_posu[10];
          op[11] = m_posu[11];
          op[12] = m_posu[12];
          op[13] = m_posu[13];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posu;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posu + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posu + 2);
          *((_WORD *)op + 6) = *((_WORD *)m_posu + 6);
        }
        op += 14;
        goto next_state;
      case 0x2Du:
      case 0x12Du:
      case 0x22Du:
        v25 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posv = &op[-v25 - 1];
        v403 += 2;
        if ( v25 + 1 < 4 )
        {
          *op = *m_posv;
          op[1] = op[-v25];
          op[2] = m_posv[2];
          op[3] = m_posv[3];
          op[4] = m_posv[4];
          op[5] = m_posv[5];
          op[6] = m_posv[6];
          op[7] = m_posv[7];
          op[8] = m_posv[8];
          op[9] = m_posv[9];
          op[10] = m_posv[10];
          op[11] = m_posv[11];
          op[12] = m_posv[12];
          op[13] = m_posv[13];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posv;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posv + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posv + 2);
          *((_WORD *)op + 6) = *((_WORD *)m_posv + 6);
        }
        op[14] = m_posv[14];
        op += 15;
        goto next_state;
      case 0x2Eu:
      case 0x12Eu:
      case 0x22Eu:
        v26 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posw = &op[-v26 - 1];
        v403 += 2;
        if ( v26 + 1 < 4 )
        {
          *op = *m_posw;
          op[1] = op[-v26];
          op[2] = m_posw[2];
          op[3] = m_posw[3];
          op[4] = m_posw[4];
          op[5] = m_posw[5];
          op[6] = m_posw[6];
          op[7] = m_posw[7];
          op[8] = m_posw[8];
          op[9] = m_posw[9];
          op[10] = m_posw[10];
          op[11] = m_posw[11];
          op[12] = m_posw[12];
          op[13] = m_posw[13];
          op[14] = m_posw[14];
          op[15] = m_posw[15];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posw;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posw + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posw + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posw + 3);
        }
        op += 16;
        goto next_state;
      case 0x2Fu:
      case 0x12Fu:
      case 0x22Fu:
        v27 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posx = &op[-v27 - 1];
        v403 += 2;
        if ( v27 + 1 < 4 )
        {
          *op = *m_posx;
          op[1] = op[-v27];
          op[2] = m_posx[2];
          op[3] = m_posx[3];
          op[4] = m_posx[4];
          op[5] = m_posx[5];
          op[6] = m_posx[6];
          op[7] = m_posx[7];
          op[8] = m_posx[8];
          op[9] = m_posx[9];
          op[10] = m_posx[10];
          op[11] = m_posx[11];
          op[12] = m_posx[12];
          op[13] = m_posx[13];
          op[14] = m_posx[14];
          op[15] = m_posx[15];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posx;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posx + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posx + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posx + 3);
        }
        op[16] = m_posx[16];
        op += 17;
        goto next_state;
      case 0x30u:
      case 0x130u:
      case 0x230u:
        v28 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posy = &op[-v28 - 1];
        v403 += 2;
        if ( v28 + 1 < 4 )
        {
          *op = *m_posy;
          op[1] = op[-v28];
          op[2] = m_posy[2];
          op[3] = m_posy[3];
          op[4] = m_posy[4];
          op[5] = m_posy[5];
          op[6] = m_posy[6];
          op[7] = m_posy[7];
          op[8] = m_posy[8];
          op[9] = m_posy[9];
          op[10] = m_posy[10];
          op[11] = m_posy[11];
          op[12] = m_posy[12];
          op[13] = m_posy[13];
          op[14] = m_posy[14];
          op[15] = m_posy[15];
          op[16] = m_posy[16];
          op[17] = m_posy[17];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posy;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posy + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posy + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posy + 3);
          *((_WORD *)op + 8) = *((_WORD *)m_posy + 8);
        }
        op += 18;
        goto next_state;
      case 0x31u:
      case 0x131u:
      case 0x231u:
        v29 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posz = &op[-v29 - 1];
        v403 += 2;
        if ( v29 + 1 < 4 )
        {
          *op = *m_posz;
          op[1] = op[-v29];
          op[2] = m_posz[2];
          op[3] = m_posz[3];
          op[4] = m_posz[4];
          op[5] = m_posz[5];
          op[6] = m_posz[6];
          op[7] = m_posz[7];
          op[8] = m_posz[8];
          op[9] = m_posz[9];
          op[10] = m_posz[10];
          op[11] = m_posz[11];
          op[12] = m_posz[12];
          op[13] = m_posz[13];
          op[14] = m_posz[14];
          op[15] = m_posz[15];
          op[16] = m_posz[16];
          op[17] = m_posz[17];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posz;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posz + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posz + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posz + 3);
          *((_WORD *)op + 8) = *((_WORD *)m_posz + 8);
        }
        op[18] = m_posz[18];
        op += 19;
        goto next_state;
      case 0x32u:
      case 0x132u:
      case 0x232u:
        v30 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posba = &op[-v30 - 1];
        v403 += 2;
        if ( v30 + 1 < 4 )
        {
          *op = *m_posba;
          op[1] = op[-v30];
          op[2] = m_posba[2];
          op[3] = m_posba[3];
          op[4] = m_posba[4];
          op[5] = m_posba[5];
          op[6] = m_posba[6];
          op[7] = m_posba[7];
          op[8] = m_posba[8];
          op[9] = m_posba[9];
          op[10] = m_posba[10];
          op[11] = m_posba[11];
          op[12] = m_posba[12];
          op[13] = m_posba[13];
          op[14] = m_posba[14];
          op[15] = m_posba[15];
          op[16] = m_posba[16];
          op[17] = m_posba[17];
          op[18] = m_posba[18];
          op[19] = m_posba[19];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posba;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posba + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posba + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posba + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posba + 4);
        }
        op += 20;
        goto next_state;
      case 0x33u:
      case 0x133u:
      case 0x233u:
        v31 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbb = &op[-v31 - 1];
        v403 += 2;
        if ( v31 + 1 < 4 )
        {
          *op = *m_posbb;
          op[1] = op[-v31];
          op[2] = m_posbb[2];
          op[3] = m_posbb[3];
          op[4] = m_posbb[4];
          op[5] = m_posbb[5];
          op[6] = m_posbb[6];
          op[7] = m_posbb[7];
          op[8] = m_posbb[8];
          op[9] = m_posbb[9];
          op[10] = m_posbb[10];
          op[11] = m_posbb[11];
          op[12] = m_posbb[12];
          op[13] = m_posbb[13];
          op[14] = m_posbb[14];
          op[15] = m_posbb[15];
          op[16] = m_posbb[16];
          op[17] = m_posbb[17];
          op[18] = m_posbb[18];
          op[19] = m_posbb[19];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbb;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbb + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbb + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbb + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbb + 4);
        }
        op[20] = m_posbb[20];
        op += 21;
        goto next_state;
      case 0x34u:
      case 0x134u:
      case 0x234u:
        v32 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbc = &op[-v32 - 1];
        v403 += 2;
        if ( v32 + 1 < 4 )
        {
          *op = *m_posbc;
          op[1] = op[-v32];
          op[2] = m_posbc[2];
          op[3] = m_posbc[3];
          op[4] = m_posbc[4];
          op[5] = m_posbc[5];
          op[6] = m_posbc[6];
          op[7] = m_posbc[7];
          op[8] = m_posbc[8];
          op[9] = m_posbc[9];
          op[10] = m_posbc[10];
          op[11] = m_posbc[11];
          op[12] = m_posbc[12];
          op[13] = m_posbc[13];
          op[14] = m_posbc[14];
          op[15] = m_posbc[15];
          op[16] = m_posbc[16];
          op[17] = m_posbc[17];
          op[18] = m_posbc[18];
          op[19] = m_posbc[19];
          op[20] = m_posbc[20];
          op[21] = m_posbc[21];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbc;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbc + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbc + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbc + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbc + 4);
          *((_WORD *)op + 10) = *((_WORD *)m_posbc + 10);
        }
        op += 22;
        goto next_state;
      case 0x35u:
      case 0x135u:
      case 0x235u:
        v33 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbd = &op[-v33 - 1];
        v403 += 2;
        if ( v33 + 1 < 4 )
        {
          *op = *m_posbd;
          op[1] = op[-v33];
          op[2] = m_posbd[2];
          op[3] = m_posbd[3];
          op[4] = m_posbd[4];
          op[5] = m_posbd[5];
          op[6] = m_posbd[6];
          op[7] = m_posbd[7];
          op[8] = m_posbd[8];
          op[9] = m_posbd[9];
          op[10] = m_posbd[10];
          op[11] = m_posbd[11];
          op[12] = m_posbd[12];
          op[13] = m_posbd[13];
          op[14] = m_posbd[14];
          op[15] = m_posbd[15];
          op[16] = m_posbd[16];
          op[17] = m_posbd[17];
          op[18] = m_posbd[18];
          op[19] = m_posbd[19];
          op[20] = m_posbd[20];
          op[21] = m_posbd[21];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbd;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbd + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbd + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbd + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbd + 4);
          *((_WORD *)op + 10) = *((_WORD *)m_posbd + 10);
        }
        op[22] = m_posbd[22];
        op += 23;
        goto next_state;
      case 0x36u:
      case 0x136u:
      case 0x236u:
        v34 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbe = &op[-v34 - 1];
        v403 += 2;
        if ( v34 + 1 < 4 )
        {
          *op = *m_posbe;
          op[1] = op[-v34];
          op[2] = m_posbe[2];
          op[3] = m_posbe[3];
          op[4] = m_posbe[4];
          op[5] = m_posbe[5];
          op[6] = m_posbe[6];
          op[7] = m_posbe[7];
          op[8] = m_posbe[8];
          op[9] = m_posbe[9];
          op[10] = m_posbe[10];
          op[11] = m_posbe[11];
          op[12] = m_posbe[12];
          op[13] = m_posbe[13];
          op[14] = m_posbe[14];
          op[15] = m_posbe[15];
          op[16] = m_posbe[16];
          op[17] = m_posbe[17];
          op[18] = m_posbe[18];
          op[19] = m_posbe[19];
          op[20] = m_posbe[20];
          op[21] = m_posbe[21];
          op[22] = m_posbe[22];
          op[23] = m_posbe[23];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbe;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbe + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbe + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbe + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbe + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbe + 5);
        }
        op += 24;
        goto next_state;
      case 0x37u:
      case 0x137u:
      case 0x237u:
        v35 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbf = &op[-v35 - 1];
        v403 += 2;
        if ( v35 + 1 < 4 )
        {
          *op = *m_posbf;
          op[1] = op[-v35];
          op[2] = m_posbf[2];
          op[3] = m_posbf[3];
          op[4] = m_posbf[4];
          op[5] = m_posbf[5];
          op[6] = m_posbf[6];
          op[7] = m_posbf[7];
          op[8] = m_posbf[8];
          op[9] = m_posbf[9];
          op[10] = m_posbf[10];
          op[11] = m_posbf[11];
          op[12] = m_posbf[12];
          op[13] = m_posbf[13];
          op[14] = m_posbf[14];
          op[15] = m_posbf[15];
          op[16] = m_posbf[16];
          op[17] = m_posbf[17];
          op[18] = m_posbf[18];
          op[19] = m_posbf[19];
          op[20] = m_posbf[20];
          op[21] = m_posbf[21];
          op[22] = m_posbf[22];
          op[23] = m_posbf[23];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbf;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbf + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbf + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbf + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbf + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbf + 5);
        }
        op[24] = m_posbf[24];
        op += 25;
        goto next_state;
      case 0x38u:
      case 0x138u:
      case 0x238u:
        v36 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbg = &op[-v36 - 1];
        v403 += 2;
        if ( v36 + 1 < 4 )
        {
          *op = *m_posbg;
          op[1] = op[-v36];
          op[2] = m_posbg[2];
          op[3] = m_posbg[3];
          op[4] = m_posbg[4];
          op[5] = m_posbg[5];
          op[6] = m_posbg[6];
          op[7] = m_posbg[7];
          op[8] = m_posbg[8];
          op[9] = m_posbg[9];
          op[10] = m_posbg[10];
          op[11] = m_posbg[11];
          op[12] = m_posbg[12];
          op[13] = m_posbg[13];
          op[14] = m_posbg[14];
          op[15] = m_posbg[15];
          op[16] = m_posbg[16];
          op[17] = m_posbg[17];
          op[18] = m_posbg[18];
          op[19] = m_posbg[19];
          op[20] = m_posbg[20];
          op[21] = m_posbg[21];
          op[22] = m_posbg[22];
          op[23] = m_posbg[23];
          op[24] = m_posbg[24];
          op[25] = m_posbg[25];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbg;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbg + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbg + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbg + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbg + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbg + 5);
          *((_WORD *)op + 12) = *((_WORD *)m_posbg + 12);
        }
        op += 26;
        goto next_state;
      case 0x39u:
      case 0x139u:
      case 0x239u:
        v37 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbh = &op[-v37 - 1];
        v403 += 2;
        if ( v37 + 1 < 4 )
        {
          *op = *m_posbh;
          op[1] = op[-v37];
          op[2] = m_posbh[2];
          op[3] = m_posbh[3];
          op[4] = m_posbh[4];
          op[5] = m_posbh[5];
          op[6] = m_posbh[6];
          op[7] = m_posbh[7];
          op[8] = m_posbh[8];
          op[9] = m_posbh[9];
          op[10] = m_posbh[10];
          op[11] = m_posbh[11];
          op[12] = m_posbh[12];
          op[13] = m_posbh[13];
          op[14] = m_posbh[14];
          op[15] = m_posbh[15];
          op[16] = m_posbh[16];
          op[17] = m_posbh[17];
          op[18] = m_posbh[18];
          op[19] = m_posbh[19];
          op[20] = m_posbh[20];
          op[21] = m_posbh[21];
          op[22] = m_posbh[22];
          op[23] = m_posbh[23];
          op[24] = m_posbh[24];
          op[25] = m_posbh[25];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbh;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbh + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbh + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbh + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbh + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbh + 5);
          *((_WORD *)op + 12) = *((_WORD *)m_posbh + 12);
        }
        op[26] = m_posbh[26];
        op += 27;
        goto next_state;
      case 0x3Au:
      case 0x13Au:
      case 0x23Au:
        v38 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbi = &op[-v38 - 1];
        v403 += 2;
        if ( v38 + 1 < 4 )
        {
          *op = *m_posbi;
          op[1] = op[-v38];
          op[2] = m_posbi[2];
          op[3] = m_posbi[3];
          op[4] = m_posbi[4];
          op[5] = m_posbi[5];
          op[6] = m_posbi[6];
          op[7] = m_posbi[7];
          op[8] = m_posbi[8];
          op[9] = m_posbi[9];
          op[10] = m_posbi[10];
          op[11] = m_posbi[11];
          op[12] = m_posbi[12];
          op[13] = m_posbi[13];
          op[14] = m_posbi[14];
          op[15] = m_posbi[15];
          op[16] = m_posbi[16];
          op[17] = m_posbi[17];
          op[18] = m_posbi[18];
          op[19] = m_posbi[19];
          op[20] = m_posbi[20];
          op[21] = m_posbi[21];
          op[22] = m_posbi[22];
          op[23] = m_posbi[23];
          op[24] = m_posbi[24];
          op[25] = m_posbi[25];
          op[26] = m_posbi[26];
          op[27] = m_posbi[27];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbi;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbi + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbi + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbi + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbi + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbi + 5);
          *((unsigned int *)op + 6) = *((unsigned int *)m_posbi + 6);
        }
        op += 28;
        goto next_state;
      case 0x3Bu:
      case 0x13Bu:
      case 0x23Bu:
        v39 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbj = &op[-v39 - 1];
        v403 += 2;
        if ( v39 + 1 < 4 )
        {
          *op = *m_posbj;
          op[1] = op[-v39];
          op[2] = m_posbj[2];
          op[3] = m_posbj[3];
          op[4] = m_posbj[4];
          op[5] = m_posbj[5];
          op[6] = m_posbj[6];
          op[7] = m_posbj[7];
          op[8] = m_posbj[8];
          op[9] = m_posbj[9];
          op[10] = m_posbj[10];
          op[11] = m_posbj[11];
          op[12] = m_posbj[12];
          op[13] = m_posbj[13];
          op[14] = m_posbj[14];
          op[15] = m_posbj[15];
          op[16] = m_posbj[16];
          op[17] = m_posbj[17];
          op[18] = m_posbj[18];
          op[19] = m_posbj[19];
          op[20] = m_posbj[20];
          op[21] = m_posbj[21];
          op[22] = m_posbj[22];
          op[23] = m_posbj[23];
          op[24] = m_posbj[24];
          op[25] = m_posbj[25];
          op[26] = m_posbj[26];
          op[27] = m_posbj[27];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbj;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbj + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbj + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbj + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbj + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbj + 5);
          *((unsigned int *)op + 6) = *((unsigned int *)m_posbj + 6);
        }
        op[28] = m_posbj[28];
        op += 29;
        goto next_state;
      case 0x3Cu:
      case 0x13Cu:
      case 0x23Cu:
        v40 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbk = &op[-v40 - 1];
        v403 += 2;
        if ( v40 + 1 < 4 )
        {
          *op = *m_posbk;
          op[1] = op[-v40];
          op[2] = m_posbk[2];
          op[3] = m_posbk[3];
          op[4] = m_posbk[4];
          op[5] = m_posbk[5];
          op[6] = m_posbk[6];
          op[7] = m_posbk[7];
          op[8] = m_posbk[8];
          op[9] = m_posbk[9];
          op[10] = m_posbk[10];
          op[11] = m_posbk[11];
          op[12] = m_posbk[12];
          op[13] = m_posbk[13];
          op[14] = m_posbk[14];
          op[15] = m_posbk[15];
          op[16] = m_posbk[16];
          op[17] = m_posbk[17];
          op[18] = m_posbk[18];
          op[19] = m_posbk[19];
          op[20] = m_posbk[20];
          op[21] = m_posbk[21];
          op[22] = m_posbk[22];
          op[23] = m_posbk[23];
          op[24] = m_posbk[24];
          op[25] = m_posbk[25];
          op[26] = m_posbk[26];
          op[27] = m_posbk[27];
          op[28] = m_posbk[28];
          op[29] = m_posbk[29];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbk;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbk + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbk + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbk + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbk + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbk + 5);
          *((unsigned int *)op + 6) = *((unsigned int *)m_posbk + 6);
          *((_WORD *)op + 14) = *((_WORD *)m_posbk + 14);
        }
        op += 30;
        goto next_state;
      case 0x3Du:
      case 0x13Du:
      case 0x23Du:
        v41 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbl = &op[-v41 - 1];
        v403 += 2;
        if ( v41 + 1 < 4 )
        {
          *op = *m_posbl;
          op[1] = op[-v41];
          op[2] = m_posbl[2];
          op[3] = m_posbl[3];
          op[4] = m_posbl[4];
          op[5] = m_posbl[5];
          op[6] = m_posbl[6];
          op[7] = m_posbl[7];
          op[8] = m_posbl[8];
          op[9] = m_posbl[9];
          op[10] = m_posbl[10];
          op[11] = m_posbl[11];
          op[12] = m_posbl[12];
          op[13] = m_posbl[13];
          op[14] = m_posbl[14];
          op[15] = m_posbl[15];
          op[16] = m_posbl[16];
          op[17] = m_posbl[17];
          op[18] = m_posbl[18];
          op[19] = m_posbl[19];
          op[20] = m_posbl[20];
          op[21] = m_posbl[21];
          op[22] = m_posbl[22];
          op[23] = m_posbl[23];
          op[24] = m_posbl[24];
          op[25] = m_posbl[25];
          op[26] = m_posbl[26];
          op[27] = m_posbl[27];
          op[28] = m_posbl[28];
          op[29] = m_posbl[29];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbl;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbl + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbl + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbl + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbl + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbl + 5);
          *((unsigned int *)op + 6) = *((unsigned int *)m_posbl + 6);
          *((_WORD *)op + 14) = *((_WORD *)m_posbl + 14);
        }
        op[30] = m_posbl[30];
        op += 31;
        goto next_state;
      case 0x3Eu:
      case 0x13Eu:
      case 0x23Eu:
        v42 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbm = &op[-v42 - 1];
        v403 += 2;
        if ( v42 + 1 < 4 )
        {
          *op = *m_posbm;
          op[1] = op[-v42];
          op[2] = m_posbm[2];
          op[3] = m_posbm[3];
          op[4] = m_posbm[4];
          op[5] = m_posbm[5];
          op[6] = m_posbm[6];
          op[7] = m_posbm[7];
          op[8] = m_posbm[8];
          op[9] = m_posbm[9];
          op[10] = m_posbm[10];
          op[11] = m_posbm[11];
          op[12] = m_posbm[12];
          op[13] = m_posbm[13];
          op[14] = m_posbm[14];
          op[15] = m_posbm[15];
          op[16] = m_posbm[16];
          op[17] = m_posbm[17];
          op[18] = m_posbm[18];
          op[19] = m_posbm[19];
          op[20] = m_posbm[20];
          op[21] = m_posbm[21];
          op[22] = m_posbm[22];
          op[23] = m_posbm[23];
          op[24] = m_posbm[24];
          op[25] = m_posbm[25];
          op[26] = m_posbm[26];
          op[27] = m_posbm[27];
          op[28] = m_posbm[28];
          op[29] = m_posbm[29];
          op[30] = m_posbm[30];
          op[31] = m_posbm[31];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbm;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbm + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbm + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbm + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbm + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbm + 5);
          *((unsigned int *)op + 6) = *((unsigned int *)m_posbm + 6);
          *((unsigned int *)op + 7) = *((unsigned int *)m_posbm + 7);
        }
        op += 32;
        goto next_state;
      case 0x3Fu:
      case 0x13Fu:
      case 0x23Fu:
        v43 = (int)*(unsigned __int16 *)v403 >> 2;
        m_posbn = &op[-v43 - 1];
        v403 += 2;
        if ( v43 + 1 < 4 )
        {
          *op = *m_posbn;
          op[1] = op[-v43];
          op[2] = m_posbn[2];
          op[3] = m_posbn[3];
          op[4] = m_posbn[4];
          op[5] = m_posbn[5];
          op[6] = m_posbn[6];
          op[7] = m_posbn[7];
          op[8] = m_posbn[8];
          op[9] = m_posbn[9];
          op[10] = m_posbn[10];
          op[11] = m_posbn[11];
          op[12] = m_posbn[12];
          op[13] = m_posbn[13];
          op[14] = m_posbn[14];
          op[15] = m_posbn[15];
          op[16] = m_posbn[16];
          op[17] = m_posbn[17];
          op[18] = m_posbn[18];
          op[19] = m_posbn[19];
          op[20] = m_posbn[20];
          op[21] = m_posbn[21];
          op[22] = m_posbn[22];
          op[23] = m_posbn[23];
          op[24] = m_posbn[24];
          op[25] = m_posbn[25];
          op[26] = m_posbn[26];
          op[27] = m_posbn[27];
          op[28] = m_posbn[28];
          op[29] = m_posbn[29];
          op[30] = m_posbn[30];
          op[31] = m_posbn[31];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbn;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posbn + 1);
          *((unsigned int *)op + 2) = *((unsigned int *)m_posbn + 2);
          *((unsigned int *)op + 3) = *((unsigned int *)m_posbn + 3);
          *((unsigned int *)op + 4) = *((unsigned int *)m_posbn + 4);
          *((unsigned int *)op + 5) = *((unsigned int *)m_posbn + 5);
          *((unsigned int *)op + 6) = *((unsigned int *)m_posbn + 6);
          *((unsigned int *)op + 7) = *((unsigned int *)m_posbn + 7);
        }
        op[32] = m_posbn[32];
        op += 33;
        goto next_state;
      case 0x40u:
      case 0x140u:
      case 0x240u:
        v44 = 8 * *v403;
        m_posbo = &op[-v44 - 1];
        ++v403;
        if ( v44 + 1 < 4 )
        {
          *op = *m_posbo;
          op[1] = op[-v44];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posbo;
        }
        op[2] = m_posbo[2];
        op += 3;
        goto next_state_0;
      case 0x41u:
      case 0x141u:
      case 0x241u:
        v45 = 8 * *v403;
        m_posbp = &op[-v45 - 1];
        v404 = v403 + 1;
        if ( v45 + 1 < 4 )
        {
          *op = *m_posbp;
          op[1] = op[-v45];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posbp;
        }
        op[2] = m_posbp[2];
        opa = op + 3;
        goto next_state_1;
      case 0x42u:
      case 0x142u:
      case 0x242u:
        v46 = 8 * *v403;
        m_posbq = &op[-v46 - 1];
        v405 = v403 + 1;
        if ( v46 + 1 < 4 )
        {
          *op = *m_posbq;
          op[1] = op[-v46];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posbq;
        }
        op[2] = m_posbq[2];
        opb = op + 3;
        goto next_state_2;
      case 0x43u:
      case 0x143u:
      case 0x243u:
        v47 = 8 * *v403;
        m_posbr = &op[-v47 - 1];
        v406 = v403 + 1;
        if ( v47 + 1 < 4 )
        {
          *op = *m_posbr;
          op[1] = op[-v47];
        }
        else
        {
          *(_WORD *)op = *(_WORD *)m_posbr;
        }
        op[2] = m_posbr[2];
        opc = op + 3;
        goto next_state_3;
      case 0x44u:
      case 0x144u:
      case 0x244u:
        m_posfs = &op[-8 * *v403++ - 2];
        *(_WORD *)op = *(_WORD *)m_posfs;
        op[2] = m_posfs[2];
        op += 3;
        goto next_state_0;
      case 0x45u:
      case 0x145u:
      case 0x245u:
        m_posft = &op[-8 * *v403 - 2];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posft;
        op[2] = m_posft[2];
        opa = op + 3;
        goto next_state_1;
      case 0x46u:
      case 0x146u:
      case 0x246u:
        m_posfu = &op[-8 * *v403 - 2];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posfu;
        op[2] = m_posfu[2];
        opb = op + 3;
        goto next_state_2;
      case 0x47u:
      case 0x147u:
      case 0x247u:
        m_posfv = &op[-8 * *v403 - 2];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posfv;
        op[2] = m_posfv[2];
        opc = op + 3;
        goto next_state_3;
      case 0x48u:
      case 0x148u:
      case 0x248u:
        m_posfw = &op[-8 * *v403++ - 3];
        *(_WORD *)op = *(_WORD *)m_posfw;
        op[2] = m_posfw[2];
        op += 3;
        goto next_state_0;
      case 0x49u:
      case 0x149u:
      case 0x249u:
        m_posfx = &op[-8 * *v403 - 3];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posfx;
        op[2] = m_posfx[2];
        opa = op + 3;
        goto next_state_1;
      case 0x4Au:
      case 0x14Au:
      case 0x24Au:
        m_posfy = &op[-8 * *v403 - 3];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posfy;
        op[2] = m_posfy[2];
        opb = op + 3;
        goto next_state_2;
      case 0x4Bu:
      case 0x14Bu:
      case 0x24Bu:
        m_posfz = &op[-8 * *v403 - 3];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posfz;
        op[2] = m_posfz[2];
        opc = op + 3;
        goto next_state_3;
      case 0x4Cu:
      case 0x14Cu:
      case 0x24Cu:
        m_posga = &op[-8 * *v403++ - 4];
        *(_WORD *)op = *(_WORD *)m_posga;
        op[2] = m_posga[2];
        op += 3;
        goto next_state_0;
      case 0x4Du:
      case 0x14Du:
      case 0x24Du:
        m_posgb = &op[-8 * *v403 - 4];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgb;
        op[2] = m_posgb[2];
        opa = op + 3;
        goto next_state_1;
      case 0x4Eu:
      case 0x14Eu:
      case 0x24Eu:
        m_posgc = &op[-8 * *v403 - 4];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgc;
        op[2] = m_posgc[2];
        opb = op + 3;
        goto next_state_2;
      case 0x4Fu:
      case 0x14Fu:
      case 0x24Fu:
        m_posgd = &op[-8 * *v403 - 4];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgd;
        op[2] = m_posgd[2];
        opc = op + 3;
        goto next_state_3;
      case 0x50u:
      case 0x150u:
      case 0x250u:
        m_posge = &op[-8 * *v403++ - 5];
        *(_WORD *)op = *(_WORD *)m_posge;
        op[2] = m_posge[2];
        op += 3;
        goto next_state_0;
      case 0x51u:
      case 0x151u:
      case 0x251u:
        m_posgf = &op[-8 * *v403 - 5];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgf;
        op[2] = m_posgf[2];
        opa = op + 3;
        goto next_state_1;
      case 0x52u:
      case 0x152u:
      case 0x252u:
        m_posgg = &op[-8 * *v403 - 5];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgg;
        op[2] = m_posgg[2];
        opb = op + 3;
        goto next_state_2;
      case 0x53u:
      case 0x153u:
      case 0x253u:
        m_posgh = &op[-8 * *v403 - 5];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgh;
        op[2] = m_posgh[2];
        opc = op + 3;
        goto next_state_3;
      case 0x54u:
      case 0x154u:
      case 0x254u:
        m_posgi = &op[-8 * *v403++ - 6];
        *(_WORD *)op = *(_WORD *)m_posgi;
        op[2] = m_posgi[2];
        op += 3;
        goto next_state_0;
      case 0x55u:
      case 0x155u:
      case 0x255u:
        m_posgj = &op[-8 * *v403 - 6];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgj;
        op[2] = m_posgj[2];
        opa = op + 3;
        goto next_state_1;
      case 0x56u:
      case 0x156u:
      case 0x256u:
        m_posgk = &op[-8 * *v403 - 6];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgk;
        op[2] = m_posgk[2];
        opb = op + 3;
        goto next_state_2;
      case 0x57u:
      case 0x157u:
      case 0x257u:
        m_posgl = &op[-8 * *v403 - 6];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgl;
        op[2] = m_posgl[2];
        opc = op + 3;
        goto next_state_3;
      case 0x58u:
      case 0x158u:
      case 0x258u:
        m_posgm = &op[-8 * *v403++ - 7];
        *(_WORD *)op = *(_WORD *)m_posgm;
        op[2] = m_posgm[2];
        op += 3;
        goto next_state_0;
      case 0x59u:
      case 0x159u:
      case 0x259u:
        m_posgn = &op[-8 * *v403 - 7];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgn;
        op[2] = m_posgn[2];
        opa = op + 3;
        goto next_state_1;
      case 0x5Au:
      case 0x15Au:
      case 0x25Au:
        m_posgo = &op[-8 * *v403 - 7];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgo;
        op[2] = m_posgo[2];
        opb = op + 3;
        goto next_state_2;
      case 0x5Bu:
      case 0x15Bu:
      case 0x25Bu:
        m_posgp = &op[-8 * *v403 - 7];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgp;
        op[2] = m_posgp[2];
        opc = op + 3;
        goto next_state_3;
      case 0x5Cu:
      case 0x15Cu:
      case 0x25Cu:
        m_posgq = &op[-8 * *v403++ - 8];
        *(_WORD *)op = *(_WORD *)m_posgq;
        op[2] = m_posgq[2];
        op += 3;
        goto next_state_0;
      case 0x5Du:
      case 0x15Du:
      case 0x25Du:
        m_posgr = &op[-8 * *v403 - 8];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgr;
        op[2] = m_posgr[2];
        opa = op + 3;
        goto next_state_1;
      case 0x5Eu:
      case 0x15Eu:
      case 0x25Eu:
        m_posgs = &op[-8 * *v403 - 8];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgs;
        op[2] = m_posgs[2];
        opb = op + 3;
        goto next_state_2;
      case 0x5Fu:
      case 0x15Fu:
      case 0x25Fu:
        m_posgt = &op[-8 * *v403 - 8];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posgt;
        op[2] = m_posgt[2];
        opc = op + 3;
        goto next_state_3;
      case 0x60u:
      case 0x160u:
      case 0x260u:
        v48 = 8 * *v403;
        m_posbs = &op[-v48 - 1];
        ++v403;
        if ( v48 + 1 < 4 )
        {
          *op = *m_posbs;
          op[1] = op[-v48];
          op[2] = m_posbs[2];
          op[3] = m_posbs[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbs;
        }
        op += 4;
        goto next_state_0;
      case 0x61u:
      case 0x161u:
      case 0x261u:
        v49 = 8 * *v403;
        m_posbt = &op[-v49 - 1];
        v404 = v403 + 1;
        if ( v49 + 1 < 4 )
        {
          *op = *m_posbt;
          op[1] = op[-v49];
          op[2] = m_posbt[2];
          op[3] = m_posbt[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbt;
        }
        opa = op + 4;
        goto next_state_1;
      case 0x62u:
      case 0x162u:
      case 0x262u:
        v50 = 8 * *v403;
        m_posbu = &op[-v50 - 1];
        v405 = v403 + 1;
        if ( v50 + 1 < 4 )
        {
          *op = *m_posbu;
          op[1] = op[-v50];
          op[2] = m_posbu[2];
          op[3] = m_posbu[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbu;
        }
        opb = op + 4;
        goto next_state_2;
      case 0x63u:
      case 0x163u:
      case 0x263u:
        v51 = 8 * *v403;
        m_posbv = &op[-v51 - 1];
        v406 = v403 + 1;
        if ( v51 + 1 < 4 )
        {
          *op = *m_posbv;
          op[1] = op[-v51];
          op[2] = m_posbv[2];
          op[3] = m_posbv[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbv;
        }
        opc = op + 4;
        goto next_state_3;
      case 0x64u:
      case 0x164u:
      case 0x264u:
        v52 = 8 * *v403;
        m_posbw = &op[-v52 - 2];
        ++v403;
        if ( v52 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posbw;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v52];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbw;
        }
        op += 4;
        goto next_state_0;
      case 0x65u:
      case 0x165u:
      case 0x265u:
        v53 = 8 * *v403;
        m_posbx = &op[-v53 - 2];
        v404 = v403 + 1;
        if ( v53 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posbx;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v53];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbx;
        }
        opa = op + 4;
        goto next_state_1;
      case 0x66u:
      case 0x166u:
      case 0x266u:
        v54 = 8 * *v403;
        m_posby = &op[-v54 - 2];
        v405 = v403 + 1;
        if ( v54 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posby;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v54];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posby;
        }
        opb = op + 4;
        goto next_state_2;
      case 0x67u:
      case 0x167u:
      case 0x267u:
        v55 = 8 * *v403;
        m_posbz = &op[-v55 - 2];
        v406 = v403 + 1;
        if ( v55 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posbz;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v55];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posbz;
        }
        opc = op + 4;
        goto next_state_3;
      case 0x68u:
      case 0x168u:
      case 0x268u:
        v56 = 8 * *v403;
        m_posca = &op[-v56 - 3];
        ++v403;
        if ( v56 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posca;
          *((_WORD *)op + 1) = *((_WORD *)m_posca + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posca;
        }
        op += 4;
        goto next_state_0;
      case 0x69u:
      case 0x169u:
      case 0x269u:
        v57 = 8 * *v403;
        m_poscb = &op[-v57 - 3];
        v404 = v403 + 1;
        if ( v57 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscb;
          *((_WORD *)op + 1) = *((_WORD *)m_poscb + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscb;
        }
        opa = op + 4;
        goto next_state_1;
      case 0x6Au:
      case 0x16Au:
      case 0x26Au:
        v58 = 8 * *v403;
        m_poscc = &op[-v58 - 3];
        v405 = v403 + 1;
        if ( v58 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscc;
          *((_WORD *)op + 1) = *((_WORD *)m_poscc + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscc;
        }
        opb = op + 4;
        goto next_state_2;
      case 0x6Bu:
      case 0x16Bu:
      case 0x26Bu:
        v59 = 8 * *v403;
        m_poscd = &op[-v59 - 3];
        v406 = v403 + 1;
        if ( v59 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscd;
          *((_WORD *)op + 1) = *((_WORD *)m_poscd + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscd;
        }
        opc = op + 4;
        goto next_state_3;
      case 0x6Cu:
      case 0x16Cu:
      case 0x26Cu:
        m_posgu = &op[-8 * *v403++ - 4];
        *(unsigned int *)op = *(unsigned int *)m_posgu;
        op += 4;
        goto next_state_0;
      case 0x6Du:
      case 0x16Du:
      case 0x26Du:
        m_posgv = &op[-8 * *v403 - 4];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posgv;
        opa = op + 4;
        goto next_state_1;
      case 0x6Eu:
      case 0x16Eu:
      case 0x26Eu:
        m_posgw = &op[-8 * *v403 - 4];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posgw;
        opb = op + 4;
        goto next_state_2;
      case 0x6Fu:
      case 0x16Fu:
      case 0x26Fu:
        m_posgx = &op[-8 * *v403 - 4];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posgx;
        opc = op + 4;
        goto next_state_3;
      case 0x70u:
      case 0x170u:
      case 0x270u:
        m_posgy = &op[-8 * *v403++ - 5];
        *(unsigned int *)op = *(unsigned int *)m_posgy;
        op += 4;
        goto next_state_0;
      case 0x71u:
      case 0x171u:
      case 0x271u:
        m_posgz = &op[-8 * *v403 - 5];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posgz;
        opa = op + 4;
        goto next_state_1;
      case 0x72u:
      case 0x172u:
      case 0x272u:
        m_posha = &op[-8 * *v403 - 5];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posha;
        opb = op + 4;
        goto next_state_2;
      case 0x73u:
      case 0x173u:
      case 0x273u:
        m_poshb = &op[-8 * *v403 - 5];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshb;
        opc = op + 4;
        goto next_state_3;
      case 0x74u:
      case 0x174u:
      case 0x274u:
        m_poshc = &op[-8 * *v403++ - 6];
        *(unsigned int *)op = *(unsigned int *)m_poshc;
        op += 4;
        goto next_state_0;
      case 0x75u:
      case 0x175u:
      case 0x275u:
        m_poshd = &op[-8 * *v403 - 6];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshd;
        opa = op + 4;
        goto next_state_1;
      case 0x76u:
      case 0x176u:
      case 0x276u:
        m_poshe = &op[-8 * *v403 - 6];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshe;
        opb = op + 4;
        goto next_state_2;
      case 0x77u:
      case 0x177u:
      case 0x277u:
        m_poshf = &op[-8 * *v403 - 6];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshf;
        opc = op + 4;
        goto next_state_3;
      case 0x78u:
      case 0x178u:
      case 0x278u:
        m_poshg = &op[-8 * *v403++ - 7];
        *(unsigned int *)op = *(unsigned int *)m_poshg;
        op += 4;
        goto next_state_0;
      case 0x79u:
      case 0x179u:
      case 0x279u:
        m_poshh = &op[-8 * *v403 - 7];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshh;
        opa = op + 4;
        goto next_state_1;
      case 0x7Au:
      case 0x17Au:
      case 0x27Au:
        m_poshi = &op[-8 * *v403 - 7];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshi;
        opb = op + 4;
        goto next_state_2;
      case 0x7Bu:
      case 0x17Bu:
      case 0x27Bu:
        m_poshj = &op[-8 * *v403 - 7];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshj;
        opc = op + 4;
        goto next_state_3;
      case 0x7Cu:
      case 0x17Cu:
      case 0x27Cu:
        m_poshk = &op[-8 * *v403++ - 8];
        *(unsigned int *)op = *(unsigned int *)m_poshk;
        op += 4;
        goto next_state_0;
      case 0x7Du:
      case 0x17Du:
      case 0x27Du:
        m_poshl = &op[-8 * *v403 - 8];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshl;
        opa = op + 4;
        goto next_state_1;
      case 0x7Eu:
      case 0x17Eu:
      case 0x27Eu:
        m_poshm = &op[-8 * *v403 - 8];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshm;
        opb = op + 4;
        goto next_state_2;
      case 0x7Fu:
      case 0x17Fu:
      case 0x27Fu:
        m_poshn = &op[-8 * *v403 - 8];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshn;
        opc = op + 4;
        goto next_state_3;
      case 0x80u:
      case 0x180u:
      case 0x280u:
        v60 = 8 * *v403;
        m_posce = &op[-v60 - 1];
        ++v403;
        if ( v60 + 1 < 4 )
        {
          *op = *m_posce;
          op[1] = op[-v60];
          op[2] = m_posce[2];
          op[3] = m_posce[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posce;
        }
        op[4] = m_posce[4];
        op += 5;
        goto next_state_0;
      case 0x81u:
      case 0x181u:
      case 0x281u:
        v61 = 8 * *v403;
        m_poscf = &op[-v61 - 1];
        v404 = v403 + 1;
        if ( v61 + 1 < 4 )
        {
          *op = *m_poscf;
          op[1] = op[-v61];
          op[2] = m_poscf[2];
          op[3] = m_poscf[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscf;
        }
        op[4] = m_poscf[4];
        opa = op + 5;
        goto next_state_1;
      case 0x82u:
      case 0x182u:
      case 0x282u:
        v62 = 8 * *v403;
        m_poscg = &op[-v62 - 1];
        v405 = v403 + 1;
        if ( v62 + 1 < 4 )
        {
          *op = *m_poscg;
          op[1] = op[-v62];
          op[2] = m_poscg[2];
          op[3] = m_poscg[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscg;
        }
        op[4] = m_poscg[4];
        opb = op + 5;
        goto next_state_2;
      case 0x83u:
      case 0x183u:
      case 0x283u:
        v63 = 8 * *v403;
        m_posch = &op[-v63 - 1];
        v406 = v403 + 1;
        if ( v63 + 1 < 4 )
        {
          *op = *m_posch;
          op[1] = op[-v63];
          op[2] = m_posch[2];
          op[3] = m_posch[3];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posch;
        }
        op[4] = m_posch[4];
        opc = op + 5;
        goto next_state_3;
      case 0x84u:
      case 0x184u:
      case 0x284u:
        v64 = 8 * *v403;
        m_posci = &op[-v64 - 2];
        ++v403;
        if ( v64 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posci;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v64];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posci;
        }
        op[4] = m_posci[4];
        op += 5;
        goto next_state_0;
      case 0x85u:
      case 0x185u:
      case 0x285u:
        v65 = 8 * *v403;
        m_poscj = &op[-v65 - 2];
        v404 = v403 + 1;
        if ( v65 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscj;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v65];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscj;
        }
        op[4] = m_poscj[4];
        opa = op + 5;
        goto next_state_1;
      case 0x86u:
      case 0x186u:
      case 0x286u:
        v66 = 8 * *v403;
        m_posck = &op[-v66 - 2];
        v405 = v403 + 1;
        if ( v66 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posck;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v66];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posck;
        }
        op[4] = m_posck[4];
        opb = op + 5;
        goto next_state_2;
      case 0x87u:
      case 0x187u:
      case 0x287u:
        v67 = 8 * *v403;
        m_poscl = &op[-v67 - 2];
        v406 = v403 + 1;
        if ( v67 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscl;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v67];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscl;
        }
        op[4] = m_poscl[4];
        opc = op + 5;
        goto next_state_3;
      case 0x88u:
      case 0x188u:
      case 0x288u:
        v68 = 8 * *v403;
        m_poscm = &op[-v68 - 3];
        ++v403;
        if ( v68 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscm;
          *((_WORD *)op + 1) = *((_WORD *)m_poscm + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscm;
        }
        op[4] = m_poscm[4];
        op += 5;
        goto next_state_0;
      case 0x89u:
      case 0x189u:
      case 0x289u:
        v69 = 8 * *v403;
        m_poscn = &op[-v69 - 3];
        v404 = v403 + 1;
        if ( v69 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscn;
          *((_WORD *)op + 1) = *((_WORD *)m_poscn + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscn;
        }
        op[4] = m_poscn[4];
        opa = op + 5;
        goto next_state_1;
      case 0x8Au:
      case 0x18Au:
      case 0x28Au:
        v70 = 8 * *v403;
        m_posco = &op[-v70 - 3];
        v405 = v403 + 1;
        if ( v70 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posco;
          *((_WORD *)op + 1) = *((_WORD *)m_posco + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posco;
        }
        op[4] = m_posco[4];
        opb = op + 5;
        goto next_state_2;
      case 0x8Bu:
      case 0x18Bu:
      case 0x28Bu:
        v71 = 8 * *v403;
        m_poscp = &op[-v71 - 3];
        v406 = v403 + 1;
        if ( v71 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscp;
          *((_WORD *)op + 1) = *((_WORD *)m_poscp + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscp;
        }
        op[4] = m_poscp[4];
        opc = op + 5;
        goto next_state_3;
      case 0x8Cu:
      case 0x18Cu:
      case 0x28Cu:
        m_posho = &op[-8 * *v403++ - 4];
        *(unsigned int *)op = *(unsigned int *)m_posho;
        op[4] = m_posho[4];
        op += 5;
        goto next_state_0;
      case 0x8Du:
      case 0x18Du:
      case 0x28Du:
        m_poshp = &op[-8 * *v403 - 4];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshp;
        op[4] = m_poshp[4];
        opa = op + 5;
        goto next_state_1;
      case 0x8Eu:
      case 0x18Eu:
      case 0x28Eu:
        m_poshq = &op[-8 * *v403 - 4];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshq;
        op[4] = m_poshq[4];
        opb = op + 5;
        goto next_state_2;
      case 0x8Fu:
      case 0x18Fu:
      case 0x28Fu:
        m_poshr = &op[-8 * *v403 - 4];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshr;
        op[4] = m_poshr[4];
        opc = op + 5;
        goto next_state_3;
      case 0x90u:
      case 0x190u:
      case 0x290u:
        m_poshs = &op[-8 * *v403++ - 5];
        *(unsigned int *)op = *(unsigned int *)m_poshs;
        op[4] = m_poshs[4];
        op += 5;
        goto next_state_0;
      case 0x91u:
      case 0x191u:
      case 0x291u:
        m_posht = &op[-8 * *v403 - 5];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posht;
        op[4] = m_posht[4];
        opa = op + 5;
        goto next_state_1;
      case 0x92u:
      case 0x192u:
      case 0x292u:
        m_poshu = &op[-8 * *v403 - 5];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshu;
        op[4] = m_poshu[4];
        opb = op + 5;
        goto next_state_2;
      case 0x93u:
      case 0x193u:
      case 0x293u:
        m_poshv = &op[-8 * *v403 - 5];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshv;
        op[4] = m_poshv[4];
        opc = op + 5;
        goto next_state_3;
      case 0x94u:
      case 0x194u:
      case 0x294u:
        m_poshw = &op[-8 * *v403++ - 6];
        *(unsigned int *)op = *(unsigned int *)m_poshw;
        op[4] = m_poshw[4];
        op += 5;
        goto next_state_0;
      case 0x95u:
      case 0x195u:
      case 0x295u:
        m_poshx = &op[-8 * *v403 - 6];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshx;
        op[4] = m_poshx[4];
        opa = op + 5;
        goto next_state_1;
      case 0x96u:
      case 0x196u:
      case 0x296u:
        m_poshy = &op[-8 * *v403 - 6];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshy;
        op[4] = m_poshy[4];
        opb = op + 5;
        goto next_state_2;
      case 0x97u:
      case 0x197u:
      case 0x297u:
        m_poshz = &op[-8 * *v403 - 6];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poshz;
        op[4] = m_poshz[4];
        opc = op + 5;
        goto next_state_3;
      case 0x98u:
      case 0x198u:
      case 0x298u:
        m_posia = &op[-8 * *v403++ - 7];
        *(unsigned int *)op = *(unsigned int *)m_posia;
        op[4] = m_posia[4];
        op += 5;
        goto next_state_0;
      case 0x99u:
      case 0x199u:
      case 0x299u:
        m_posib = &op[-8 * *v403 - 7];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posib;
        op[4] = m_posib[4];
        opa = op + 5;
        goto next_state_1;
      case 0x9Au:
      case 0x19Au:
      case 0x29Au:
        m_posic = &op[-8 * *v403 - 7];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posic;
        op[4] = m_posic[4];
        opb = op + 5;
        goto next_state_2;
      case 0x9Bu:
      case 0x19Bu:
      case 0x29Bu:
        m_posid = &op[-8 * *v403 - 7];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posid;
        op[4] = m_posid[4];
        opc = op + 5;
        goto next_state_3;
      case 0x9Cu:
      case 0x19Cu:
      case 0x29Cu:
        m_posie = &op[-8 * *v403++ - 8];
        *(unsigned int *)op = *(unsigned int *)m_posie;
        op[4] = m_posie[4];
        op += 5;
        goto next_state_0;
      case 0x9Du:
      case 0x19Du:
      case 0x29Du:
        m_posif = &op[-8 * *v403 - 8];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posif;
        op[4] = m_posif[4];
        opa = op + 5;
        goto next_state_1;
      case 0x9Eu:
      case 0x19Eu:
      case 0x29Eu:
        m_posig = &op[-8 * *v403 - 8];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posig;
        op[4] = m_posig[4];
        opb = op + 5;
        goto next_state_2;
      case 0x9Fu:
      case 0x19Fu:
      case 0x29Fu:
        m_posih = &op[-8 * *v403 - 8];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posih;
        op[4] = m_posih[4];
        opc = op + 5;
        goto next_state_3;
      case 0xA0u:
      case 0x1A0u:
      case 0x2A0u:
        v72 = 8 * *v403;
        m_poscq = &op[-v72 - 1];
        ++v403;
        if ( v72 + 1 < 4 )
        {
          *op = *m_poscq;
          op[1] = op[-v72];
          op[2] = m_poscq[2];
          op[3] = m_poscq[3];
          op[4] = m_poscq[4];
          op[5] = m_poscq[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscq;
          *((_WORD *)op + 2) = *((_WORD *)m_poscq + 2);
        }
        op += 6;
        goto next_state_0;
      case 0xA1u:
      case 0x1A1u:
      case 0x2A1u:
        v73 = 8 * *v403;
        m_poscr = &op[-v73 - 1];
        v404 = v403 + 1;
        if ( v73 + 1 < 4 )
        {
          *op = *m_poscr;
          op[1] = op[-v73];
          op[2] = m_poscr[2];
          op[3] = m_poscr[3];
          op[4] = m_poscr[4];
          op[5] = m_poscr[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscr;
          *((_WORD *)op + 2) = *((_WORD *)m_poscr + 2);
        }
        opa = op + 6;
        goto next_state_1;
      case 0xA2u:
      case 0x1A2u:
      case 0x2A2u:
        v74 = 8 * *v403;
        m_poscs = &op[-v74 - 1];
        v405 = v403 + 1;
        if ( v74 + 1 < 4 )
        {
          *op = *m_poscs;
          op[1] = op[-v74];
          op[2] = m_poscs[2];
          op[3] = m_poscs[3];
          op[4] = m_poscs[4];
          op[5] = m_poscs[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscs;
          *((_WORD *)op + 2) = *((_WORD *)m_poscs + 2);
        }
        opb = op + 6;
        goto next_state_2;
      case 0xA3u:
      case 0x1A3u:
      case 0x2A3u:
        v75 = 8 * *v403;
        m_posct = &op[-v75 - 1];
        v406 = v403 + 1;
        if ( v75 + 1 < 4 )
        {
          *op = *m_posct;
          op[1] = op[-v75];
          op[2] = m_posct[2];
          op[3] = m_posct[3];
          op[4] = m_posct[4];
          op[5] = m_posct[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posct;
          *((_WORD *)op + 2) = *((_WORD *)m_posct + 2);
        }
        opc = op + 6;
        goto next_state_3;
      case 0xA4u:
      case 0x1A4u:
      case 0x2A4u:
        v76 = 8 * *v403;
        m_poscu = &op[-v76 - 2];
        ++v403;
        if ( v76 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscu;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v76];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscu;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_poscu + 2);
        op += 6;
        goto next_state_0;
      case 0xA5u:
      case 0x1A5u:
      case 0x2A5u:
        v77 = 8 * *v403;
        m_poscv = &op[-v77 - 2];
        v404 = v403 + 1;
        if ( v77 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscv;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v77];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscv;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_poscv + 2);
        opa = op + 6;
        goto next_state_1;
      case 0xA6u:
      case 0x1A6u:
      case 0x2A6u:
        v78 = 8 * *v403;
        m_poscw = &op[-v78 - 2];
        v405 = v403 + 1;
        if ( v78 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscw;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v78];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscw;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_poscw + 2);
        opb = op + 6;
        goto next_state_2;
      case 0xA7u:
      case 0x1A7u:
      case 0x2A7u:
        v79 = 8 * *v403;
        m_poscx = &op[-v79 - 2];
        v406 = v403 + 1;
        if ( v79 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscx;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v79];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscx;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_poscx + 2);
        opc = op + 6;
        goto next_state_3;
      case 0xA8u:
      case 0x1A8u:
      case 0x2A8u:
        v80 = 8 * *v403;
        m_poscy = &op[-v80 - 3];
        ++v403;
        if ( v80 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscy;
          *((_WORD *)op + 1) = *((_WORD *)m_poscy + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscy;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_poscy + 2);
        op += 6;
        goto next_state_0;
      case 0xA9u:
      case 0x1A9u:
      case 0x2A9u:
        v81 = 8 * *v403;
        m_poscz = &op[-v81 - 3];
        v404 = v403 + 1;
        if ( v81 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_poscz;
          *((_WORD *)op + 1) = *((_WORD *)m_poscz + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_poscz;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_poscz + 2);
        opa = op + 6;
        goto next_state_1;
      case 0xAAu:
      case 0x1AAu:
      case 0x2AAu:
        v82 = 8 * *v403;
        m_posda = &op[-v82 - 3];
        v405 = v403 + 1;
        if ( v82 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posda;
          *((_WORD *)op + 1) = *((_WORD *)m_posda + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posda;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posda + 2);
        opb = op + 6;
        goto next_state_2;
      case 0xABu:
      case 0x1ABu:
      case 0x2ABu:
        v83 = 8 * *v403;
        m_posdb = &op[-v83 - 3];
        v406 = v403 + 1;
        if ( v83 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdb;
          *((_WORD *)op + 1) = *((_WORD *)m_posdb + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdb;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdb + 2);
        opc = op + 6;
        goto next_state_3;
      case 0xACu:
      case 0x1ACu:
      case 0x2ACu:
        m_posii = &op[-8 * *v403++ - 4];
        *(unsigned int *)op = *(unsigned int *)m_posii;
        *((_WORD *)op + 2) = *((_WORD *)m_posii + 2);
        op += 6;
        goto next_state_0;
      case 0xADu:
      case 0x1ADu:
      case 0x2ADu:
        m_posij = &op[-8 * *v403 - 4];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posij;
        *((_WORD *)op + 2) = *((_WORD *)m_posij + 2);
        opa = op + 6;
        goto next_state_1;
      case 0xAEu:
      case 0x1AEu:
      case 0x2AEu:
        m_posik = &op[-8 * *v403 - 4];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posik;
        *((_WORD *)op + 2) = *((_WORD *)m_posik + 2);
        opb = op + 6;
        goto next_state_2;
      case 0xAFu:
      case 0x1AFu:
      case 0x2AFu:
        m_posil = &op[-8 * *v403 - 4];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posil;
        *((_WORD *)op + 2) = *((_WORD *)m_posil + 2);
        opc = op + 6;
        goto next_state_3;
      case 0xB0u:
      case 0x1B0u:
      case 0x2B0u:
        m_posim = &op[-8 * *v403++ - 5];
        *(unsigned int *)op = *(unsigned int *)m_posim;
        *((_WORD *)op + 2) = *((_WORD *)m_posim + 2);
        op += 6;
        goto next_state_0;
      case 0xB1u:
      case 0x1B1u:
      case 0x2B1u:
        m_posin = &op[-8 * *v403 - 5];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posin;
        *((_WORD *)op + 2) = *((_WORD *)m_posin + 2);
        opa = op + 6;
        goto next_state_1;
      case 0xB2u:
      case 0x1B2u:
      case 0x2B2u:
        m_posio = &op[-8 * *v403 - 5];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posio;
        *((_WORD *)op + 2) = *((_WORD *)m_posio + 2);
        opb = op + 6;
        goto next_state_2;
      case 0xB3u:
      case 0x1B3u:
      case 0x2B3u:
        m_posip = &op[-8 * *v403 - 5];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posip;
        *((_WORD *)op + 2) = *((_WORD *)m_posip + 2);
        opc = op + 6;
        goto next_state_3;
      case 0xB4u:
      case 0x1B4u:
      case 0x2B4u:
        m_posiq = &op[-8 * *v403++ - 6];
        *(unsigned int *)op = *(unsigned int *)m_posiq;
        *((_WORD *)op + 2) = *((_WORD *)m_posiq + 2);
        op += 6;
        goto next_state_0;
      case 0xB5u:
      case 0x1B5u:
      case 0x2B5u:
        m_posir = &op[-8 * *v403 - 6];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posir;
        *((_WORD *)op + 2) = *((_WORD *)m_posir + 2);
        opa = op + 6;
        goto next_state_1;
      case 0xB6u:
      case 0x1B6u:
      case 0x2B6u:
        m_posis = &op[-8 * *v403 - 6];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posis;
        *((_WORD *)op + 2) = *((_WORD *)m_posis + 2);
        opb = op + 6;
        goto next_state_2;
      case 0xB7u:
      case 0x1B7u:
      case 0x2B7u:
        m_posit = &op[-8 * *v403 - 6];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posit;
        *((_WORD *)op + 2) = *((_WORD *)m_posit + 2);
        opc = op + 6;
        goto next_state_3;
      case 0xB8u:
      case 0x1B8u:
      case 0x2B8u:
        m_posiu = &op[-8 * *v403++ - 7];
        *(unsigned int *)op = *(unsigned int *)m_posiu;
        *((_WORD *)op + 2) = *((_WORD *)m_posiu + 2);
        op += 6;
        goto next_state_0;
      case 0xB9u:
      case 0x1B9u:
      case 0x2B9u:
        m_posiv = &op[-8 * *v403 - 7];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posiv;
        *((_WORD *)op + 2) = *((_WORD *)m_posiv + 2);
        opa = op + 6;
        goto next_state_1;
      case 0xBAu:
      case 0x1BAu:
      case 0x2BAu:
        m_posiw = &op[-8 * *v403 - 7];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posiw;
        *((_WORD *)op + 2) = *((_WORD *)m_posiw + 2);
        opb = op + 6;
        goto next_state_2;
      case 0xBBu:
      case 0x1BBu:
      case 0x2BBu:
        m_posix = &op[-8 * *v403 - 7];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posix;
        *((_WORD *)op + 2) = *((_WORD *)m_posix + 2);
        opc = op + 6;
        goto next_state_3;
      case 0xBCu:
      case 0x1BCu:
      case 0x2BCu:
        m_posiy = &op[-8 * *v403++ - 8];
        *(unsigned int *)op = *(unsigned int *)m_posiy;
        *((_WORD *)op + 2) = *((_WORD *)m_posiy + 2);
        op += 6;
        goto next_state_0;
      case 0xBDu:
      case 0x1BDu:
      case 0x2BDu:
        m_posiz = &op[-8 * *v403 - 8];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posiz;
        *((_WORD *)op + 2) = *((_WORD *)m_posiz + 2);
        opa = op + 6;
        goto next_state_1;
      case 0xBEu:
      case 0x1BEu:
      case 0x2BEu:
        m_posja = &op[-8 * *v403 - 8];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posja;
        *((_WORD *)op + 2) = *((_WORD *)m_posja + 2);
        opb = op + 6;
        goto next_state_2;
      case 0xBFu:
      case 0x1BFu:
      case 0x2BFu:
        m_posjb = &op[-8 * *v403 - 8];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjb;
        *((_WORD *)op + 2) = *((_WORD *)m_posjb + 2);
        opc = op + 6;
        goto next_state_3;
      case 0xC0u:
      case 0x1C0u:
      case 0x2C0u:
        v84 = 8 * *v403;
        m_posdc = &op[-v84 - 1];
        ++v403;
        if ( v84 + 1 < 4 )
        {
          *op = *m_posdc;
          op[1] = op[-v84];
          op[2] = m_posdc[2];
          op[3] = m_posdc[3];
          op[4] = m_posdc[4];
          op[5] = m_posdc[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdc;
          *((_WORD *)op + 2) = *((_WORD *)m_posdc + 2);
        }
        op[6] = m_posdc[6];
        op += 7;
        goto next_state_0;
      case 0xC1u:
      case 0x1C1u:
      case 0x2C1u:
        v85 = 8 * *v403;
        m_posdd = &op[-v85 - 1];
        v404 = v403 + 1;
        if ( v85 + 1 < 4 )
        {
          *op = *m_posdd;
          op[1] = op[-v85];
          op[2] = m_posdd[2];
          op[3] = m_posdd[3];
          op[4] = m_posdd[4];
          op[5] = m_posdd[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdd;
          *((_WORD *)op + 2) = *((_WORD *)m_posdd + 2);
        }
        op[6] = m_posdd[6];
        opa = op + 7;
        goto next_state_1;
      case 0xC2u:
      case 0x1C2u:
      case 0x2C2u:
        v86 = 8 * *v403;
        m_posde = &op[-v86 - 1];
        v405 = v403 + 1;
        if ( v86 + 1 < 4 )
        {
          *op = *m_posde;
          op[1] = op[-v86];
          op[2] = m_posde[2];
          op[3] = m_posde[3];
          op[4] = m_posde[4];
          op[5] = m_posde[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posde;
          *((_WORD *)op + 2) = *((_WORD *)m_posde + 2);
        }
        op[6] = m_posde[6];
        opb = op + 7;
        goto next_state_2;
      case 0xC3u:
      case 0x1C3u:
      case 0x2C3u:
        v87 = 8 * *v403;
        m_posdf = &op[-v87 - 1];
        v406 = v403 + 1;
        if ( v87 + 1 < 4 )
        {
          *op = *m_posdf;
          op[1] = op[-v87];
          op[2] = m_posdf[2];
          op[3] = m_posdf[3];
          op[4] = m_posdf[4];
          op[5] = m_posdf[5];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdf;
          *((_WORD *)op + 2) = *((_WORD *)m_posdf + 2);
        }
        op[6] = m_posdf[6];
        opc = op + 7;
        goto next_state_3;
      case 0xC4u:
      case 0x1C4u:
      case 0x2C4u:
        v88 = 8 * *v403;
        m_posdg = &op[-v88 - 2];
        ++v403;
        if ( v88 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdg;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v88];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdg;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdg + 2);
        op[6] = m_posdg[6];
        op += 7;
        goto next_state_0;
      case 0xC5u:
      case 0x1C5u:
      case 0x2C5u:
        v89 = 8 * *v403;
        m_posdh = &op[-v89 - 2];
        v404 = v403 + 1;
        if ( v89 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdh;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v89];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdh;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdh + 2);
        op[6] = m_posdh[6];
        opa = op + 7;
        goto next_state_1;
      case 0xC6u:
      case 0x1C6u:
      case 0x2C6u:
        v90 = 8 * *v403;
        m_posdi = &op[-v90 - 2];
        v405 = v403 + 1;
        if ( v90 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdi;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v90];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdi;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdi + 2);
        op[6] = m_posdi[6];
        opb = op + 7;
        goto next_state_2;
      case 0xC7u:
      case 0x1C7u:
      case 0x2C7u:
        v91 = 8 * *v403;
        m_posdj = &op[-v91 - 2];
        v406 = v403 + 1;
        if ( v91 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdj;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v91];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdj;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdj + 2);
        op[6] = m_posdj[6];
        opc = op + 7;
        goto next_state_3;
      case 0xC8u:
      case 0x1C8u:
      case 0x2C8u:
        v92 = 8 * *v403;
        m_posdk = &op[-v92 - 3];
        ++v403;
        if ( v92 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdk;
          *((_WORD *)op + 1) = *((_WORD *)m_posdk + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdk;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdk + 2);
        op[6] = m_posdk[6];
        op += 7;
        goto next_state_0;
      case 0xC9u:
      case 0x1C9u:
      case 0x2C9u:
        v93 = 8 * *v403;
        m_posdl = &op[-v93 - 3];
        v404 = v403 + 1;
        if ( v93 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdl;
          *((_WORD *)op + 1) = *((_WORD *)m_posdl + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdl;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdl + 2);
        op[6] = m_posdl[6];
        opa = op + 7;
        goto next_state_1;
      case 0xCAu:
      case 0x1CAu:
      case 0x2CAu:
        v94 = 8 * *v403;
        m_posdm = &op[-v94 - 3];
        v405 = v403 + 1;
        if ( v94 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdm;
          *((_WORD *)op + 1) = *((_WORD *)m_posdm + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdm;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdm + 2);
        op[6] = m_posdm[6];
        opb = op + 7;
        goto next_state_2;
      case 0xCBu:
      case 0x1CBu:
      case 0x2CBu:
        v95 = 8 * *v403;
        m_posdn = &op[-v95 - 3];
        v406 = v403 + 1;
        if ( v95 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdn;
          *((_WORD *)op + 1) = *((_WORD *)m_posdn + 1);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdn;
        }
        *((_WORD *)op + 2) = *((_WORD *)m_posdn + 2);
        op[6] = m_posdn[6];
        opc = op + 7;
        goto next_state_3;
      case 0xCCu:
      case 0x1CCu:
      case 0x2CCu:
        m_posjc = &op[-8 * *v403++ - 4];
        *(unsigned int *)op = *(unsigned int *)m_posjc;
        *((_WORD *)op + 2) = *((_WORD *)m_posjc + 2);
        op[6] = m_posjc[6];
        op += 7;
        goto next_state_0;
      case 0xCDu:
      case 0x1CDu:
      case 0x2CDu:
        m_posjd = &op[-8 * *v403 - 4];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjd;
        *((_WORD *)op + 2) = *((_WORD *)m_posjd + 2);
        op[6] = m_posjd[6];
        opa = op + 7;
        goto next_state_1;
      case 0xCEu:
      case 0x1CEu:
      case 0x2CEu:
        m_posje = &op[-8 * *v403 - 4];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posje;
        *((_WORD *)op + 2) = *((_WORD *)m_posje + 2);
        op[6] = m_posje[6];
        opb = op + 7;
        goto next_state_2;
      case 0xCFu:
      case 0x1CFu:
      case 0x2CFu:
        m_posjf = &op[-8 * *v403 - 4];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjf;
        *((_WORD *)op + 2) = *((_WORD *)m_posjf + 2);
        op[6] = m_posjf[6];
        opc = op + 7;
        goto next_state_3;
      case 0xD0u:
      case 0x1D0u:
      case 0x2D0u:
        m_posjg = &op[-8 * *v403++ - 5];
        *(unsigned int *)op = *(unsigned int *)m_posjg;
        *((_WORD *)op + 2) = *((_WORD *)m_posjg + 2);
        op[6] = m_posjg[6];
        op += 7;
        goto next_state_0;
      case 0xD1u:
      case 0x1D1u:
      case 0x2D1u:
        m_posjh = &op[-8 * *v403 - 5];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjh;
        *((_WORD *)op + 2) = *((_WORD *)m_posjh + 2);
        op[6] = m_posjh[6];
        opa = op + 7;
        goto next_state_1;
      case 0xD2u:
      case 0x1D2u:
      case 0x2D2u:
        m_posji = &op[-8 * *v403 - 5];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posji;
        *((_WORD *)op + 2) = *((_WORD *)m_posji + 2);
        op[6] = m_posji[6];
        opb = op + 7;
        goto next_state_2;
      case 0xD3u:
      case 0x1D3u:
      case 0x2D3u:
        m_posjj = &op[-8 * *v403 - 5];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjj;
        *((_WORD *)op + 2) = *((_WORD *)m_posjj + 2);
        op[6] = m_posjj[6];
        opc = op + 7;
        goto next_state_3;
      case 0xD4u:
      case 0x1D4u:
      case 0x2D4u:
        m_posjk = &op[-8 * *v403++ - 6];
        *(unsigned int *)op = *(unsigned int *)m_posjk;
        *((_WORD *)op + 2) = *((_WORD *)m_posjk + 2);
        op[6] = m_posjk[6];
        op += 7;
        goto next_state_0;
      case 0xD5u:
      case 0x1D5u:
      case 0x2D5u:
        m_posjl = &op[-8 * *v403 - 6];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjl;
        *((_WORD *)op + 2) = *((_WORD *)m_posjl + 2);
        op[6] = m_posjl[6];
        opa = op + 7;
        goto next_state_1;
      case 0xD6u:
      case 0x1D6u:
      case 0x2D6u:
        m_posjm = &op[-8 * *v403 - 6];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjm;
        *((_WORD *)op + 2) = *((_WORD *)m_posjm + 2);
        op[6] = m_posjm[6];
        opb = op + 7;
        goto next_state_2;
      case 0xD7u:
      case 0x1D7u:
      case 0x2D7u:
        m_posjn = &op[-8 * *v403 - 6];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjn;
        *((_WORD *)op + 2) = *((_WORD *)m_posjn + 2);
        op[6] = m_posjn[6];
        opc = op + 7;
        goto next_state_3;
      case 0xD8u:
      case 0x1D8u:
      case 0x2D8u:
        m_posjo = &op[-8 * *v403++ - 7];
        *(unsigned int *)op = *(unsigned int *)m_posjo;
        *((_WORD *)op + 2) = *((_WORD *)m_posjo + 2);
        op[6] = m_posjo[6];
        op += 7;
        goto next_state_0;
      case 0xD9u:
      case 0x1D9u:
      case 0x2D9u:
        m_posjp = &op[-8 * *v403 - 7];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjp;
        *((_WORD *)op + 2) = *((_WORD *)m_posjp + 2);
        op[6] = m_posjp[6];
        opa = op + 7;
        goto next_state_1;
      case 0xDAu:
      case 0x1DAu:
      case 0x2DAu:
        m_posjq = &op[-8 * *v403 - 7];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjq;
        *((_WORD *)op + 2) = *((_WORD *)m_posjq + 2);
        op[6] = m_posjq[6];
        opb = op + 7;
        goto next_state_2;
      case 0xDBu:
      case 0x1DBu:
      case 0x2DBu:
        m_posjr = &op[-8 * *v403 - 7];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjr;
        *((_WORD *)op + 2) = *((_WORD *)m_posjr + 2);
        op[6] = m_posjr[6];
        opc = op + 7;
        goto next_state_3;
      case 0xDCu:
      case 0x1DCu:
      case 0x2DCu:
        m_posjs = &op[-8 * *v403++ - 8];
        *(unsigned int *)op = *(unsigned int *)m_posjs;
        *((_WORD *)op + 2) = *((_WORD *)m_posjs + 2);
        op[6] = m_posjs[6];
        op += 7;
        goto next_state_0;
      case 0xDDu:
      case 0x1DDu:
      case 0x2DDu:
        m_posjt = &op[-8 * *v403 - 8];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjt;
        *((_WORD *)op + 2) = *((_WORD *)m_posjt + 2);
        op[6] = m_posjt[6];
        opa = op + 7;
        goto next_state_1;
      case 0xDEu:
      case 0x1DEu:
      case 0x2DEu:
        m_posju = &op[-8 * *v403 - 8];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posju;
        *((_WORD *)op + 2) = *((_WORD *)m_posju + 2);
        op[6] = m_posju[6];
        opb = op + 7;
        goto next_state_2;
      case 0xDFu:
      case 0x1DFu:
      case 0x2DFu:
        m_posjv = &op[-8 * *v403 - 8];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjv;
        *((_WORD *)op + 2) = *((_WORD *)m_posjv + 2);
        op[6] = m_posjv[6];
        opc = op + 7;
        goto next_state_3;
      case 0xE0u:
      case 0x1E0u:
      case 0x2E0u:
        v96 = 8 * *v403;
        m_posdo = &op[-v96 - 1];
        ++v403;
        if ( v96 + 1 < 4 )
        {
          *op = *m_posdo;
          op[1] = op[-v96];
          op[2] = m_posdo[2];
          op[3] = m_posdo[3];
          op[4] = m_posdo[4];
          op[5] = m_posdo[5];
          op[6] = m_posdo[6];
          op[7] = m_posdo[7];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdo;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdo + 1);
        }
        op += 8;
        goto next_state_0;
      case 0xE1u:
      case 0x1E1u:
      case 0x2E1u:
        v97 = 8 * *v403;
        m_posdp = &op[-v97 - 1];
        v404 = v403 + 1;
        if ( v97 + 1 < 4 )
        {
          *op = *m_posdp;
          op[1] = op[-v97];
          op[2] = m_posdp[2];
          op[3] = m_posdp[3];
          op[4] = m_posdp[4];
          op[5] = m_posdp[5];
          op[6] = m_posdp[6];
          op[7] = m_posdp[7];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdp;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdp + 1);
        }
        opa = op + 8;
        goto next_state_1;
      case 0xE2u:
      case 0x1E2u:
      case 0x2E2u:
        v98 = 8 * *v403;
        m_posdq = &op[-v98 - 1];
        v405 = v403 + 1;
        if ( v98 + 1 < 4 )
        {
          *op = *m_posdq;
          op[1] = op[-v98];
          op[2] = m_posdq[2];
          op[3] = m_posdq[3];
          op[4] = m_posdq[4];
          op[5] = m_posdq[5];
          op[6] = m_posdq[6];
          op[7] = m_posdq[7];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdq;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdq + 1);
        }
        opb = op + 8;
        goto next_state_2;
      case 0xE3u:
      case 0x1E3u:
      case 0x2E3u:
        v99 = 8 * *v403;
        m_posdr = &op[-v99 - 1];
        v406 = v403 + 1;
        if ( v99 + 1 < 4 )
        {
          *op = *m_posdr;
          op[1] = op[-v99];
          op[2] = m_posdr[2];
          op[3] = m_posdr[3];
          op[4] = m_posdr[4];
          op[5] = m_posdr[5];
          op[6] = m_posdr[6];
          op[7] = m_posdr[7];
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdr;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdr + 1);
        }
        opc = op + 8;
        goto next_state_3;
      case 0xE4u:
      case 0x1E4u:
      case 0x2E4u:
        v100 = 8 * *v403;
        m_posds = &op[-v100 - 2];
        ++v403;
        if ( v100 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posds;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v100];
          *((_WORD *)op + 2) = *((_WORD *)m_posds + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posds + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posds;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posds + 1);
        }
        op += 8;
        goto next_state_0;
      case 0xE5u:
      case 0x1E5u:
      case 0x2E5u:
        v101 = 8 * *v403;
        m_posdt = &op[-v101 - 2];
        v404 = v403 + 1;
        if ( v101 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdt;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v101];
          *((_WORD *)op + 2) = *((_WORD *)m_posdt + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posdt + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdt;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdt + 1);
        }
        opa = op + 8;
        goto next_state_1;
      case 0xE6u:
      case 0x1E6u:
      case 0x2E6u:
        v102 = 8 * *v403;
        m_posdu = &op[-v102 - 2];
        v405 = v403 + 1;
        if ( v102 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdu;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v102];
          *((_WORD *)op + 2) = *((_WORD *)m_posdu + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posdu + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdu;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdu + 1);
        }
        opb = op + 8;
        goto next_state_2;
      case 0xE7u:
      case 0x1E7u:
      case 0x2E7u:
        v103 = 8 * *v403;
        m_posdv = &op[-v103 - 2];
        v406 = v403 + 1;
        if ( v103 + 2 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdv;
          *((_WORD *)op + 1) = *(_WORD *)&op[-v103];
          *((_WORD *)op + 2) = *((_WORD *)m_posdv + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posdv + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdv;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdv + 1);
        }
        opc = op + 8;
        goto next_state_3;
      case 0xE8u:
      case 0x1E8u:
      case 0x2E8u:
        v104 = 8 * *v403;
        m_posdw = &op[-v104 - 3];
        ++v403;
        if ( v104 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdw;
          *((_WORD *)op + 1) = *((_WORD *)m_posdw + 1);
          *((_WORD *)op + 2) = *((_WORD *)m_posdw + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posdw + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdw;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdw + 1);
        }
        op += 8;
        goto next_state_0;
      case 0xE9u:
      case 0x1E9u:
      case 0x2E9u:
        v105 = 8 * *v403;
        m_posdx = &op[-v105 - 3];
        v404 = v403 + 1;
        if ( v105 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdx;
          *((_WORD *)op + 1) = *((_WORD *)m_posdx + 1);
          *((_WORD *)op + 2) = *((_WORD *)m_posdx + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posdx + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdx;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdx + 1);
        }
        opa = op + 8;
        goto next_state_1;
      case 0xEAu:
      case 0x1EAu:
      case 0x2EAu:
        v106 = 8 * *v403;
        m_posdy = &op[-v106 - 3];
        v405 = v403 + 1;
        if ( v106 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdy;
          *((_WORD *)op + 1) = *((_WORD *)m_posdy + 1);
          *((_WORD *)op + 2) = *((_WORD *)m_posdy + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posdy + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdy;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdy + 1);
        }
        opb = op + 8;
        goto next_state_2;
      case 0xEBu:
      case 0x1EBu:
      case 0x2EBu:
        v107 = 8 * *v403;
        m_posdz = &op[-v107 - 3];
        v406 = v403 + 1;
        if ( v107 + 3 < 4 )
        {
          *(_WORD *)op = *(_WORD *)m_posdz;
          *((_WORD *)op + 1) = *((_WORD *)m_posdz + 1);
          *((_WORD *)op + 2) = *((_WORD *)m_posdz + 2);
          *((_WORD *)op + 3) = *((_WORD *)m_posdz + 3);
        }
        else
        {
          *(unsigned int *)op = *(unsigned int *)m_posdz;
          *((unsigned int *)op + 1) = *((unsigned int *)m_posdz + 1);
        }
        opc = op + 8;
        goto next_state_3;
      case 0xECu:
      case 0x1ECu:
      case 0x2ECu:
        m_posjw = &op[-8 * *v403++ - 4];
        *(unsigned int *)op = *(unsigned int *)m_posjw;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posjw + 1);
        op += 8;
        goto next_state_0;
      case 0xEDu:
      case 0x1EDu:
      case 0x2EDu:
        m_posjx = &op[-8 * *v403 - 4];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjx;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posjx + 1);
        opa = op + 8;
        goto next_state_1;
      case 0xEEu:
      case 0x1EEu:
      case 0x2EEu:
        m_posjy = &op[-8 * *v403 - 4];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjy;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posjy + 1);
        opb = op + 8;
        goto next_state_2;
      case 0xEFu:
      case 0x1EFu:
      case 0x2EFu:
        m_posjz = &op[-8 * *v403 - 4];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posjz;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posjz + 1);
        opc = op + 8;
        goto next_state_3;
      case 0xF0u:
      case 0x1F0u:
      case 0x2F0u:
        m_poska = &op[-8 * *v403++ - 5];
        *(unsigned int *)op = *(unsigned int *)m_poska;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poska + 1);
        op += 8;
        goto next_state_0;
      case 0xF1u:
      case 0x1F1u:
      case 0x2F1u:
        m_poskb = &op[-8 * *v403 - 5];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskb;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskb + 1);
        opa = op + 8;
        goto next_state_1;
      case 0xF2u:
      case 0x1F2u:
      case 0x2F2u:
        m_poskc = &op[-8 * *v403 - 5];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskc;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskc + 1);
        opb = op + 8;
        goto next_state_2;
      case 0xF3u:
      case 0x1F3u:
      case 0x2F3u:
        m_poskd = &op[-8 * *v403 - 5];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskd;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskd + 1);
        opc = op + 8;
        goto next_state_3;
      case 0xF4u:
      case 0x1F4u:
      case 0x2F4u:
        m_poske = &op[-8 * *v403++ - 6];
        *(unsigned int *)op = *(unsigned int *)m_poske;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poske + 1);
        op += 8;
        goto next_state_0;
      case 0xF5u:
      case 0x1F5u:
      case 0x2F5u:
        m_poskf = &op[-8 * *v403 - 6];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskf;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskf + 1);
        opa = op + 8;
        goto next_state_1;
      case 0xF6u:
      case 0x1F6u:
      case 0x2F6u:
        m_poskg = &op[-8 * *v403 - 6];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskg;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskg + 1);
        opb = op + 8;
        goto next_state_2;
      case 0xF7u:
      case 0x1F7u:
      case 0x2F7u:
        m_poskh = &op[-8 * *v403 - 6];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskh;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskh + 1);
        opc = op + 8;
        goto next_state_3;
      case 0xF8u:
      case 0x1F8u:
      case 0x2F8u:
        m_poski = &op[-8 * *v403++ - 7];
        *(unsigned int *)op = *(unsigned int *)m_poski;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poski + 1);
        op += 8;
        goto next_state_0;
      case 0xF9u:
      case 0x1F9u:
      case 0x2F9u:
        m_poskj = &op[-8 * *v403 - 7];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskj;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskj + 1);
        opa = op + 8;
        goto next_state_1;
      case 0xFAu:
      case 0x1FAu:
      case 0x2FAu:
        m_poskk = &op[-8 * *v403 - 7];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskk;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskk + 1);
        opb = op + 8;
        goto next_state_2;
      case 0xFBu:
      case 0x1FBu:
      case 0x2FBu:
        m_poskl = &op[-8 * *v403 - 7];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskl;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskl + 1);
        opc = op + 8;
        goto next_state_3;
      case 0xFCu:
      case 0x1FCu:
      case 0x2FCu:
        m_poskm = &op[-8 * *v403++ - 8];
        *(unsigned int *)op = *(unsigned int *)m_poskm;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskm + 1);
        op += 8;
        goto next_state_0;
      case 0xFDu:
      case 0x1FDu:
      case 0x2FDu:
        m_poskn = &op[-8 * *v403 - 8];
        v404 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskn;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskn + 1);
        opa = op + 8;
        goto next_state_1;
      case 0xFEu:
      case 0x1FEu:
      case 0x2FEu:
        m_posko = &op[-8 * *v403 - 8];
        v405 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_posko;
        *((unsigned int *)op + 1) = *((unsigned int *)m_posko + 1);
        opb = op + 8;
        goto next_state_2;
      case 0xFFu:
      case 0x1FFu:
      case 0x2FFu:
        m_poskp = &op[-8 * *v403 - 8];
        v406 = v403 + 1;
        *(unsigned int *)op = *(unsigned int *)m_poskp;
        *((unsigned int *)op + 1) = *((unsigned int *)m_poskp + 1);
        opc = op + 8;
        goto next_state_3;
      case 0x100u:
        do
          v110 += 255;
        while ( !*v403++ );
        v111 = v110 + *(v403 - 1) - 255;
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        *((unsigned int *)op + 3) = *((unsigned int *)v403 + 3);
        *((_WORD *)op + 8) = *((_WORD *)v403 + 8);
        op += 18;
        for ( v403 += 18; v111 >= 4; v111 -= 4 )
        {
          *(unsigned int *)op = *(unsigned int *)v403;
          op += 4;
          v403 += 4;
        }
        for ( ; v111; --v111 )
          *op++ = *v403++;
        state = 512;
        continue;
      case 0x101u:
        *(unsigned int *)op = *(unsigned int *)v403;
        op += 4;
        v403 += 4;
        state = 512;
        continue;
      case 0x102u:
        *(unsigned int *)op = *(unsigned int *)v403;
        op[4] = v403[4];
        op += 5;
        v403 += 5;
        state = 512;
        continue;
      case 0x103u:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((_WORD *)op + 2) = *((_WORD *)v403 + 2);
        op += 6;
        v403 += 6;
        state = 512;
        continue;
      case 0x104u:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((_WORD *)op + 2) = *((_WORD *)v403 + 2);
        op[6] = v403[6];
        op += 7;
        v403 += 7;
        state = 512;
        continue;
      case 0x105u:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        op += 8;
        v403 += 8;
        state = 512;
        continue;
      case 0x106u:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        op[8] = v403[8];
        op += 9;
        v403 += 9;
        state = 512;
        continue;
      case 0x107u:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((_WORD *)op + 4) = *((_WORD *)v403 + 4);
        op += 10;
        v403 += 10;
        state = 512;
        continue;
      case 0x108u:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((_WORD *)op + 4) = *((_WORD *)v403 + 4);
        op[10] = v403[10];
        op += 11;
        v403 += 11;
        state = 512;
        continue;
      case 0x109u:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        op += 12;
        v403 += 12;
        state = 512;
        continue;
      case 0x10Au:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        op[12] = v403[12];
        op += 13;
        v403 += 13;
        state = 512;
        continue;
      case 0x10Bu:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        *((_WORD *)op + 6) = *((_WORD *)v403 + 6);
        op += 14;
        v403 += 14;
        state = 512;
        continue;
      case 0x10Cu:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        *((_WORD *)op + 6) = *((_WORD *)v403 + 6);
        op[14] = v403[14];
        op += 15;
        v403 += 15;
        state = 512;
        continue;
      case 0x10Du:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        *((unsigned int *)op + 3) = *((unsigned int *)v403 + 3);
        op += 16;
        v403 += 16;
        state = 512;
        continue;
      case 0x10Eu:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        *((unsigned int *)op + 3) = *((unsigned int *)v403 + 3);
        op[16] = v403[16];
        op += 17;
        v403 += 17;
        state = 512;
        continue;
      case 0x10Fu:
        *(unsigned int *)op = *(unsigned int *)v403;
        *((unsigned int *)op + 1) = *((unsigned int *)v403 + 1);
        *((unsigned int *)op + 2) = *((unsigned int *)v403 + 2);
        *((unsigned int *)op + 3) = *((unsigned int *)v403 + 3);
        *((_WORD *)op + 8) = *((_WORD *)v403 + 8);
        op += 18;
        v403 += 18;
        state = 512;
        continue;
      case 0x200u:
        m_posea = &op[-4 * *v403++ - 2049];
        *(_WORD *)op = *(_WORD *)m_posea;
        op[2] = m_posea[2];
        op += 3;
        goto next_state_0;
      case 0x201u:
        m_poseb = &op[-4 * *v403 - 2049];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_poseb;
        op[2] = m_poseb[2];
        opa = op + 3;
        goto next_state_1;
      case 0x202u:
        m_posec = &op[-4 * *v403 - 2049];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posec;
        op[2] = m_posec[2];
        opb = op + 3;
        goto next_state_2;
      case 0x203u:
        m_posed = &op[-4 * *v403 - 2049];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posed;
        op[2] = m_posed[2];
        opc = op + 3;
        goto next_state_3;
      case 0x204u:
        m_posee = &op[-4 * *v403++ - 2050];
        *(_WORD *)op = *(_WORD *)m_posee;
        op[2] = m_posee[2];
        op += 3;
        goto next_state_0;
      case 0x205u:
        m_posef = &op[-4 * *v403 - 2050];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posef;
        op[2] = m_posef[2];
        opa = op + 3;
        goto next_state_1;
      case 0x206u:
        m_poseg = &op[-4 * *v403 - 2050];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_poseg;
        op[2] = m_poseg[2];
        opb = op + 3;
        goto next_state_2;
      case 0x207u:
        m_poseh = &op[-4 * *v403 - 2050];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_poseh;
        op[2] = m_poseh[2];
        opc = op + 3;
        goto next_state_3;
      case 0x208u:
        m_posei = &op[-4 * *v403++ - 2051];
        *(_WORD *)op = *(_WORD *)m_posei;
        op[2] = m_posei[2];
        op += 3;
        goto next_state_0;
      case 0x209u:
        m_posej = &op[-4 * *v403 - 2051];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posej;
        op[2] = m_posej[2];
        opa = op + 3;
        goto next_state_1;
      case 0x20Au:
        m_posek = &op[-4 * *v403 - 2051];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posek;
        op[2] = m_posek[2];
        opb = op + 3;
        goto next_state_2;
      case 0x20Bu:
        m_posel = &op[-4 * *v403 - 2051];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posel;
        op[2] = m_posel[2];
        opc = op + 3;
        goto next_state_3;
      case 0x20Cu:
        m_posem = &op[-4 * *v403++ - 2052];
        *(_WORD *)op = *(_WORD *)m_posem;
        op[2] = m_posem[2];
        op += 3;
next_state_0:
        state = 256;
        continue;
      case 0x20Du:
        m_posen = &op[-4 * *v403 - 2052];
        v404 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posen;
        op[2] = m_posen[2];
        opa = op + 3;
next_state_1:
        *opa = *v404;
        op = opa + 1;
        v403 = v404 + 1;
        state = 0;
        continue;
      case 0x20Eu:
        m_poseo = &op[-4 * *v403 - 2052];
        v405 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_poseo;
        op[2] = m_poseo[2];
        opb = op + 3;
next_state_2:
        *(_WORD *)opb = *(_WORD *)v405;
        op = opb + 2;
        v403 = v405 + 2;
        state = 0;
        continue;
      case 0x20Fu:
        m_posep = &op[-4 * *v403 - 2052];
        v406 = v403 + 1;
        *(_WORD *)op = *(_WORD *)m_posep;
        op[2] = m_posep[2];
        opc = op + 3;
next_state_3:
        *(_WORD *)opc = *(_WORD *)v406;
        opc[2] = v406[2];
        op = opc + 3;
        v403 = v406 + 3;
        state = 0;
        continue;
      default:
        goto next_state;
    }
  }
}

