#include "db_stream_load.h"
#include "db_stream.h"
#include <universal/assertive.h>
#include <string.h>
#include "db_file_load.h"
#include <clientscript/cscr_stringlist.h>

StreamDelayInfo g_streamDelayArray[4096];

void __cdecl Load_Stream(bool atStreamStart, unsigned __int8 *ptr, int size)
{
    iassert(atStreamStart == (ptr == DB_GetStreamPos()));

    if (atStreamStart && size)
    {
        if (g_streamPosIndex > 0 && g_streamPosIndex < 4)
        {
            if (g_streamPosIndex == 1)
            {
                memset(ptr, 0, size);
            }
            else
            {
                if (g_streamDelayIndex >= ARRAY_COUNT(g_streamDelayArray)
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_stream_load.cpp",
                        36,
                        0,
                        "g_streamDelayIndex doesn't index ARRAY_COUNT( g_streamDelayArray )\n\t%i not in [0, %i)",
                        g_streamDelayIndex,
                        ARRAY_COUNT(g_streamDelayArray)))
                {
                    __debugbreak();
                }
                g_streamDelayArray[g_streamDelayIndex].ptr = ptr;
                g_streamDelayArray[g_streamDelayIndex++].size = size;
            }
        }
        else
        {
            DB_LoadXFileData(ptr, size);
        }
        DB_IncStreamPos(size);
    }
}

void __cdecl Load_DelayStream()
{
    for (unsigned int index = 0; index < g_streamDelayIndex; ++index)
        DB_LoadXFileData((unsigned __int8 *)g_streamDelayArray[index].ptr, g_streamDelayArray[index].size);
}

void __cdecl DB_ConvertOffsetToAlias(unsigned int *data)
{
    const unsigned int offset = *data;

    if ((!offset || offset >= 0xFFFFFFFE)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_stream_load.cpp",
            67,
            0,
            "%s\n\t(offset) = %i",
            "(offset && (offset != 0xFFFFFFFF) && (offset != 0xFFFFFFFE))",
            offset))
    {
        __debugbreak();
    }

    const unsigned int streamOffset = offset - 1;
    *data = *(unsigned int *)&g_streamBlocks[streamOffset >> 29].data[streamOffset & 0x1FFFFFFF];
}

void __cdecl DB_ConvertOffsetToPointer(unsigned int *data)
{
    const unsigned int streamOffset = *data - 1;
    *data = (unsigned int)&g_streamBlocks[streamOffset >> 29].data[streamOffset & 0x1FFFFFFF];
}

void __cdecl Load_XStringCustom(const char **str)
{
    const int bytesLoaded = (int)DB_LoadXFileDataNullTerminated((unsigned __int8 *)*str);
    DB_IncStreamPos(bytesLoaded);
}

void __cdecl Load_TempStringCustom(const char **str)
{
    const char *tempString;

    Load_XStringCustom(str);
    if (*str)
        tempString = (const char *)SL_GetString((char *)*str, 4u, SCRIPTINSTANCE_SERVER);
    else
        tempString = 0;
    *str = tempString;
}

