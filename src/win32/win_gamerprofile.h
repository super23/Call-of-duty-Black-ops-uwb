#pragma once

void __cdecl ResetCreateAClassNames(int controllerIndex);
void __cdecl SetDvarFromLocString(int controllerIndex, const char *dvarName, char *preLocalizedText);
GamerSettingState *__cdecl Gamer//Profile_GetProfileSettings(int controllerIndex);
bool __cdecl Gamer//Profile_IsProfileLoggedIn(int controllerIndex);
bool __cdecl Gamer//Profile_WasErrorOnRead(int controllerIndex);
int __cdecl Gamer//Profile_GetPlaylistNum(int controllerIndex);
void __cdecl Gamer//Profile_SetPlaylistNum(int controllerIndex, int playlistNum);
void __cdecl Gamer//Profile_ExecControllerBindings(int controllerIndex);
void __cdecl Gamer//Profile_UpdateDvarsFromProfile(int controllerIndex);
void __cdecl DebugReportProfileDVars(const char *headerMsg);
void __cdecl ResetCreateAClassNames(int controllerIndex);
void __cdecl Gamer//Profile_UpdateProfileFromDvars(int controllerIndex, profileWriteState_t profileWriteState);
void __cdecl Gamer//Profile_SaveProfile(int controllerIndex);
void __cdecl Gamer//Profile_CreateExeBindingsConfig(int controllerIndex);
void __cdecl Gamer//Profile_AppendDvarListToConfig(char *buffer, unsigned int size, const char **dvarNameList);
void __cdecl Gamer//Profile_AppendByteListToConfig(
        char *buffer,
        unsigned int bufSize,
        unsigned __int8 *data,
        unsigned __int16 dataSize);
void __cdecl Gamer//Profile_CreateCommonBindingsConfig(int controllerIndex);
bool __cdecl Gamer//Profile_IsNewLocalProfile(int controllerIndex);
void __cdecl Gamer//Profile_InitAllProfiles();
void __cdecl Gamer//Profile_LogInProfile(int controllerIndex);
void __cdecl Gamer//Profile_LogOutProfile(int controllerIndex);
