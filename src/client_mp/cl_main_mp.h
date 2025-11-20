#pragma once

void __cdecl CL_AddReliableCommand(int localClientNum, const char *cmd);
void __cdecl CL_AnimateUI_f();
void __cdecl CL_ShowContextualItemUI_f();
void __cdecl CL_ToggleDemoRecording_f();
void __cdecl CL_WriteDemoEpilog(int localClientNum);
void __cdecl CL_WriteDemoClientArchive(int demoFile, unsigned __int8 msgType, ClientArchiveData *archive, int index);
void __cdecl CL_WriteDemoFromContinuousStateBuffer(int localClientNum, int continuousStateBufferIndex);
void __cdecl CL_StopRecord_f();
void __cdecl CL_Record_f();
void __cdecl CL_WriteInitialStateToDemo(
        int demoFile,
        unsigned __int8 type,
        int serverMessageSequence,
        unsigned __int8 *compressedBuf,
        int compressedBufSize);
void __cdecl CL_WriteUncompressedDemoInfo(int localClientNum);
void __cdecl CL_WriteAllDemoClientArchive(int localClientNum);
void __cdecl CL_WriteAllDemoClientArchiveToMemory(int localClientNum);
void __cdecl CL_WriteDemoClientArchiveToMemory(const clientActive_t *cl, ClientArchiveData *archive, int index);
void __cdecl CL_ReadDemoMessage(int localClientNum);
void __cdecl CL_DemoCompleted(int localClientNum);
void __cdecl CL_ReadDemoClientArchive(int localClientNum);
void __cdecl CL_ReadDemoNetworkPacket(int localClientNum);
void __cdecl CL_NextDemo(int localClientNum);
void __cdecl CL_ShutdownDevGui();
void __cdecl CL_ShutdownHunkUsers();
void __cdecl CL_ShutdownAll();
void __cdecl CL_ShutdownDemo();
void __cdecl CL_MapLoading(const char *mapname);
void __cdecl CL_ResetSkeletonCache(unsigned int localClientNum);
void __cdecl CL_ClearStaticDownload();
void __cdecl CL_ClearState(int localClientNum);
void __cdecl CL_UploadStatsForController(int localControllerIndex);
void __cdecl CL_Disconnect(unsigned int localClientNum, bool deactivateClient);
void __cdecl CL_UpdateProfileAfterGame(int controllerIndex);
void __cdecl CL_CanNotConnect(int localClientNum, const char *msg);
void __cdecl CL_ForwardCommandToServer(int localClientNum, const char *string);
void __cdecl CL_ForwardToServer_f();
void __cdecl CL_Setenv_f();
void __cdecl CL_DisconnectLocalClient(int localClientNum);
void __cdecl CL_Reconnect_f();
void __cdecl CL_SendPureChecksums(int localClientNum);
void __cdecl CL_Vid_Restart_f();
void __cdecl CL_Snd_Restart_f();
void __cdecl CL_OpenedIWDList_f();
void __cdecl CL_ReferencedIWDList_f();
void __cdecl CL_Configstrings_f();
void __cdecl CL_Clientinfo_f();
bool __cdecl CL_WasMapAlreadyLoaded();
void __cdecl CL_DownloadsComplete(int localClientNum);
void __cdecl LoadMapLoadscreen(char *mapname);
void __cdecl CL_BeginDownload(const char *localName, const char *remoteName);
void __cdecl CL_NextDownload(int localClientNum);
void __cdecl CL_InitDownloads(int localClientNum);
void __cdecl CL_CheckForResend(int localClientNum);
void __cdecl CL_BuildMd5StrFromCDKey(char *md5Str);
char *__cdecl strtrm(char *str);
void __cdecl CL_DisconnectError(const char *message);
bool __cdecl CL_HandleVoiceTypePacket(int localClientNum, msg_t *msg);
char __cdecl CL_ConnectionlessPacket(int localClientNum, netadr_t from, msg_t *msg, int time);
char  CL_DispatchConnectionlessPacket@<al>(int a1@<esi>, int localClientNum, netadr_t from, msg_t *msg);
void __cdecl CL_DisconnectPacket(int localClientNum, netadr_t from, const char *reason);
void __cdecl CL_SteamServerAuth(unsigned __int64 serverSteamID);
void __cdecl CL_AllocatePerLocalClientMemory();
void __cdecl AllocatePerLocalClientMemory(int maxLocalClients, int maxClients, unsigned int flags);
void __cdecl CL_FreePerLocalClientMemory();
void  CL_InitLoad(int a1@<esi>, char *mapname, const char *gametype);
bool __cdecl CL_PacketEvent(int localClientNum, netadr_t from, msg_t *msg, int time, bool connectionlesss_packets_only);
void __cdecl CL_WriteNewDemoClientArchive(int localClientNum);
void __cdecl CL_WriteNewDemoClientArchiveToMemory(int localClientNum);
unsigned __int8 *__cdecl CL_DemoMemoryPoolAllocate(clientConnection_t *clc, int size);
void __cdecl CL_DemoMemoryPoolDeallocate(clientConnection_t *clc, int size);
int __cdecl CL_GetClientArchiveCount(int start, int end);
void __cdecl CL_WriteDemoMessage(int localClientNum, msg_t *msg, int headerBytes);
void __cdecl CL_WriteDemoMessageToMemory(int localClientNum, msg_t *msg, int headerBytes);
void __cdecl CL_VoiceTransmit(int localClientNum);
voiceCommunication_t *__cdecl CL_GetLocalClientVoiceCommunication(int localClientNum);
int __cdecl Client_SendVoiceData(int bytes, char *enc_buffer);
char __cdecl Voice_SendVoiceData();
void __cdecl CL_RunOncePerClientFrame(int localClientNum, int msec);
void __cdecl CL_Frame(int localClientNum, int msec);
void __cdecl CL_CheckTimeout(int localClientNum);
void __cdecl CL_ServerTimedOut(int localClientNum);
void __cdecl CL_CheckUserinfo(int localClientNum);
void __cdecl CL_UpdateInGameState(int localClientNum);
void __cdecl CL_VoiceFrame(int localClientNum);
char __cdecl CL_IsClientLocal(int clientNum);
void __cdecl CL_ParseBadPacket_f();
void __cdecl CL_ShutdownRef();
void __cdecl CL_ShutdownWorld();
void __cdecl CL_SetupViewport();
void __cdecl CL_InitRenderer();
void __cdecl CL_ShutdownRenderer(int destroyWindow);
void __cdecl CL_StartHunkUsers();
void CL_InitDevGui();
void __cdecl CL_DevGuiDvar_f();
void __cdecl CL_DevGuiCmd_f();
void __cdecl CL_DevGuiOpen_f();
int __cdecl CL_ScaledMilliseconds();
void __cdecl CL_InitRef();
void __cdecl SetupGfxConfig(GfxConfiguration *config);
void __cdecl CL_InitDedicated();
void __cdecl CL_startSingleplayer_f();
void __cdecl CL_DrawLogo(int localClientNum);
void __cdecl CL_StopLogo(int localClientNum);
void __cdecl CL_StopLogoOrCinematic(int localClientNum);
void __cdecl CL_ToggleMenu_f();
bool __cdecl CL_ShouldAllowInGameMenu(int localClientNum);
char __cdecl Playlist_ReadFromDisk();
void __cdecl CL_PauseGame(bool state);
void __cdecl CL_InitOnceForAllClients();
void __cdecl CL_PlayDemo_f();
void __cdecl CL_Disconnect_f();
void __cdecl CL_SetupViewport_f();
void __cdecl CL_PlayLogo_f();
void __cdecl CL_OpenScriptMenu_f();
void __cdecl Playlist_CmdReadFromDisk();
void __cdecl Playlist_CmdSetCategoryFilter();
void __cdecl Playlist_CmdSetPrevPlaylist();
void __cdecl Playlist_CmdGetPrevPlaylist();
void __cdecl CL_CmdGetOldCustomName();
void __cdecl CL_CmdSetNewCustomName();
void __cdecl CL_ResetStats_f();
void __cdecl CL_Init(int localClientNum);
int __cdecl CountBitsEnabled(unsigned int num);
void __cdecl CL_Shutdown(int localClientNum);
bool __cdecl CL_GetServerList(int source, serverInfo_t **servers, int **count);
int __cdecl CL_UpdateDirtyPings(int localClientNum, unsigned int source);
void __cdecl CL_ShowIP_f();
char *__cdecl CL_GetServerIPAddress();
void  CL_SetupForNewServerMap(int a1@<esi>, int localClientNum, char *pszMapName, const char *pszGametype);
bool __cdecl CL_IsServerLoadingMap();
bool __cdecl CL_IsWaitingOnServerToLoadMap(int localClientNum);
void __cdecl CL_SetWaitingOnServerToLoadMap(int localClientNum, bool waiting);
void __cdecl CL_SyncGpu();
void __cdecl CL_DrawTextPhysical(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float w,
        float xScale,
        float yScale,
        const float *color,
        int style);
