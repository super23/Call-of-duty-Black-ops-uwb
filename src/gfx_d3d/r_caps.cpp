#include "r_caps.h"

void __cdecl R_CheckDxCaps(const _D3DCAPS9 *caps)
{
  unsigned int integer; // [esp+0h] [ebp-Ch]
  unsigned int checkIndex; // [esp+4h] [ebp-8h]
  unsigned int checkIndexa; // [esp+4h] [ebp-8h]
  unsigned int bits; // [esp+8h] [ebp-4h]

  for ( checkIndex = 0; checkIndex < 0x21; ++checkIndex )
  {
    bits = *(_D3DDEVTYPE *)((char *)&caps->DeviceType + s_capsCheckBits[checkIndex].offset);
    if ( (!s_capsCheckBits[checkIndex].clearBits || (s_capsCheckBits[checkIndex].clearBits & ~bits) != 0)
      && (!s_capsCheckBits[checkIndex].setBits || (s_capsCheckBits[checkIndex].setBits & bits) != 0) )
    {
      R_RespondToMissingCaps(s_capsCheckBits[checkIndex].response, s_capsCheckBits[checkIndex].msg);
    }
  }
  for ( checkIndexa = 0; checkIndexa < 0xA; ++checkIndexa )
  {
    integer = *(_D3DDEVTYPE *)((char *)&caps->DeviceType + s_capsCheckInt[checkIndexa].offset);
    if ( integer < s_capsCheckInt[checkIndexa].min || integer > s_capsCheckInt[checkIndexa].max )
      R_RespondToMissingCaps(s_capsCheckInt[checkIndexa].response, s_capsCheckInt[checkIndexa].msg);
  }
}

void __cdecl R_RespondToMissingCaps(DxCapsResponse response, const char *msg)
{
  const char *v2; // eax

  if ( response == DX_CAPS_RESPONSE_WARN )
    Com_PrintWarning(8, "Video card or driver %s.\n", msg);
  else
    Com_Printf(8, "Video card or driver %s.\n", msg);
  switch ( response )
  {
    case DX_CAPS_RESPONSE_QUIT:
      Com_Error(ERR_FATAL, "Video card or driver %s.\n", msg);
      break;
    case DX_CAPS_RESPONSE_WARN:
    case DX_CAPS_RESPONSE_INFO:
      return;
    case DX_CAPS_RESPONSE_FORBID_SM3:
      Com_Error(ERR_FATAL, "Shader model 3.0 not available.\n");
      break;
    default:
      v2 = va("unhandled response %i", response);
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_caps.cpp", 161, 1, v2) )
        __debugbreak();
      break;
  }
}

