#include "adler32.h"

unsigned int __cdecl adler32(unsigned int adler, const unsigned __int8 *buf, unsigned int len)
{
  unsigned int sum2; // [esp+0h] [ebp-8h]
  unsigned int sum2a; // [esp+0h] [ebp-8h]
  unsigned int sum2b; // [esp+0h] [ebp-8h]
  unsigned int sum2c; // [esp+0h] [ebp-8h]
  unsigned int sum2d; // [esp+0h] [ebp-8h]
  unsigned int sum2e; // [esp+0h] [ebp-8h]
  unsigned int sum2f; // [esp+0h] [ebp-8h]
  unsigned int sum2g; // [esp+0h] [ebp-8h]
  unsigned int sum2h; // [esp+0h] [ebp-8h]
  unsigned int sum2i; // [esp+0h] [ebp-8h]
  unsigned int sum2j; // [esp+0h] [ebp-8h]
  unsigned int sum2k; // [esp+0h] [ebp-8h]
  unsigned int sum2l; // [esp+0h] [ebp-8h]
  unsigned int sum2m; // [esp+0h] [ebp-8h]
  unsigned int sum2n; // [esp+0h] [ebp-8h]
  unsigned int sum2o; // [esp+0h] [ebp-8h]
  unsigned int sum2p; // [esp+0h] [ebp-8h]
  unsigned int sum2q; // [esp+0h] [ebp-8h]
  unsigned int sum2r; // [esp+0h] [ebp-8h]
  unsigned int sum2s; // [esp+0h] [ebp-8h]
  unsigned int sum2t; // [esp+0h] [ebp-8h]
  unsigned int sum2u; // [esp+0h] [ebp-8h]
  unsigned int sum2v; // [esp+0h] [ebp-8h]
  unsigned int sum2w; // [esp+0h] [ebp-8h]
  unsigned int sum2x; // [esp+0h] [ebp-8h]
  unsigned int sum2y; // [esp+0h] [ebp-8h]
  unsigned int sum2z; // [esp+0h] [ebp-8h]
  unsigned int sum2ba; // [esp+0h] [ebp-8h]
  unsigned int sum2bb; // [esp+0h] [ebp-8h]
  unsigned int sum2bc; // [esp+0h] [ebp-8h]
  unsigned int sum2bd; // [esp+0h] [ebp-8h]
  unsigned int sum2be; // [esp+0h] [ebp-8h]
  unsigned int n; // [esp+4h] [ebp-4h]
  unsigned int adlera; // [esp+10h] [ebp+8h]
  unsigned int adlerb; // [esp+10h] [ebp+8h]
  unsigned int adlerc; // [esp+10h] [ebp+8h]
  unsigned int adlerd; // [esp+10h] [ebp+8h]
  unsigned int adlere; // [esp+10h] [ebp+8h]
  unsigned int adlerf; // [esp+10h] [ebp+8h]
  unsigned int adlerg; // [esp+10h] [ebp+8h]
  unsigned int adlerh; // [esp+10h] [ebp+8h]
  unsigned int adleri; // [esp+10h] [ebp+8h]
  unsigned int adlerj; // [esp+10h] [ebp+8h]
  unsigned int adlerk; // [esp+10h] [ebp+8h]
  unsigned int adlerl; // [esp+10h] [ebp+8h]
  unsigned int adlerm; // [esp+10h] [ebp+8h]
  unsigned int adlern; // [esp+10h] [ebp+8h]
  unsigned int adlero; // [esp+10h] [ebp+8h]
  unsigned int adlerp; // [esp+10h] [ebp+8h]
  unsigned int adlerq; // [esp+10h] [ebp+8h]
  unsigned int adlerr; // [esp+10h] [ebp+8h]
  unsigned int adlers; // [esp+10h] [ebp+8h]
  unsigned int adlert; // [esp+10h] [ebp+8h]
  unsigned int adleru; // [esp+10h] [ebp+8h]
  unsigned int adlerv; // [esp+10h] [ebp+8h]
  unsigned int adlerw; // [esp+10h] [ebp+8h]
  unsigned int adlerx; // [esp+10h] [ebp+8h]
  unsigned int adlery; // [esp+10h] [ebp+8h]
  unsigned int adlerz; // [esp+10h] [ebp+8h]
  unsigned int adlerba; // [esp+10h] [ebp+8h]
  unsigned int adlerbb; // [esp+10h] [ebp+8h]
  unsigned int adlerbc; // [esp+10h] [ebp+8h]
  unsigned int adlerbd; // [esp+10h] [ebp+8h]
  unsigned int adlerbe; // [esp+10h] [ebp+8h]
  unsigned int adlerbf; // [esp+10h] [ebp+8h]

  sum2 = HIWORD(adler);
  adlera = (unsigned __int16)adler;
  if ( len == 1 )
  {
    adlerb = (unsigned __int16)adlera + *buf;
    if ( adlerb >= 0xFFF1 )
      adlerb -= 65521;
    sum2a = adlerb + sum2;
    if ( sum2a >= 0xFFF1 )
      sum2a -= 65521;
    return adlerb | (sum2a << 16);
  }
  else if ( buf )
  {
    if ( len >= 0x10 )
    {
      while ( len >= 0x15B0 )
      {
        len -= 5552;
        n = 347;
        do
        {
          adlerc = adlera + *buf;
          sum2b = adlerc + sum2;
          adlerd = adlerc + buf[1];
          sum2c = adlerd + sum2b;
          adlere = adlerd + buf[2];
          sum2d = adlere + sum2c;
          adlerf = adlere + buf[3];
          sum2e = adlerf + sum2d;
          adlerg = adlerf + buf[4];
          sum2f = adlerg + sum2e;
          adlerh = adlerg + buf[5];
          sum2g = adlerh + sum2f;
          adleri = adlerh + buf[6];
          sum2h = adleri + sum2g;
          adlerj = adleri + buf[7];
          sum2i = adlerj + sum2h;
          adlerk = adlerj + buf[8];
          sum2j = adlerk + sum2i;
          adlerl = adlerk + buf[9];
          sum2k = adlerl + sum2j;
          adlerm = adlerl + buf[10];
          sum2l = adlerm + sum2k;
          adlern = adlerm + buf[11];
          sum2m = adlern + sum2l;
          adlero = adlern + buf[12];
          sum2n = adlero + sum2m;
          adlerp = adlero + buf[13];
          sum2o = adlerp + sum2n;
          adlerq = adlerp + buf[14];
          sum2p = adlerq + sum2o;
          adlera = adlerq + buf[15];
          sum2 = adlera + sum2p;
          buf += 16;
          --n;
        }
        while ( n );
        adlera %= 0xFFF1u;
        sum2 %= 0xFFF1u;
      }
      if ( len )
      {
        while ( len >= 0x10 )
        {
          len -= 16;
          adlerr = adlera + *buf;
          sum2q = adlerr + sum2;
          adlers = adlerr + buf[1];
          sum2r = adlers + sum2q;
          adlert = adlers + buf[2];
          sum2s = adlert + sum2r;
          adleru = adlert + buf[3];
          sum2t = adleru + sum2s;
          adlerv = adleru + buf[4];
          sum2u = adlerv + sum2t;
          adlerw = adlerv + buf[5];
          sum2v = adlerw + sum2u;
          adlerx = adlerw + buf[6];
          sum2w = adlerx + sum2v;
          adlery = adlerx + buf[7];
          sum2x = adlery + sum2w;
          adlerz = adlery + buf[8];
          sum2y = adlerz + sum2x;
          adlerba = adlerz + buf[9];
          sum2z = adlerba + sum2y;
          adlerbb = adlerba + buf[10];
          sum2ba = adlerbb + sum2z;
          adlerbc = adlerbb + buf[11];
          sum2bb = adlerbc + sum2ba;
          adlerbd = adlerbc + buf[12];
          sum2bc = adlerbd + sum2bb;
          adlerbe = adlerbd + buf[13];
          sum2bd = adlerbe + sum2bc;
          adlerbf = adlerbe + buf[14];
          sum2be = adlerbf + sum2bd;
          adlera = adlerbf + buf[15];
          sum2 = adlera + sum2be;
          buf += 16;
        }
        while ( len-- )
        {
          adlera += *buf++;
          sum2 += adlera;
        }
        adlera %= 0xFFF1u;
        sum2 %= 0xFFF1u;
      }
      return adlera | (sum2 << 16);
    }
    else
    {
      while ( len-- )
      {
        adlera += *buf++;
        sum2 += adlera;
      }
      if ( adlera >= 0xFFF1 )
        adlera -= 65521;
      return adlera | ((sum2 % 0xFFF1) << 16);
    }
  }
  else
  {
    return 1;
  }
}

