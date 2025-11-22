#pragma once
#include "msg_mp.h"
#include "net_chan_mp.h"

enum errorParm_t : __int32
{                                       // XREF: ?Com_Error@@YAXW4errorParm_t@@PBDZZ/r
    ERR_FATAL            = 0x0,
    ERR_DROP             = 0x1,
    ERR_SERVERDISCONNECT = 0x2,
    ERR_DISCONNECT       = 0x3,
    ERR_SCRIPT           = 0x4,
    ERR_SCRIPT_DROP      = 0x5,
    ERR_LOCALIZATION     = 0x6,
};

struct __declspec(align(8)) SysInfo // sizeof=0x260
{                                       // XREF: .data:sys_info/r
                                        // ?Com_SetRecommended@@YAXHH@Z/r
    long double cpuGHz;                 // XREF: Sys_RegisterInfoDvars+DA/r
                                        // Sys_Init(void)+1B7/r ...
    long double configureGHz;           // XREF: Com_SetRecommended(int,int)+31/r
                                        // Com_SetRecommended(int,int)+41/w ...
    int logicalCpuCount;                // XREF: Sys_Init(void)+13F/r
                                        // Sys_Init(void)+165/r ...
    int physicalCpuCount;               // XREF: Sys_Init(void)+17B/r
                                        // Sys_Init(void)+1A1/r
    int sysMB;                          // XREF: Com_SetRecommended(int,int)+49/r
                                        // Com_SetRecommended(int,int)+55/w ...
    char gpuDescription[512];           // XREF: Com_SetRecommended(int,int)+125/o
                                        // Sys_HasInfoChanged(void):loc_7D9644/o ...
    bool SSE;                           // XREF: Sys_RegisterInfoDvars+A5/r
                                        // Sys_Init(void)+211/r ...
    char cpuVendor[13];                 // XREF: Sys_Init(void):loc_7DA2D7/o
                                        // Sys_FindInfo+90/o
    char cpuName[49];                   // XREF: Sys_RegisterInfoDvars+FC/o
                                        // Sys_Init(void)+12B/o ...
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct field_t // sizeof=0x118
{                                       // XREF: .data:field_t g_consoleField/r
                                        // .data:field_t * historyEditLines/r ...
    int cursor;                         // XREF: Con_Echo_f+50/w
                                        // Con_CommitToAutoComplete(void)+C5/w ...
    int scroll;
    int drawWidth;                      // XREF: Con_CommitToAutoComplete(void)+FF/w
    int widthInPixels;                  // XREF: Con_ToggleConsole(void)+20/w
                                        // Con_Init(void)+67/w ...
    float charHeight;                   // XREF: Con_ToggleConsole(void)+2D/w
                                        // Con_Init(void)+74/w ...
    int fixedSize;                      // XREF: Con_ToggleConsole(void)+35/w
                                        // Con_Init(void)+7C/w ...
    char buffer[256];                   // XREF: Con_Echo_f+15/o
                                        // Con_Echo_f+22/o ...
};

struct XAnim_s;
struct clientState_s;
struct XAnimTree_s;

void *__cdecl Com_LiveAllocate(unsigned int size);
void __cdecl Com_LiveDeallocate(void *data);
unsigned __int8 *__cdecl Com_LiveRealloc(unsigned __int8 *ptr, unsigned int size);
unsigned __int8 *__cdecl Com_LiveCalloc(unsigned int num, unsigned int size);
bool __cdecl Com_IsRunningMenuLevel(const char *name);
bool __cdecl Com_IsMenuLevel(const char *name);
void __cdecl Com_BeginRedirect(char *buffer, unsigned int buffersize, void (__cdecl *flush)(char *));
void __cdecl Com_EndRedirect();
void __cdecl Com_PrintMessage(int channel, char *msg, int error);
void __cdecl Com_LogPrintMessage(int channel, char *msg);
void Com_OpenLogFile();
void Com_Printf(int channel, const char *fmt, ...);
void Com_DPrintf(int channel, const char *fmt, ...);
void Com_PrintError(int channel, const char *fmt, ...);
void Com_PrintWarning(int channel, const char *fmt, ...);
void __cdecl Com_Shutdown(const char *finalmsg);
void __cdecl Com_ShutdownInternal(const char *finalmsg);
void __cdecl Com_InitDynamicRender();
void __cdecl Com_InitDynamicMemorySystems();
void __cdecl Com_ShutdownDynamicMemorySystems();
double __cdecl Com_GetTimeScale();
void __cdecl Com_SetTimeScale(float timescale);
void __cdecl Com_SetLocalizedErrorMessage(const char *localizedErrorMessage, const char *titleToken);
void __cdecl Com_SetErrorMessage(char *errorMessage);
char __cdecl Com_ErrorIsNotice(const char *errorMessage);
void __cdecl Com_PrintStackTrace();
void __cdecl  Com_ErrorAbort();
void Com_Error(errorParm_t code, const char *fmt, ...);
void __cdecl Com_CheckError();
void __cdecl  Com_Quit_f();
void Com_ClearTempMemory();
int __cdecl Com_SafeMode();
void __cdecl Com_ForceSafeMode();
void __cdecl Com_StartupVariable(const char *match);
bool __cdecl Com_StartupProcessSetCommand(int lineIndex, const char *match);
void __cdecl Info_Print(const char *s);
unsigned int *__cdecl Com_AllocEvent(int size);
void __cdecl Com_FreeEvent(char *ptr);
void __cdecl Com_ServerPacketEvent();
void __cdecl Com_EventLoop();
void Com_ClientPacketEvent();
void __cdecl Com_PacketEventLoop(int localClientNum, msg_t *netmsg);
void __cdecl Com_DispatchClientPacketEvent(netadr_t adr, msg_t *netmsg);
void __cdecl Com_ReadCDKey();
int Com_ClearCDKey();
void __cdecl CL_ConvertRegKeytoDWKey(char *key, unsigned int size);
void __cdecl Com_SetRecommended(int localClientNum, int restart);
int __cdecl Com_ConfigureChecksum(const char *csv, int filesize);
char __cdecl Com_SetRecommendedCpu(int localClientNum, const SysInfo *info, char **text);
int __cdecl Com_GetConfigureDvarNames(const char **text, char (*dvarNames)[32]);
void __cdecl Com_GetConfigureDvarValues(int dvarCount, const char **text, char (*dvarValues)[32]);
void __cdecl Com_SetConfigureDvars(int dvarCount, const char (*dvarNames)[32], const char (*dvarValues)[32]);
char __cdecl Com_SetRecommendedGpu(const SysInfo *info, char **text);
bool __cdecl Com_DoesGpuStringMatch(const char *find, const char *ref);
int __cdecl Com_GpuStringCompare(const char *wild, const char *s);
void __cdecl Com_CheckSetRecommended(int localClientNum);
bool __cdecl Com_HasConfigureFileChanged();
void __cdecl Com_SetScriptSettings();
void __cdecl Com_RunAutoExec(int localClientNum, int controllerIndex);
void __cdecl Com_ExecStartupConfigs(int localClientNum, const char *configFile);
void __cdecl Com_InitUI3DCallback();
void __cdecl Com_InitUIAndCommonXAssets();
unsigned int __cdecl Com_CalculateStreamBuffer();
void __cdecl Com_Init(char *commandLine);
void Com_ErrorCleanup();
void Com_AddStartupCommands();
void __cdecl Com_Init_Try_Block_Function(char *commandLine);
void __cdecl Com_ParseCommandLine(char *commandLine);
void __cdecl Com_Error_f();
void __cdecl Com_Freeze_f();
void __cdecl Com_Crash_f();
void __cdecl Com_Assert_f();
int COM_PlayIntroMovies();
void Com_InitDvars();
void __cdecl Com_StartupConfigs(int localClientNum);
void __cdecl Com_InitCodeXAssets();
void __cdecl Com_WriteDefaultsToFile(char *filename);
void __cdecl Com_WriteConfig_f();
void __cdecl Com_WriteConfigToFile(int localClientNum, char *filename);
void __cdecl Com_WriteKeyConfig_f();
void __cdecl Com_WriteKeyConfigToFile(int localClientNum, char *filename);
void __cdecl Com_SaveKeys_f();
void __cdecl Com_RestoreKeys_f();
void __cdecl Com_WriteDefaults_f();
double __cdecl Com_GetTimescaleForSnd();
int Com_LoadUiFastFile();
void __cdecl Com_LoadMapLoadingScreenFastFile();
void __cdecl Com_UnloadLevelFastFiles();
void __cdecl Com_LoadLevelFastFiles(char *mapName);
char *__cdecl Com_GetLevelSharedFastFile(char *mapName);
void Com_LoadCommonFastFile();
void __cdecl Com_LoadFrontEnd();
void __cdecl Com_UnloadFrontEnd();
void __cdecl Com_AssetLoadUI();
void __cdecl Com_ResetFrametime();
void __cdecl Com_CheckSyncFrame();
void __cdecl Com_Frame();
unsigned intCom_Frame_Try_Block_Function();
void __cdecl Com_WriteConfiguration(int localClientNum);
int __cdecl Com_ModifyMsec(int msec);
void Com_Statmon();
void __cdecl Com_AdjustMaxFPS(int *maxFPS);
char Com_UpdateMenu();
void Com_StartHunkUsers();
//void __thiscall cdl_proftimer::reset(cdl_proftimer *this);
void __cdecl Com_CloseLogfiles();
bool __cdecl Com_LogFileOpen();
void __cdecl Com_Close();
void __cdecl Field_Clear(field_t *edit);
void __cdecl Com_Restart();
XAnimTree_s *__cdecl Com_XAnimCreateSmallTree(XAnim_s *anims);
void __cdecl Com_XAnimFreeSmallTree(XAnimTree_s *animtree);
void __cdecl Com_SetWeaponInfoMemory(int source);
void __cdecl Com_FreeWeaponInfoMemory(int source);
int __cdecl Com_AddToString(const char *add, char *msg, int len, int maxlen, int mayAddQuotes);
char __cdecl Com_GetDecimalDelimiter();
void __cdecl Com_LocalizedFloatToString(float f, char *buffer, unsigned int maxlen, unsigned int numDecimalPlaces);
void __cdecl Com_SyncThreads();
const char *__cdecl Com_DisplayName(const char *name, const char *clanAbbrev, int type);
const char *__cdecl CS_DisplayName(const clientState_s *cs, int type);
int __cdecl Com_GetPrivateClients();
void Com_Printf_NoFilter(char *fmt, ...);
