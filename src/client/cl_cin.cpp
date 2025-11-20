#include "cl_cin.h"

void __cdecl SCR_DrawCinematic()
{
  if ( R_Cinematic_IsNextReady() )
    R_Cinematic_StartNextPlayback();
  if ( R_Cinematic_IsFinished() )
  {
    if ( Dvar_GetBool("com_movieIsPlaying") )
      Dvar_SetBool((dvar_s *)com_movieIsPlaying, 0);
    SCR_StopCinematic();
  }
  if ( R_Cinematic_IsStarted() && !Dvar_GetBool("com_movieIsPlaying") )
    Dvar_SetBool((dvar_s *)com_movieIsPlaying, 1);
}

void __cdecl SCR_StopCinematic()
{
  const char *v0; // eax
  int clientNum; // [esp+0h] [ebp-8h]
  bool oneClientWasChanged; // [esp+7h] [ebp-1h]

  if ( cin_skippable || R_Cinematic_IsFinished() )
  {
    R_Cinematic_StopPlayback();
    oneClientWasChanged = 0;
    for ( clientNum = 0; clientNum < 1; ++clientNum )
    {
      if ( CL_GetLocalClientConnectionState(clientNum) == 1 || CL_GetLocalClientConnectionState(clientNum) == 2 )
      {
        oneClientWasChanged = 1;
        CL_SetLocalClientConnectionState(clientNum, CA_DISCONNECTED);
      }
    }
    if ( oneClientWasChanged && *(_BYTE *)nextmap->current.integer )
    {
      v0 = va("%s\n", nextmap->current.string);
      Cbuf_AddText(0, v0);
      Dvar_SetString((dvar_s *)nextmap, &toastPopupTitle);
    }
  }
}

