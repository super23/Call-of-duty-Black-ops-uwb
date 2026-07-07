#include "db_stream.h"
#include <stdint.h>
#include <universal/assertive.h>

XBlock *g_streamBlocks;
unsigned __int8 *g_streamPos;
unsigned int g_streamPosIndex;
unsigned int g_streamDelayIndex;
unsigned __int8 *g_streamPosArray[DB_STREAM_COUNT];
unsigned int g_streamPosStackIndex;

namespace
{
    const unsigned int STREAM_POS_STACK_DEPTH = 64;
}

static StreamPosInfo g_streamPosStack[STREAM_POS_STACK_DEPTH];

void __cdecl DB_InitStreams(XBlock *blocks)
{
    unsigned int i;

    g_streamBlocks = blocks;
    g_streamPos = blocks->data;
    g_streamPosIndex = 0;
    g_streamDelayIndex = 0;
    g_streamPosArray[0] = 0;
    g_streamPosStackIndex = 0;
    for (i = 1; i < ARRAY_COUNT(g_streamPosArray); ++i)
        g_streamPosArray[i] = blocks[i].data;
}

void __cdecl DB_PushStreamPos(unsigned int index)
{
    StreamPosInfo *stackEntry;

    iassert(index < ARRAY_COUNT(g_streamPosArray));
    iassert(g_streamPosIndex < ARRAY_COUNT(g_streamPosArray));
    iassert(g_streamPosStackIndex < ARRAY_COUNT(g_streamPosStack));

    stackEntry = &g_streamPosStack[g_streamPosStackIndex++];
    stackEntry->index = g_streamPosIndex;
    DB_SetStreamIndex(index);
    stackEntry->pos = g_streamPos;
}

void __cdecl DB_SetStreamIndex(unsigned int index)
{
    if ( index != g_streamPosIndex )
    {
        g_streamPosArray[g_streamPosIndex] = g_streamPos;
        g_streamPosIndex = index;
        g_streamPos = g_streamPosArray[index];
        g_streamPosArray[index] = 0;
    }
}

void __cdecl DB_PopStreamPos()
{
    const StreamPosInfo *stackEntry;

    iassert(g_streamPosStackIndex > 0);

    stackEntry = &g_streamPosStack[--g_streamPosStackIndex];
    if (!g_streamPosIndex)
        g_streamPos = stackEntry->pos;
    DB_SetStreamIndex(stackEntry->index);
}

unsigned __int8 *__cdecl DB_GetStreamPos()
{
    return g_streamPos;
}

unsigned __int8 *__cdecl DB_AllocStreamPos(int alignment)
{
    uintptr_t alignedPos;

    iassert(alignment >= 0);
    iassert(g_streamPos);

    alignedPos = ((uintptr_t)g_streamPos + alignment) & ~(uintptr_t)alignment;
    g_streamPos = (unsigned __int8 *)alignedPos;
    return g_streamPos;
}

void __cdecl DB_IncStreamPos(int size)
{
    iassert(size >= 0);
    iassert(g_streamPos);
    iassert(g_streamPos + size <= g_streamBlocks[g_streamPosIndex].data + g_streamBlocks[g_streamPosIndex].size);
    g_streamPos += size;
}

const void **__cdecl DB_InsertPointer()
{
    const void **insertedPointer;

    DB_PushStreamPos(4u);
    insertedPointer = (const void **)DB_AllocStreamPos(3);
    DB_IncStreamPos(4);
    DB_PopStreamPos();
    return insertedPointer;
}

