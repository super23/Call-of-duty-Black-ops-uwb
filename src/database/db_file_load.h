#pragma once

double __cdecl DB_GetLoadedFraction();
bool __cdecl DB_FinishedLoadingAssets();
void __cdecl DB_ResetZoneSize(int trackLoadProgress);
bool __cdecl DB_IsMinimumFastFileLoaded();
void __cdecl DB_ResetMinimumFastFileLoaded();
void __cdecl DB_CancelLoadXFile();
signed __int32 DB_WaitXFileStage();
void __cdecl DB_LoadedExternalData(unsigned int size);
unsigned __int8 *__cdecl DB_LoadXFileDataNullTerminated(unsigned __int8 *pos);
void __cdecl DB_LoadXFileSetSize(int size);
void __cdecl DB_LoadXFileData(unsigned __int8 *pos, int size);
void DB_ReadXFileStage();
int __cdecl DB_ReadData();
void __stdcall DB_FileReadCompletionDummyCallback(
        unsigned int dwErrorCode,
        unsigned int dwNumberOfBytesTransfered,
        _OVERLAPPED *lpOverlapped);
void __cdecl DB_LoadXFileInternal();
void Load_XAssetListCustom();
void __cdecl Load_XAssetArrayCustom(int count);
char __cdecl DB_LoadXFile(
        const char *path,
        void *f,
        const char *filename,
        XBlock *blocks,
        void (__cdecl *interrupt)(),
        unsigned __int8 *buf,
        int allocType,
        int flags);
