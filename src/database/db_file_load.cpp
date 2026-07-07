#include "db_file_load.h"
#include <universal/assertive.h>
#include "db_auth.h"
#include "db_registry.h"

#include <Windows.h>
#include <win32/win_shared.h>
#include <qcommon/common.h>
#include <qcommon/threads.h>
#include "db_memory.h"
#include "db_stream.h"
#include "db_stream_load.h"
#include "db_load.h"
#include <tl/tl_system.h>

volatile int g_totalSize;
volatile unsigned int g_loadedSize;
volatile unsigned int g_loadingAssets;
volatile int g_totalExternalBytes;
volatile unsigned int g_loadedExternalBytes;
volatile int g_totalStreamBytes;
int g_trackLoadProgress;
bool g_minimumFastFileLoaded;
int g_totalWait;

XAssetList *varXAssetList;
XAssetList g_varXAssetList;

DB_LoadData g_load;

double __cdecl DB_GetLoadedFraction()
{
    double loadedBytesInternal; // [esp+Ch] [ebp-30h]
    double totalBytesInternal; // [esp+1Ch] [ebp-20h]

    if ( !g_totalSize )
        return 0.0;
    totalBytesInternal = (double)g_totalSize * 262144.0;
    loadedBytesInternal = (double)g_loadedSize * 262144.0;
    if ( loadedBytesInternal < 0.0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    326,
                    0,
                    "%s",
                    "loadedBytesInternal >= 0") )
    {
        __debugbreak();
    }
    if ( loadedBytesInternal > totalBytesInternal )
        loadedBytesInternal = totalBytesInternal;
    if ( g_loadedSize >= g_totalSize )
        return 1.0;
    return (float)(loadedBytesInternal / totalBytesInternal);
}

bool __cdecl DB_FinishedLoadingAssets()
{
    return g_loadingAssets == 0;
}

void __cdecl DB_ResetZoneSize(int trackLoadProgress)
{
    g_totalSize = 0;
    g_loadedSize = 0;
    g_totalExternalBytes = 0;
    g_loadedExternalBytes = 0;
    g_totalStreamBytes = 0;
    g_trackLoadProgress = trackLoadProgress;
}

bool __cdecl DB_IsMinimumFastFileLoaded()
{
    return g_minimumFastFileLoaded;
}

void __cdecl DB_ResetMinimumFastFileLoaded()
{
    g_minimumFastFileLoaded = 0;
}

void __cdecl DB_CancelLoadXFile()
{
    if ( g_load.compressBufferStart )
    {
        while ( g_load.outstandingReads )
            DB_WaitXFileStage();
        DB_AuthLoad_InflateEnd(&g_load.stream);
        if ( !g_load.f
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp", 1172, 0, "%s", "g_load.f") )
        {
            __debugbreak();
        }
        if ( g_load.f == (void *)-1
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                        1179,
                        0,
                        "%s",
                        "g_load.f != INVALID_DBFILE") )
        {
            __debugbreak();
        }
        CloseHandle(g_load.f);
    }
}

signed __int32 DB_WaitXFileStage()
{
    signed __int32 previousLoadedBlockCount; // eax
    DWORD waitStart; // [esp+0h] [ebp-8h]

    if (!g_load.f
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp", 1327, 0, "%s", "g_load.f"))
    {
        __debugbreak();
    }
    if (g_load.outstandingReads <= 0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
            1329,
            0,
            "%s",
            "g_load.outstandingReads > 0"))
    {
        __debugbreak();
    }
    --g_load.outstandingReads;
    waitStart = Sys_Milliseconds();
    SleepEx(0xFFFFFFFF, 1);
    g_totalWait += Sys_Milliseconds() - waitStart;
    previousLoadedBlockCount = _InterlockedExchangeAdd(&g_loadedSize, 1u);
    g_load.stream.avail_in += 0x40000;
    return previousLoadedBlockCount;
}

void __cdecl DB_LoadedExternalData(unsigned int size)
{
    _InterlockedExchangeAdd(&g_loadedExternalBytes, size);
}

unsigned __int8 *__cdecl DB_LoadXFileDataNullTerminated(unsigned __int8 *pos)
{
    unsigned __int8 *startPos; // [esp+0h] [ebp-4h]

    startPos = pos;
    do
    {
        if ( g_load.deflateBufferPos == 0x8000 )
            DB_LoadXFileData(pos, 1);
        else
            *pos = g_load.deflateBuffer[g_load.deflateBufferPos++];
    }
    while ( *pos++ );
    return (unsigned __int8 *)(pos - startPos);
}

void __cdecl DB_LoadXFileSetSize(int size)
{
    if ( g_load.deflateRemainingFileSize
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1425,
                    0,
                    "%s",
                    "g_load.deflateRemainingFileSize == 0") )
    {
        __debugbreak();
    }
    g_load.deflateRemainingFileSize = size;
}