void __cdecl CL_DrawTextPhysicalWithEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float w,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl CL_DrawText(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float xScale,
        float yScale,
        const float *color,
        int style);
void __cdecl CL_DrawTextWithEffects(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float rotation,
        int horzAlign,
        int vertAlign,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl CL_DrawTextWithCOD7TypeWriterEffects(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float rotation,
        int horzAlign,
        int vertAlign,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl CL_DrawTextWithRedactEffects(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        int fxRedactDecayStartTime,
        int fxRedactDecayDuration);
void __cdecl CL_DrawTextPhysicalWithCOD7DecodeEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl CL_DrawTextPhysicalWithRedactEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        int fxRedactDecayStartTime,
        int fxRedactDecayDuration);
void __cdecl CL_DrawTextWithPopInEffects(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float rotation,
        int horzAlign,
        int vertAlign,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl CL_DrawTextRotate(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float rotation,
        int horzAlign,
        int vertAlign,
        float xScale,
        float yScale,
        const float *color,
        int style);
void __cdecl CL_DrawTextPhysicalWithCursor(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style,
        int cursorPos,
        char cursor);
void __cdecl CL_DrawTextWithCursor(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float xScale,
        float yScale,
        const float *color,
        int style,
        int cursorPos,
        char cursor);
bool __cdecl CL_ShouldDisplayHud(int localClientNum);
bool __cdecl CL_IsUIActive(int localClientNum);
Font_s *__cdecl CL_RegisterFont(const char *fontName, int imageTrack);
float (*__cdecl CL_GetMapCenter())[3];
void __cdecl CL_SanitizeClanName();
int __cdecl CL_FilterChar(unsigned __int8 input);
bdTrulyRandomImpl *__cdecl bdSingleton<bdTrulyRandomImpl>::getInstance();
void bdSingleton<bdTrulyRandomImpl>::destroyInstance();
bdSingletonRegistryImpl *__cdecl bdSingleton<bdSingletonRegistryImpl>::getInstance();
bdSingletonRegistryImpl *__thiscall bdSingletonRegistryImpl::bdSingletonRegistryImpl(bdSingletonRegistryImpl *this);
void bdSingleton<bdSingletonRegistryImpl>::destroyInstance();
clientStatic_t *__thiscall clientStatic_t::clientStatic_t(clientStatic_t *this);
bool __cdecl CL_IsLocalClientInGame(int localClientNum);
