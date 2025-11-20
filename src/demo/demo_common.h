#pragma once

void __cdecl Demo_RegisterDvars();
void __cdecl Demo_RegisterCommands();
void __cdecl Demo_AddDemoClient_f();
void __cdecl Demo_RemoveDemoClient_f();
void __cdecl Demo_SetTags(
        int controllerIndex,
        int *numTags,
        bdTag *tags,
        demoMetaInfo *info,
        fileShareSearchFileType fileType);
void __cdecl Demo_SetTags(
        int controllerIndex,
        int *numTags,
        bdTag *tags,
        screenshotMetaInfo *info,
        fileShareSearchFileType fileType);
char __cdecl Demo_SetMetaData(
        int controllerIndex,
        char *metaData,
        int *metaDataSize,
        demoMetaInfo *dInfo,
        screenshotMetaInfo *sInfo,
        fileShareSearchFileType fileType);
void __cdecl Demo_StreamingSuccessCallback(int controllerIndex, unsigned __int64 fileID);
void __cdecl Demo_StreamingFailureCallback();
void __cdecl Demo_StartRecord_f();
void __cdecl Demo_StopRecord_f();
void __cdecl Demo_Play_f();
void __cdecl Demo_Stop_f();
void __cdecl Demo_Forward_f();
void __cdecl Demo_Back_f();
void __cdecl Demo_JumpToStart_f();
void __cdecl Demo_Pause_f();
void __cdecl Demo_TimeScale_f();
void __cdecl Demo_ToggleGameHud_f();
void __cdecl Demo_ToggleDemoHud_f();
void __cdecl Demo_SwitchPlayer_f();
void __cdecl Demo_SwitchCamera_f();
void __cdecl Demo_StartClipRecord_f();
void __cdecl Demo_PauseClipRecord_f();
void __cdecl Demo_PreviewClip_f();
void __cdecl Demo_DeleteClip_f();
void __cdecl Demo_SaveAndUploadClip_f();
void __cdecl Demo_SaveSegment_f();
void __cdecl Demo_MoveSegment_f();
void __cdecl Demo_DeleteSegment_f();
void __cdecl Demo_PreviewSegment_f();
void __cdecl Demo_SwitchDefaultSegmentTag_f();
void __cdecl Demo_SwitchTransition_f();
void __cdecl Demo_Keyboard_f();
void  Demo_Screenshot_f(int a1@<esi>);
void __cdecl Demo_SaveScreenshot_f();
void __cdecl Demo_RenderMovie_f();
void __cdecl Demo_SwitchControls_f();
bool __cdecl Demo_IsEnabled();
void __cdecl Demo_SetDemoState(int state);
int __cdecl Demo_GetDemoState();
void __cdecl Demo_SetDemoClientState(int state);
int __cdecl Demo_GetDemoClientState();
bool __cdecl Demo_IsIdle();
bool __cdecl Demo_IsRecording();
bool __cdecl Demo_IsPlaying();
demoMain *__cdecl Demo_GetDemoName();
void __cdecl Demo_Printf(int channel, const char *string);
void __cdecl Demo_Init();
bool __cdecl Demo_InitWrite();
bool __cdecl Demo_InitRead();
void __cdecl Demo_Frame(int msec);
void __cdecl Demo_End(bool abnormalTermination);
void __cdecl Demo_ReadDataCallback(char *data, unsigned int dataSize);
void __cdecl Demo_ReadDataSuccessPlayIt(dwFileShareReadFileTask *task);
void __cdecl Demo_ReadDataSuccessRenderIt(dwFileShareReadFileTask *task);
void __cdecl Demo_ReadDataFailure();
void __cdecl Demo_DownloadFile(
        int controllerIndex,
        char *filmName,
        unsigned __int64 fileId,
        int fileSize,
        bool isUserFile,
        bool renderIt);
void __thiscall demoRecordedFileUploadInfo::~demoRecordedFileUploadInfo(demoRecordedFileUploadInfo *this);