void __cdecl DB_LoadXFileData(unsigned __int8 *pos, int size)
{
    const char *errorMessage; // eax
    int requestedOutputSize; // [esp+4h] [ebp-1Ch]
    signed int inflateChunkSize; // [esp+8h] [ebp-18h]
    unsigned int bytesToCopy; // [esp+10h] [ebp-10h]
    unsigned int inflateResult; // [esp+18h] [ebp-8h]

    iassert(size);
    iassert(g_load.f);
    iassert(!g_load.stream.avail_out);

RESTART:
    if ( size > 0 )
    {
        if ( size + g_load.deflateBufferPos > 0x8000 )
        {
            if ( g_load.deflateBufferPos > 0x8000
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                            1474,
                            0,
                            "%s",
                            "g_load.deflateBufferPos <= DEFLATE_BUFFER_SIZE") )
            {
                __debugbreak();
            }
            if ( g_load.deflateBufferPos < 0x8000 )
            {
                bytesToCopy = 0x8000 - g_load.deflateBufferPos;
                if ( g_load.deflateBufferPos == 32767 )
                    *pos = g_load.deflateBuffer[g_load.deflateBufferPos];
                else
                    memcpy(pos, &g_load.deflateBuffer[g_load.deflateBufferPos], bytesToCopy);
                g_load.deflateBufferPos += bytesToCopy;
                pos += bytesToCopy;
                size -= bytesToCopy;
            }
            if ( g_load.deflateRemainingFileSize < 0x8000 )
                inflateChunkSize = g_load.deflateRemainingFileSize;
            else
                inflateChunkSize = 0x8000;
            g_load.stream.avail_out = inflateChunkSize;
            g_load.deflateBufferPos = 0x8000 - inflateChunkSize;
            g_load.stream.next_out = &g_load.deflateBuffer[0x8000 - inflateChunkSize];
            if ( inflateChunkSize < size )
                requestedOutputSize = g_load.stream.avail_out;
            else
                requestedOutputSize = size;
            size -= requestedOutputSize;
            g_load.deflateRemainingFileSize -= g_load.stream.avail_out;


            while ( 1 )
            {
                if ( g_load.stream.avail_in )
                {
                    inflateResult = DB_AuthLoad_Inflate(&g_load.stream, 2);
                    if ( inflateResult != Z_OK && inflateResult != Z_STREAM_END )
                    {
                        DB_CancelLoadXFile();
                        Com_Error(
                            ERR_DROP,
                            "Fastfile for zone '%s' appears corrupt or unreadable (code %i.)",
                            g_load.filename,
                            inflateResult + 110);
                    }
                    if ( g_load.f )
                    {
                        if ( (unsigned int)(g_load.stream.next_in - g_load.compressBufferStart) > 0x80000
                            && !Assert_MyHandler(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                                        1537,
                                        0,
                                        "%s",
                                        "static_cast< unsigned >( g_load.stream.next_in - g_load.compressBufferStart ) <= DBFILE_BUFFER_SIZE") )
                        {
                            __debugbreak();
                        }
                        if ( g_load.stream.next_in == g_load.compressBufferEnd )
                            g_load.stream.next_in = g_load.compressBufferStart;
                    }

                    _mm_prefetch((const char *)g_load.stream.next_in + 512, 1);

                    if ( !g_load.stream.avail_out )
                    {
                        if ( requestedOutputSize > 0x8000
                            && !Assert_MyHandler(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                                        1558,
                                        0,
                                        "%s",
                                        "lastAvailOutSize <= DEFLATE_BUFFER_SIZE") )
                        {
                            __debugbreak();
                        }
                        if ( requestedOutputSize == 1 )
                            *pos = g_load.deflateBuffer[g_load.deflateBufferPos];
                        else
                            memcpy(pos, &g_load.deflateBuffer[g_load.deflateBufferPos], requestedOutputSize);
                        g_load.deflateBufferPos += requestedOutputSize;
                        pos += requestedOutputSize;
                        goto RESTART;
                    }

                    if ( inflateResult )
                    {
                        errorMessage = va("Invalid fast file '%s' (%d != Z_OK)", g_load.filename, inflateResult);
                        if ( !Assert_MyHandler(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                                        1547,
                                        0,
                                        "%s\n\t%s",
                                        "err == Z_OK",
                                        errorMessage) )
                            __debugbreak();
                    }
                }
                DB_WaitXFileStage();
                DB_ReadXFileStage();
            }
        }
        if ( size == 1 )
            *pos = g_load.deflateBuffer[g_load.deflateBufferPos];
        else
            memcpy(pos, &g_load.deflateBuffer[g_load.deflateBufferPos], size);
        g_load.deflateBufferPos += size;
    }
}

