#include "live_presence_win.h"

int __cdecl Live_GetMapCount()
{
  return 9;
}

void __cdecl Live_SetPlaylistVersion(int controllerIndex)
{
  int playlistVersion; // [esp+0h] [ebp-4h]

  if ( dw_loggedin && dw_loggedin->current.enabled )
  {
    playlistVersion = Playlist_GetVersionNumber();
    if ( g_matchmakingInfo->m_memberPLAYLIST_VERSION != playlistVersion )
    {
      Com_Printf(16, "Setting playlist version to %i for controller %i\n", playlistVersion, controllerIndex);
      g_matchmakingInfo->m_memberPLAYLIST_VERSION = playlistVersion;
      g_matchmakingInfo->m_dirty = 1;
    }
  }
}

void __cdecl Live_SetPlaylistNum(int controllerIndex, int playlistNum)
{
  int PlaylistIdForNum; // eax

  if ( dw_loggedin && dw_loggedin->current.enabled )
  {
    if ( g_matchmakingInfo->m_memberPLAYLIST != Playlist_GetPlaylistIdForNum(controllerIndex, playlistNum) )
    {
      PlaylistIdForNum = Playlist_GetPlaylistIdForNum(controllerIndex, playlistNum);
      Com_Printf(16, "Setting playlistId to %i for controller %i\n", PlaylistIdForNum, controllerIndex);
      g_matchmakingInfo->m_memberPLAYLIST = Playlist_GetPlaylistIdForNum(controllerIndex, playlistNum);
      g_matchmakingInfo->m_dirty = 1;
    }
    if ( g_matchmakingInfo->m_memberGAME_MODE != playlistNum )
    {
      Com_Printf(16, "Setting GAME_MODE to %i for controller %i\n", playlistNum, controllerIndex);
      g_matchmakingInfo->m_memberGAME_MODE = playlistNum;
      g_matchmakingInfo->m_dirty = 1;
    }
  }
}

int __cdecl Live_GetMapIndex()
{
  int mapIndex; // [esp+0h] [ebp-4h]

  for ( mapIndex = 0; mapRichPresenceInfo[mapIndex].mapname[0]; ++mapIndex )
    ;
  return -1;
}

int __cdecl Live_GetMapSource(int mapIndex)
{
  if ( mapIndex < 0 )
    return 2;
  if ( mapIndex >= 9
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_presence_win.cpp",
          210,
          0,
          "%s\n\t(mapIndex) = %i",
          "(mapIndex >= 0 && mapIndex < static_cast< int >( ((sizeof( mapRichPresenceInfo ) / (sizeof( mapRichPresenceInf"
          "o[0] ) * (sizeof( mapRichPresenceInfo ) != 4 || sizeof( mapRichPresenceInfo[0] ) <= 4))) - 1) ))",
          mapIndex) )
  {
    __debugbreak();
  }
  return mapRichPresenceInfo[mapIndex].mapPack;
}

