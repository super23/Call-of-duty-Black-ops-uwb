#pragma once

char *__cdecl FS_GetOsFolderPath(int folder, char *ospath);
bool __cdecl FS_Initialized();
void __cdecl FS_CheckFileSystemStarted();
int __cdecl FS_IwdIsPure(const iwd_t *iwd);
void __cdecl FS_DisablePureCheck(bool disable);
int __cdecl FS_LoadStack();
bool __cdecl FS_UseSearchPath(const searchpath_s *pSearch);
int __cdecl FS_LanguageHasAssets(int iLanguage);
int __cdecl FS_HashFileName(const char *fname, int hashSize);
int __cdecl FS_HandleForFile(const char *name, FsThread thread);
int __cdecl FS_HandleForFileCurrentThread(const char *filename);
int __cdecl FS_GetCurrentThread();
_iobuf *__cdecl FS_FileForHandle(int f);
int __cdecl FS_filelength(int f);
void __cdecl FS_ReplaceSeparators(char *path);
void __cdecl FS_BuildOSPath(char *base, char *game, char *qpath, char *ospath);
void __cdecl FS_BuildOSPathForThread(char *base, char *game, char *qpath, char *ospath, FsThread thread);
int __cdecl FS_CreatePath(char *OSPath);
int __cdecl FS_FileExists(char *file);
int __cdecl FS_OSFPathExists(const char *file);
void __cdecl FS_CopyFile(char *fromOSPath, char *toOSPath);
void __cdecl FS_Remove(const char *osPath);
void __cdecl FS_FCloseFile(int h);
void __cdecl FS_FCloseLogFile(int h);
int __cdecl FS_FOpenFileWriteToDir(char *filename, char *dir, const char *osbasepath);
int __cdecl FS_FOpenFileWriteToDirForThread(char *filename, char *dir, const char *osbasepath, FsThread thread);
int __cdecl FS_GetHandleAndOpenFile(const char *filename, const char *ospath, FsThread thread);
int __cdecl FS_FOpenFileWrite(char *filename);
int __cdecl FS_FOpenFileWriteCurrentThread(char *filename, char *dir, const char *osbasepath);
int __cdecl FS_FOpenTextFileWrite(char *filename);
int __cdecl FS_FOpenFileAppend(char *filename);
int __cdecl FS_FilenameCompare(const char *s1, const char *s2);
bool __cdecl FS_PureIgnoreFiles(const char *filename);
unsigned int __cdecl FS_FOpenFileRead(const char *filename, int *file);
unsigned int __cdecl FS_FOpenFileReadCurrentThread(const char *filename, int *file);
unsigned int __cdecl FS_FOpenFileReadForThread(const char *filename, int *file, FsThread thread);
char __cdecl FS_SanitizeFilename(const char *filename, char *sanitizedName, int sanitizedNameSize);
bool __cdecl FS_IsBackupSubStr(const char *filenameSubStr);
char __cdecl FS_FilesAreLoadedGlobally(const char *filename);
bool __cdecl FS_TouchFile(const char *name);
bool __cdecl FS_Delete(char *filename);
unsigned int __cdecl FS_Read(unsigned __int8 *buffer, unsigned int len, int h);
unsigned int __cdecl FS_Write(char *buffer, unsigned int len, int h);
unsigned int __cdecl FS_WriteToDemo(char *buffer, unsigned int len, int h);
void FS_Printf(int h, char *fmt, ...);
int __cdecl FS_Seek(int f, int offset, int origin);
int __cdecl FS_ReadFile(const char *qpath, void **buffer);
char *__cdecl FS_AllocMem(int bytes);
void __cdecl FS_ResetFiles();
void __cdecl FS_FreeFile(void *buffer);
void __cdecl FS_FreeMem(void *buffer);
int __cdecl FS_WriteFile(char *filename, char *buffer, unsigned int size);
int __cdecl FS_GetFileOsPath(const char *filename, char *ospath);
int __cdecl FS_OpenFileOverwrite(const char *qpath);
const char **__cdecl FS_ListFilteredFiles(
        searchpath_s *searchPath,
        const char *path,
        char *extension,
        char *filter,
        FsListBehavior_e behavior,
        int *numfiles);
int __cdecl FS_ReturnPath(const char *zname, char *zpath, int *depth);
int __cdecl FS_AddFileToList(HunkUser *user, const char *name, const char **list, int nfiles);
const char **__cdecl FS_ListFiles(const char *path, char *extension, FsListBehavior_e behavior, int *numfiles);
const char **__cdecl FS_ListFilesInLocation(
        const char *path,
        char *extension,
        FsListBehavior_e behavior,
        int *numfiles,
        int lookInFlags);
const char **__cdecl FS_ListFilteredFilesInLocation(
        const char *path,
        char *extension,
        char *filter,
        FsListBehavior_e behavior,
        int *numfiles,
        int lookInFlags);
bool __cdecl FS_CheckLocation(const char *path, int lookInFlags);
void __cdecl FS_FreeFileList(const char **list);
int __cdecl FS_GetFileList(const char *path, char *extension, FsListBehavior_e behavior, char *listbuf, int bufsize);
void __cdecl FS_ConvertPath(char *s);
int __cdecl FS_PathCmp(const char *s1, const char *s2);
void __cdecl FS_SortFileList(const char **filelist, int numfiles);
void __cdecl FS_DisplayPath(int bLanguageCull);
void __cdecl FS_FullPath_f();
void __cdecl FS_Path_f();
void __cdecl FS_AddUserMapDirIWDs(char *pszGameFolder);
void __cdecl FS_AddIwdFilesForGameDirectory(char *path, char *pszGameFolder);
iwd_t *__cdecl FS_LoadZipFile(char *zipfile, const char *basename);
iwd_t *__cdecl FS_LoadFakeIwdFile(char *fakeiwdfile, const char *basename);
char *__cdecl IwdFileLanguage(const char *pszIwdFileName);
int __cdecl iwdsort(const char **a, char **b);
void __cdecl FS_AddSearchPath(searchpath_s *search);
void __cdecl FS_ShutdownServerIwdNames();
void __cdecl FS_ShutdownServerFileReferences(int *numFiles, const char **fileNames);
void __cdecl FS_ShutdownServerReferencedIwds();
void __cdecl FS_ShutdownServerReferencedFFs();
void __cdecl FS_Shutdown();
void __cdecl FS_ShutdownSearchPaths(searchpath_s *p);
void __cdecl FS_Startup(const char *gameName, bool allow_devraw);
void __cdecl FS_AddGameDirectory(char *path, const char *dir, int bLanguageDirectory, int iLanguage);
void __cdecl FS_AddLocalizedGameDirectory(char *path, const char *dir);
const dvar_s *__thiscall FS_RegisterDvars(jpeg_common_struct *cinfo);
void __cdecl FS_AddDevGameDirs(char *path, bool allow_devraw);
void __cdecl FS_ClearIwdReferences();
void __cdecl FS_AddIwdPureCheckReference(const searchpath_s *search);
void __cdecl FS_ShutDownIwdPureCheckReferences();
bool __cdecl FS_IsBasePathValid();
void __cdecl FS_InitFilesystem(bool allow_devraw);
void __cdecl FS_Restart(int localClientNum, int checksumFeed);
bool __cdecl FS_NeedRestart(int checksumFeed);
unsigned int __cdecl FS_FOpenFileByMode(char *qpath, int *f, fsMode_t mode);
unsigned int __cdecl FS_FTell(int f);
void __cdecl FS_Flush(int f);
void __cdecl Com_GetBspFilename(char *filename, unsigned int size, const char *mapname);