void DB_ReadXFileStage()
{
    if ( g_load.f )
    {
        if ( g_load.outstandingReads
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                        1304,
                        0,
                        "%s",
                        "!g_load.outstandingReads") )
        {
            __debugbreak();
        }
        if ( !DB_ReadData() && GetLastError() != 38 )
            Com_Error(ERR_DROP, "Read error of file '%s'", g_load.filename);
    }
}

VOID WINAPI DB_FileReadCompletionDummyCallback(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped)
{
    ;
}

int __cdecl DB_ReadData()
{
    unsigned __int8 *fileBuffer; // [esp+0h] [ebp-8h]

    if ( !g_load.compressBufferStart
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1203,
                    0,
                    "%s",
                    "g_load.compressBufferStart") )
    {
        __debugbreak();
    }
    if ( !g_load.f
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp", 1204, 0, "%s", "g_load.f") )
    {
        __debugbreak();
    }
    if ( g_load.interrupt )
        g_load.interrupt();
    fileBuffer = &g_load.compressBufferStart[g_load.overlapped.Offset % 0x80000];
    Sys_WaitDiscReads();
    if ( !ReadFileEx(g_load.f, fileBuffer, 0x40000u, &g_load.overlapped, DB_FileReadCompletionDummyCallback) )
        return 0;
    ++g_load.outstandingReads;
    g_load.overlapped.Offset += 0x40000;
    return 1;
}

void __cdecl DB_LoadXFileInternal()
{
    int waitTimeMs; // [esp-4h] [ebp-50h]
    int inflateInitResult; // [esp+8h] [ebp-44h]
    bool isAuthenticatedFile; // [esp+Fh] [ebp-3Dh]
    unsigned int fastfileVersion; // [esp+10h] [ebp-3Ch]
    XFile xfileHeader; // [esp+14h] [ebp-38h] BYREF
    int compressedFileSize; // [esp+38h] [ebp-14h]
    const char *failureReason; // [esp+3Ch] [ebp-10h]
    char magicHeader[8]; // [esp+40h] [ebp-Ch] BYREF

    g_totalWait = 0;
    if ( !g_load.f
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp", 1739, 0, "%s", "g_load.f") )
    {
        __debugbreak();
    }
    DB_ReadXFileStage();
    if ( !g_load.outstandingReads )
        Com_Error(ERR_DROP, "Fastfile for zone '%s' is empty.", g_load.filename);
    DB_WaitXFileStage();
    DB_ReadXFileStage();
    if ( g_load.stream.avail_in < sizeof(magicHeader)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1768,
                    0,
                    "%s",
                    "sizeof( magic ) <= g_load.stream.avail_in") )
    {
        __debugbreak();
    }
    *(unsigned int *)magicHeader = *(unsigned int *)g_load.stream.next_in;
    *(unsigned int *)&magicHeader[4] = *((unsigned int *)g_load.stream.next_in + 1);
    g_load.stream.next_in += sizeof(magicHeader);
    g_load.stream.avail_in -= sizeof(magicHeader);
    if ( memcmp(magicHeader, "IWff0100", sizeof(magicHeader)) && memcmp(magicHeader, "IWffu100", sizeof(magicHeader)) )
    {
        Com_Error(ERR_DROP, "Fastfile for zone '%s' is corrupt or unreadable.", g_load.filename);
    }
    if ( g_load.stream.avail_in < sizeof(fastfileVersion)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1787,
                    0,
                    "%s",
                    "sizeof( version ) <= g_load.stream.avail_in") )
    {
        __debugbreak();
    }
    fastfileVersion = *(unsigned int *)g_load.stream.next_in;
    g_load.stream.next_in += sizeof(fastfileVersion);
    g_load.stream.avail_in -= sizeof(fastfileVersion);
    if ( fastfileVersion != 473 )
    {
        if ( fastfileVersion >= 0x1D9 )
            Com_Error(
                ERR_DROP,
                "Fastfile for zone '%s' is newer than client executable (version %d, expecting %d)",
                g_load.filename,
                fastfileVersion,
                473);
        else
            Com_Error(
                ERR_DROP,
                "Fastfile for zone '%s' is out of date (version %d, expecting %d)",
                g_load.filename,
                fastfileVersion,
                473);
    }
    isAuthenticatedFile = memcmp(magicHeader, "IWffu100", sizeof(magicHeader)) != 0;
    inflateInitResult = DB_AuthLoad_InflateInit(&g_load.stream, isAuthenticatedFile);
    failureReason = 0;
    if ( isAuthenticatedFile )
        failureReason = "authenticated file not supported";
    if ( inflateInitResult )
        failureReason = "init failed";
    if ( failureReason )
    {
        DB_CancelLoadXFile();
        Com_Error(ERR_DROP, "Fastfile for zone '%s' could not be loaded (%s)", g_load.filename, failureReason);
    }
    if ( g_load.deflateBufferPos != 0x8000
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1861,
                    0,
                    "%s",
                    "g_load.deflateBufferPos == DEFLATE_BUFFER_SIZE") )
    {
        __debugbreak();
    }
    DB_LoadXFileSetSize(sizeof(xfileHeader));
    DB_LoadXFileData((unsigned __int8 *)&xfileHeader, sizeof(xfileHeader));
    DB_LoadXFileSetSize(xfileHeader.size);
    if ( g_trackLoadProgress )
    {
        compressedFileSize = GetFileSize(g_load.f, 0);
        if ( xfileHeader.externalSize + compressedFileSize >= 0x100000 )
        {
            g_totalSize = (compressedFileSize + 0x3FFFF) / 0x40000 - g_loadedSize;
            g_loadedSize = 0;
            g_totalExternalBytes = xfileHeader.externalSize - g_loadedExternalBytes;
            g_loadedExternalBytes = 0;
        }
    }
    Com_Printf(16, "Loading fastfile '%s'\n", g_load.filename);
    tlPrintf("TLPrint: Loading fastfile '%s'\n", g_load.filename);
    DB_AllocXBlocks(xfileHeader.blockSize, g_load.filename, g_load.blocks, g_load.allocType, g_load.flags);
    DB_InitStreams(g_load.blocks);
    Load_XAssetListCustom();
    DB_PushStreamPos(4u);
    if ( varXAssetList->assets )
    {
        varXAssetList->assets = AllocLoad_FxElemVisStateSample();
        varXAsset = varXAssetList->assets;
        Load_XAssetArrayCustom(varXAssetList->assetCount);
    }
    DB_PopStreamPos();
    Load_DelayStream();
    --g_loadingAssets;
    DB_MarkAssetTypeUsageDirty();
    DB_LoadDelayedImages();
    if ( !g_load.compressBufferStart
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1928,
                    0,
                    "%s",
                    "g_load.compressBufferStart") )
    {
        __debugbreak();
    }
    waitTimeMs = g_totalWait;
    Com_Printf(10, "Loaded fastfile '%s' in %ims (%dms waiting)\n", g_load.filename, Sys_Milliseconds() - g_load.startTime, waitTimeMs);
    if ( (g_load.flags & 1) != 0 )
        g_minimumFastFileLoaded = 1;
    DB_CancelLoadXFile();
}

void Load_XAssetListCustom()
{
    varXAssetList = &g_varXAssetList;
    DB_LoadXFileData((unsigned __int8 *)&g_varXAssetList, sizeof(g_varXAssetList));
    DB_PushStreamPos(4u);
    varScriptStringList = &varXAssetList->stringList;
    Load_ScriptStringList(0);
    DB_PopStreamPos();
}

void __cdecl Load_XAssetArrayCustom(int count)
{
    XAsset *asset; // [esp+0h] [ebp-8h]
    int assetIndex; // [esp+4h] [ebp-4h]

    Load_Stream(1, (unsigned __int8 *)varXAsset, 8 * count);
    asset = varXAsset;
    for ( assetIndex = 0; assetIndex < count; ++assetIndex )
    {
        skipShaderCreationHack = assetIndex == skipLoadingMaterialsHack;
        varXAsset = asset;
        Load_XAsset(0);
        ++asset;
        if ( g_load.abort )
            break;
    }
}

char __cdecl DB_LoadXFile(
                const char *path,
                void *f,
                const char *filename,
                XBlock *blocks,
                void (__cdecl *interrupt)(),
                unsigned __int8 *buf,
                int allocType,
                int flags)
{
    if ( ((unsigned __int8)buf & 3) != 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1959,
                    0,
                    "%s",
                    "!(reinterpret_cast< psize_int >( buf ) & 3)") )
    {
        __debugbreak();
    }
    memset((unsigned __int8 *)&g_load, 0, sizeof(g_load));
    g_load.f = f;
    g_load.filename = filename;
    g_load.flags = flags;
    g_load.blocks = blocks;
    g_load.interrupt = interrupt;
    g_load.allocType = allocType;
    g_load.startTime = Sys_Milliseconds();
    if ( g_load.compressBufferStart
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp",
                    1979,
                    0,
                    "%s",
                    "!g_load.compressBufferStart") )
    {
        __debugbreak();
    }
    if ( !g_load.f
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp", 1981, 0, "%s", "g_load.f") )
    {
        __debugbreak();
    }
    if ( !buf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_file_load.cpp", 1983, 0, "%s", "buf") )
        __debugbreak();
    g_load.compressBufferStart = buf;
    g_load.compressBufferEnd = buf + 0x80000;
    g_load.stream.next_in = buf;
    g_load.stream.avail_in = 0;
    g_load.deflateBufferPos = 0x8000;
    DB_LoadXFileInternal();
    return 1;
}

