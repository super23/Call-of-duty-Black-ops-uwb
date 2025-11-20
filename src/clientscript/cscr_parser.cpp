#include "cscr_parser.h"

void __cdecl Scr_InitOpcodeLookup(scriptInstance_t inst)
{
  if ( gScrParserGlob[inst].opcodeLookup
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          94,
          0,
          "%s",
          "!gScrParserGlob[inst].opcodeLookup") )
  {
    __debugbreak();
  }
  if ( dword_9F7A814[13 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          95,
          0,
          "%s",
          "!gScrParserGlob[inst].sourcePosLookup") )
  {
    __debugbreak();
  }
  if ( gScrParserPub[inst].sourceBufferLookup
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          96,
          0,
          "%s",
          "!gScrParserPub[inst].sourceBufferLookup") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AB86][116 * inst] )
  {
    dword_9F7A834[13 * inst] = -1;
    dword_9F7A80C[13 * inst] = inst != SCRIPTINSTANCE_CLIENT ? 0x40000 : 0x4000;
    dword_9F7A810[13 * inst] = 0;
    gScrParserGlob[inst].opcodeLookup = (OpcodeLookup *)Hunk_UserAlloc(
                                                          g_DebugHunkUser,
                                                          12 * dword_9F7A80C[13 * inst],
                                                          4,
                                                          "Scr_InitOpcodeLookup");
    memset((unsigned __int8 *)gScrParserGlob[inst].opcodeLookup, 0, 12 * dword_9F7A80C[13 * inst]);
    dword_9F7A818[13 * inst] = inst != SCRIPTINSTANCE_CLIENT ? 393216 : 16;
    dword_9F7A81C[13 * inst] = 0;
    dword_9F7A814[13 * inst] = (int)Hunk_UserAlloc(
                                      g_DebugHunkUser,
                                      8 * dword_9F7A818[13 * inst],
                                      4,
                                      "Scr_InitOpcodeLookup");
    memset((unsigned __int8 *)dword_9F7A814[13 * inst], 0, 8 * dword_9F7A818[13 * inst]);
    dword_9F7A824[13 * inst] = 0;
    dword_9F7A828[13 * inst] = 0;
    dword_9F7A820[13 * inst] = inst != SCRIPTINSTANCE_CLIENT ? 256 : 16;
    gScrParserPub[inst].sourceBufferLookupLen = 0;
    gScrParserPub[inst].sourceBufferLookup = (SourceBufferInfo *)Hunk_UserAlloc(
                                                                   g_DebugHunkUser,
                                                                   24 * dword_9F7A820[13 * inst],
                                                                   4,
                                                                   "Scr_InitOpcodeLookup");
    memset((unsigned __int8 *)gScrParserPub[inst].sourceBufferLookup, 0, 24 * dword_9F7A820[13 * inst]);
  }
}

void __cdecl Scr_ShutdownOpcodeLookup(scriptInstance_t inst)
{
  unsigned int i; // [esp+0h] [ebp-4h]
  unsigned int ia; // [esp+0h] [ebp-4h]

  if ( gScrParserGlob[inst].opcodeLookup )
  {
    Hunk_UserFree(g_DebugHunkUser, gScrParserGlob[inst].opcodeLookup);
    gScrParserGlob[inst].opcodeLookup = 0;
  }
  if ( dword_9F7A814[13 * inst] )
  {
    Hunk_UserFree(g_DebugHunkUser, (void *)dword_9F7A814[13 * inst]);
    dword_9F7A814[13 * inst] = 0;
  }
  if ( gScrParserPub[inst].sourceBufferLookup )
  {
    for ( i = 0; i < gScrParserPub[inst].sourceBufferLookupLen; ++i )
      Hunk_UserFree(g_DebugHunkUser, gScrParserPub[inst].sourceBufferLookup[i].buf);
    Hunk_UserFree(g_DebugHunkUser, gScrParserPub[inst].sourceBufferLookup);
    gScrParserPub[inst].sourceBufferLookup = 0;
  }
  if ( dword_9F7A82C[13 * inst] )
  {
    for ( ia = 0; ia < dword_9F7A830[13 * inst]; ++ia )
    {
      if ( *(unsigned int *)(dword_9F7A82C[13 * inst] + 8 * ia) )
        Hunk_UserFree(g_DebugHunkUser, *(void **)(dword_9F7A82C[13 * inst] + 8 * ia));
    }
    Hunk_UserFree(g_DebugHunkUser, (void *)dword_9F7A82C[13 * inst]);
    dword_9F7A82C[13 * inst] = 0;
  }
}

void __cdecl AddOpcodePos(scriptInstance_t inst, unsigned int sourcePos, int type)
{
  int sourcePosLookupIndex; // [esp+0h] [ebp-14h]
  OpcodeLookup *opcodeLookup; // [esp+4h] [ebp-10h]
  SourceLookup *sourcePosLookup; // [esp+8h] [ebp-Ch]
  unsigned __int8 *newSourcePosLookup; // [esp+Ch] [ebp-8h]
  unsigned __int8 *newOpcodeLookup; // [esp+10h] [ebp-4h]

  if ( MEMORY[0xA05AB86][116 * inst] )
  {
    if ( gScrCompilePub[inst].developer_statement == 2 )
    {
      if ( MEMORY[0xA05AB87][116 * inst] )
      {
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                177,
                0,
                "%s",
                "!gScrVarPub[inst].developer_script") )
          __debugbreak();
      }
    }
    else if ( gScrCompilePub[inst].developer_statement != 3 )
    {
      if ( !gScrCompilePub[inst].allowedBreakpoint )
        type &= ~1u;
      if ( !gScrParserGlob[inst].opcodeLookup
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              189,
              0,
              "%s",
              "gScrParserGlob[inst].opcodeLookup") )
      {
        __debugbreak();
      }
      if ( !dword_9F7A80C[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              190,
              0,
              "%s",
              "gScrParserGlob[inst].opcodeLookupMaxLen") )
      {
        __debugbreak();
      }
      if ( !dword_9F7A814[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              191,
              0,
              "%s",
              "gScrParserGlob[inst].sourcePosLookup") )
      {
        __debugbreak();
      }
      if ( !gScrCompilePub[inst].opcodePos
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              192,
              0,
              "%s",
              "gScrCompilePub[inst].opcodePos") )
      {
        __debugbreak();
      }
      if ( dword_9F7A810[13 * inst] >= (unsigned int)dword_9F7A80C[13 * inst] )
      {
        dword_9F7A80C[13 * inst] += 0x10000;
        if ( dword_9F7A810[13 * inst] >= (unsigned int)dword_9F7A80C[13 * inst]
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                197,
                0,
                "%s",
                "gScrParserGlob[inst].opcodeLookupLen < gScrParserGlob[inst].opcodeLookupMaxLen") )
        {
          __debugbreak();
        }
        newOpcodeLookup = (unsigned __int8 *)Hunk_UserAlloc(
                                               g_DebugHunkUser,
                                               12 * dword_9F7A80C[13 * inst],
                                               4,
                                               "AddOpcodePos");
        memset(newOpcodeLookup, 0, 12 * dword_9F7A80C[13 * inst]);
        memcpy(newOpcodeLookup, (unsigned __int8 *)gScrParserGlob[inst].opcodeLookup, 12 * dword_9F7A810[13 * inst]);
        Hunk_UserFree(g_DebugHunkUser, gScrParserGlob[inst].opcodeLookup);
        gScrParserGlob[inst].opcodeLookup = (OpcodeLookup *)newOpcodeLookup;
      }
      if ( dword_9F7A81C[13 * inst] >= (unsigned int)dword_9F7A818[13 * inst] )
      {
        dword_9F7A818[13 * inst] += 0x10000;
        if ( dword_9F7A81C[13 * inst] >= (unsigned int)dword_9F7A818[13 * inst]
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                208,
                0,
                "%s",
                "gScrParserGlob[inst].sourcePosLookupLen < gScrParserGlob[inst].sourcePosLookupMaxLen") )
        {
          __debugbreak();
        }
        newSourcePosLookup = (unsigned __int8 *)Hunk_UserAlloc(
                                                  g_DebugHunkUser,
                                                  8 * dword_9F7A818[13 * inst],
                                                  4,
                                                  "AddOpcodePos");
        memset(newSourcePosLookup, 0, 8 * dword_9F7A818[13 * inst]);
        memcpy(newSourcePosLookup, (unsigned __int8 *)dword_9F7A814[13 * inst], 8 * dword_9F7A81C[13 * inst]);
        Hunk_UserFree(g_DebugHunkUser, (void *)dword_9F7A814[13 * inst]);
        dword_9F7A814[13 * inst] = (int)newSourcePosLookup;
      }
      if ( (unsigned __int8 *)dword_9F7A824[13 * inst] == gScrCompilePub[inst].opcodePos )
      {
        if ( !dword_9F7A828[13 * inst]
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                218,
                0,
                "%s",
                "gScrParserGlob[inst].currentSourcePosCount") )
        {
          __debugbreak();
        }
        opcodeLookup = &gScrParserGlob[inst].opcodeLookup[--dword_9F7A810[13 * inst]];
        if ( dword_9F7A828[13 * inst] + opcodeLookup->sourcePosIndex != dword_9F7A81C[13 * inst]
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                224,
                0,
                "%s",
                "opcodeLookup->sourcePosIndex + gScrParserGlob[inst].currentSourcePosCount == gScrParserGlob[inst].sourcePosLookupLen") )
        {
          __debugbreak();
        }
        if ( opcodeLookup->codePos != (const char *)dword_9F7A824[13 * inst]
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                225,
                0,
                "%s",
                "opcodeLookup->codePos == (char *)gScrParserGlob[inst].currentCodePos") )
        {
          __debugbreak();
        }
      }
      else
      {
        dword_9F7A828[13 * inst] = 0;
        dword_9F7A824[13 * inst] = (int)gScrCompilePub[inst].opcodePos;
        opcodeLookup = &gScrParserGlob[inst].opcodeLookup[dword_9F7A810[13 * inst]];
        opcodeLookup->sourcePosIndex = dword_9F7A81C[13 * inst];
        opcodeLookup->codePos = (const char *)dword_9F7A824[13 * inst];
      }
      sourcePosLookupIndex = dword_9F7A828[13 * inst] + opcodeLookup->sourcePosIndex;
      sourcePosLookup = (SourceLookup *)(dword_9F7A814[13 * inst] + 8 * sourcePosLookupIndex);
      sourcePosLookup->sourcePos = sourcePos;
      if ( sourcePos == -1 )
      {
        if ( dword_9F7A834[13 * inst] != -1
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                244,
                0,
                "%s",
                "gScrParserGlob[inst].delayedSourceIndex == -1") )
        {
          __debugbreak();
        }
        if ( (type & 1) == 0
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                245,
                0,
                "%s",
                "type & SOURCE_TYPE_BREAKPOINT") )
        {
          __debugbreak();
        }
        dword_9F7A834[13 * inst] = sourcePosLookupIndex;
      }
      else if ( sourcePos == -2 )
      {
        dword_9F7A838[13 * inst] = sourcePosLookupIndex;
      }
      else if ( dword_9F7A834[13 * inst] >= 0 && (type & 1) != 0 )
      {
        *(unsigned int *)(dword_9F7A814[13 * inst] + 8 * dword_9F7A834[13 * inst]) = sourcePos;
        dword_9F7A834[13 * inst] = -1;
      }
      sourcePosLookup->type |= type;
      opcodeLookup->sourcePosCount = ++dword_9F7A828[13 * inst];
      ++dword_9F7A810[13 * inst];
      ++dword_9F7A81C[13 * inst];
    }
  }
}

void __cdecl RemoveOpcodePos(scriptInstance_t inst)
{
  OpcodeLookup *opcodeLookup; // [esp+0h] [ebp-4h]

  if ( MEMORY[0xA05AB86][116 * inst] )
  {
    if ( gScrCompilePub[inst].developer_statement == 2 )
    {
      if ( MEMORY[0xA05AB87][116 * inst] )
      {
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                281,
                0,
                "%s",
                "!gScrVarPub[inst].developer_script") )
          __debugbreak();
      }
    }
    else
    {
      if ( !gScrParserGlob[inst].opcodeLookup
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              285,
              0,
              "%s",
              "gScrParserGlob[inst].opcodeLookup") )
      {
        __debugbreak();
      }
      if ( !dword_9F7A80C[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              286,
              0,
              "%s",
              "gScrParserGlob[inst].opcodeLookupMaxLen") )
      {
        __debugbreak();
      }
      if ( !dword_9F7A814[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              287,
              0,
              "%s",
              "gScrParserGlob[inst].sourcePosLookup") )
      {
        __debugbreak();
      }
      if ( !gScrCompilePub[inst].opcodePos
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              288,
              0,
              "%s",
              "gScrCompilePub[inst].opcodePos") )
      {
        __debugbreak();
      }
      if ( !dword_9F7A81C[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              290,
              0,
              "%s",
              "gScrParserGlob[inst].sourcePosLookupLen") )
      {
        __debugbreak();
      }
      --dword_9F7A81C[13 * inst];
      if ( !dword_9F7A810[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              293,
              0,
              "%s",
              "gScrParserGlob[inst].opcodeLookupLen") )
      {
        __debugbreak();
      }
      --dword_9F7A810[13 * inst];
      if ( !dword_9F7A828[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              296,
              0,
              "%s",
              "gScrParserGlob[inst].currentSourcePosCount") )
      {
        __debugbreak();
      }
      --dword_9F7A828[13 * inst];
      opcodeLookup = &gScrParserGlob[inst].opcodeLookup[dword_9F7A810[13 * inst]];
      if ( (unsigned __int8 *)dword_9F7A824[13 * inst] != gScrCompilePub[inst].opcodePos
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              301,
              0,
              "%s",
              "gScrParserGlob[inst].currentCodePos == gScrCompilePub[inst].opcodePos") )
      {
        __debugbreak();
      }
      if ( dword_9F7A828[13 * inst] + opcodeLookup->sourcePosIndex != dword_9F7A81C[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              302,
              0,
              "%s",
              "opcodeLookup->sourcePosIndex + gScrParserGlob[inst].currentSourcePosCount == gScrParserGlob[inst].sourcePosLookupLen") )
      {
        __debugbreak();
      }
      if ( opcodeLookup->codePos != (const char *)dword_9F7A824[13 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              303,
              0,
              "%s",
              "opcodeLookup->codePos == (char *)gScrParserGlob[inst].currentCodePos") )
      {
        __debugbreak();
      }
      if ( !dword_9F7A828[13 * inst] )
        dword_9F7A824[13 * inst] = 0;
      opcodeLookup->sourcePosCount = dword_9F7A828[13 * inst];
    }
  }
}

void __cdecl AddThreadStartOpcodePos(scriptInstance_t inst, unsigned int sourcePos)
{
  SourceLookup *sourcePosLookup; // [esp+0h] [ebp-4h]

  if ( MEMORY[0xA05AB86][116 * inst] )
  {
    if ( gScrCompilePub[inst].developer_statement == 2 )
    {
      if ( MEMORY[0xA05AB87][116 * inst] )
      {
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                323,
                0,
                "%s",
                "!gScrVarPub[inst].developer_script") )
          __debugbreak();
      }
    }
    else
    {
      if ( dword_9F7A838[13 * inst] < 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              327,
              0,
              "%s",
              "gScrParserGlob[inst].threadStartSourceIndex >= 0") )
      {
        __debugbreak();
      }
      sourcePosLookup = (SourceLookup *)(dword_9F7A814[13 * inst] + 8 * dword_9F7A838[13 * inst]);
      sourcePosLookup->sourcePos = sourcePos;
      if ( sourcePosLookup->type
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              330,
              0,
              "%s",
              "!sourcePosLookup->type") )
      {
        __debugbreak();
      }
      sourcePosLookup->type = 4;
      dword_9F7A838[13 * inst] = -1;
    }
  }
}

const char *__cdecl Scr_GetOpcodePosOfType(
        scriptInstance_t inst,
        unsigned int bufferIndex,
        unsigned int startSourcePos,
        unsigned int endSourcePos,
        int type,
        unsigned int *sourcePos)
{
  const char *codePos; // [esp+0h] [ebp-34h]
  SourceBufferInfo *v8; // [esp+4h] [ebp-30h]
  unsigned int j; // [esp+8h] [ebp-2Ch]
  signed int sourcePosCount; // [esp+10h] [ebp-24h]
  unsigned int firstSourcePos; // [esp+14h] [ebp-20h]
  int k; // [esp+18h] [ebp-1Ch]
  SourceLookup *sourcePosLookup; // [esp+1Ch] [ebp-18h]
  SourceBufferInfo *sourceBufData; // [esp+20h] [ebp-14h]
  const char *startBufferCodePos; // [esp+24h] [ebp-10h]
  const char *firstOpcodePos; // [esp+28h] [ebp-Ch]
  const char *opcodePos; // [esp+2Ch] [ebp-8h]

  sourceBufData = &gScrParserPub[inst].sourceBufferLookup[bufferIndex];
  if ( bufferIndex + 1 >= gScrParserPub[inst].sourceBufferLookupLen )
    v8 = 0;
  else
    v8 = &gScrParserPub[inst].sourceBufferLookup[bufferIndex + 1];
  startBufferCodePos = sourceBufData->codePos;
  if ( sourceBufData->codePos )
  {
    firstOpcodePos = 0;
    firstSourcePos = 0;
    if ( v8 )
      codePos = v8->codePos;
    else
      codePos = 0;
    for ( j = 0; j < dword_9F7A810[13 * inst]; ++j )
    {
      sourcePosCount = gScrParserGlob[inst].opcodeLookup[j].sourcePosCount;
      for ( k = 0; k < sourcePosCount; ++k )
      {
        opcodePos = gScrParserGlob[inst].opcodeLookup[j].codePos;
        if ( opcodePos >= startBufferCodePos )
        {
          if ( opcodePos <= codePos - 1 )
          {
            sourcePosLookup = (SourceLookup *)(dword_9F7A814[13 * inst]
                                             + 8 * (k + gScrParserGlob[inst].opcodeLookup[j].sourcePosIndex));
            if ( (type & sourcePosLookup->type) == type
              && sourcePosLookup->sourcePos >= startSourcePos
              && sourcePosLookup->sourcePos < endSourcePos
              && (!firstOpcodePos || gScrParserGlob[inst].opcodeLookup[j].codePos < firstOpcodePos) )
            {
              firstOpcodePos = gScrParserGlob[inst].opcodeLookup[j].codePos;
              firstSourcePos = sourcePosLookup->sourcePos;
            }
          }
          else if ( !v8
                 && !Assert_MyHandler(
                       "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                       379,
                       0,
                       "%s",
                       "nextSourceBufData") )
          {
            __debugbreak();
          }
        }
      }
    }
    *sourcePos = firstSourcePos;
    return firstOpcodePos;
  }
  else
  {
    *sourcePos = 0;
    return 0;
  }
}

unsigned int __cdecl Scr_GetClosestSourcePosOfType(
        scriptInstance_t inst,
        unsigned int bufferIndex,
        unsigned int sourcePos,
        int type)
{
  const char *codePos; // [esp+0h] [ebp-30h]
  SourceBufferInfo *v6; // [esp+4h] [ebp-2Ch]
  unsigned int j; // [esp+8h] [ebp-28h]
  signed int sourcePosCount; // [esp+10h] [ebp-20h]
  int k; // [esp+14h] [ebp-1Ch]
  SourceLookup *sourcePosLookup; // [esp+18h] [ebp-18h]
  SourceBufferInfo *sourceBufData; // [esp+1Ch] [ebp-14h]
  const char *startBufferCodePos; // [esp+20h] [ebp-10h]
  unsigned int bestSourcePos; // [esp+24h] [ebp-Ch]
  const char *opcodePos; // [esp+28h] [ebp-8h]

  bestSourcePos = 0;
  sourceBufData = &gScrParserPub[inst].sourceBufferLookup[bufferIndex];
  if ( bufferIndex + 1 >= gScrParserPub[inst].sourceBufferLookupLen )
    v6 = 0;
  else
    v6 = &gScrParserPub[inst].sourceBufferLookup[bufferIndex + 1];
  startBufferCodePos = sourceBufData->codePos;
  if ( !sourceBufData->codePos )
    return 0;
  if ( v6 )
    codePos = v6->codePos;
  else
    codePos = 0;
  for ( j = 0; j < dword_9F7A810[13 * inst]; ++j )
  {
    sourcePosCount = gScrParserGlob[inst].opcodeLookup[j].sourcePosCount;
    for ( k = 0; k < sourcePosCount; ++k )
    {
      opcodePos = gScrParserGlob[inst].opcodeLookup[j].codePos;
      if ( opcodePos >= startBufferCodePos )
      {
        if ( opcodePos <= codePos - 1 )
        {
          sourcePosLookup = (SourceLookup *)(dword_9F7A814[13 * inst]
                                           + 8 * (k + gScrParserGlob[inst].opcodeLookup[j].sourcePosIndex));
          if ( (type & sourcePosLookup->type) == type
            && sourcePosLookup->sourcePos >= bestSourcePos
            && sourcePosLookup->sourcePos <= sourcePos )
          {
            bestSourcePos = sourcePosLookup->sourcePos;
          }
        }
        else if ( !v6
               && !Assert_MyHandler(
                     "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
                     439,
                     0,
                     "%s",
                     "nextSourceBufData") )
        {
          __debugbreak();
        }
      }
    }
  }
  return bestSourcePos;
}

unsigned int __cdecl Scr_GetPrevSourcePos(scriptInstance_t inst, const char *codePos, unsigned int index)
{
  return *(unsigned int *)(dword_9F7A814[13 * inst]
                   + 8 * (index + Scr_GetPrevSourcePosOpcodeLookup(inst, codePos)->sourcePosIndex));
}

OpcodeLookup *__cdecl Scr_GetPrevSourcePosOpcodeLookup(scriptInstance_t inst, const char *codePos)
{
  unsigned int low; // [esp+0h] [ebp-Ch]
  unsigned int middle; // [esp+4h] [ebp-8h]
  unsigned int high; // [esp+8h] [ebp-4h]

  if ( !Scr_IsInOpcodeMemory(inst, codePos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          462,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, codePos )") )
  {
    __debugbreak();
  }
  if ( !gScrParserGlob[inst].opcodeLookup
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          463,
          0,
          "%s",
          "gScrParserGlob[inst].opcodeLookup") )
  {
    __debugbreak();
  }
  low = 0;
  high = dword_9F7A810[13 * inst] - 1;
  while ( low <= high )
  {
    middle = (high + low) >> 1;
    if ( codePos < gScrParserGlob[inst].opcodeLookup[middle].codePos )
    {
      high = middle - 1;
    }
    else
    {
      low = middle + 1;
      if ( middle + 1 == dword_9F7A810[13 * inst] || codePos < gScrParserGlob[inst].opcodeLookup[low].codePos )
        return &gScrParserGlob[inst].opcodeLookup[middle];
    }
  }
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 482, 0, "unreachable") )
    __debugbreak();
  return 0;
}

unsigned int __cdecl Scr_GetLineNum(scriptInstance_t inst, unsigned int bufferIndex, unsigned int sourcePos)
{
  const char *startLine; // [esp+0h] [ebp-8h] BYREF
  int col; // [esp+4h] [ebp-4h] BYREF

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          645,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  return Scr_GetLineNumInternal(
           gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf,
           sourcePos,
           &startLine,
           &col);
}

unsigned int __cdecl Scr_GetLineNumInternal(const char *buf, unsigned int sourcePos, const char **startLine, int *col)
{
  unsigned int lineNum; // [esp+0h] [ebp-4h]

  if ( !buf
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 618, 0, "%s", "buf") )
  {
    __debugbreak();
  }
  *startLine = buf;
  lineNum = 0;
  while ( sourcePos )
  {
    if ( !*buf )
    {
      *startLine = buf + 1;
      ++lineNum;
    }
    ++buf;
    --sourcePos;
  }
  *col = buf - *startLine;
  return lineNum;
}

unsigned int __cdecl Scr_GetFunctionLineNumInternal(const char *buf, unsigned int sourcePos, const char **startLine)
{
  unsigned int lineNum; // [esp+0h] [ebp-Ch]
  unsigned int functionLine; // [esp+4h] [ebp-8h]
  const char *functionStartLine; // [esp+8h] [ebp-4h]

  if ( !buf
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 564, 0, "%s", "buf") )
  {
    __debugbreak();
  }
  *startLine = buf;
  lineNum = 0;
  functionLine = 0;
  functionStartLine = buf;
  while ( sourcePos )
  {
    if ( !*buf )
    {
      if ( buf[1] == 123 )
      {
        functionLine = lineNum;
        functionStartLine = *startLine;
      }
      *startLine = buf + 1;
      ++lineNum;
    }
    ++buf;
    --sourcePos;
  }
  *startLine = functionStartLine;
  return functionLine;
}

int __cdecl Scr_GetSourcePosOfType(scriptInstance_t inst, const char *codePos, int type, Scr_SourcePos_t *pos)
{
  OpcodeLookup *SourcePosOpcodeLookup; // [esp+0h] [ebp-Ch]
  unsigned int index; // [esp+8h] [ebp-4h]

  if ( type )
    SourcePosOpcodeLookup = Scr_GetSourcePosOpcodeLookup(inst, codePos);
  else
    SourcePosOpcodeLookup = Scr_GetPrevSourcePosOpcodeLookup(inst, codePos);
  if ( SourcePosOpcodeLookup )
  {
    for ( index = 0; index < SourcePosOpcodeLookup->sourcePosCount; ++index )
    {
      if ( (type & *(unsigned int *)(dword_9F7A814[13 * inst] + 8 * (index + SourcePosOpcodeLookup->sourcePosIndex) + 4)) == type )
      {
        pos->sourcePos = *(unsigned int *)(dword_9F7A814[13 * inst] + 8 * (index + SourcePosOpcodeLookup->sourcePosIndex));
        pos->bufferIndex = Scr_GetSourceBuffer(inst, codePos);
        pos->lineNum = Scr_GetLineNum(inst, pos->bufferIndex, pos->sourcePos);
        return 1;
      }
    }
  }
  pos->sourcePos = 0;
  pos->bufferIndex = 0;
  pos->lineNum = 0;
  return 0;
}

OpcodeLookup *__cdecl Scr_GetSourcePosOpcodeLookup(scriptInstance_t inst, const char *codePos)
{
  int low; // [esp+0h] [ebp-Ch]
  int middle; // [esp+4h] [ebp-8h]
  int high; // [esp+8h] [ebp-4h]

  if ( !Scr_IsInOpcodeMemory(inst, codePos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          523,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, codePos )") )
  {
    __debugbreak();
  }
  if ( !gScrParserGlob[inst].opcodeLookup
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          524,
          0,
          "%s",
          "gScrParserGlob[inst].opcodeLookup") )
  {
    __debugbreak();
  }
  low = 0;
  high = dword_9F7A810[13 * inst] - 1;
  while ( low <= high )
  {
    middle = (high + low) / 2;
    if ( codePos < gScrParserGlob[inst].opcodeLookup[middle].codePos )
    {
      high = middle - 1;
    }
    else
    {
      if ( codePos == gScrParserGlob[inst].opcodeLookup[middle].codePos )
        return &gScrParserGlob[inst].opcodeLookup[middle];
      low = middle + 1;
    }
  }
  return 0;
}

void __cdecl Scr_SendSource(scriptInstance_t inst)
{
  unsigned int i; // [esp+0h] [ebp-8h]
  SourceBufferInfo *sourceBufferLookup; // [esp+4h] [ebp-4h]

  if ( !Sys_IsRemoteDebugServer()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          818,
          0,
          "%s",
          "Sys_IsRemoteDebugServer()") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AB86][116 * inst] )
  {
    Sys_WriteDebugSocketMessageType(2u);
    Sys_EndWriteDebugSocket();
    for ( i = 0; i < gScrParserPub[inst].sourceBufferLookupLen; ++i )
    {
      sourceBufferLookup = &gScrParserPub[inst].sourceBufferLookup[i];
      if ( sourceBufferLookup->codePos )
      {
        Sys_WriteDebugSocketMessageType(1u);
        Sys_WriteDebugSocketString(sourceBufferLookup->buf);
        Sys_WriteDebugSocketInt(sourceBufferLookup->len);
        if ( sourceBufferLookup->len > 0 )
          Sys_WriteDebugSocketData((unsigned __int8 *)sourceBufferLookup->sourceBuf, sourceBufferLookup->len);
        Sys_EndWriteDebugSocket();
      }
    }
    Sys_WriteDebugSocketMessageType(3u);
    Sys_EndWriteDebugSocket();
  }
}

char *__cdecl Scr_AddSourceBuffer(
        scriptInstance_t inst,
        const char *filename,
        char *extFilename,
        const char *codePos,
        bool archive)
{
  char v6; // [esp+3h] [ebp-1Dh]
  const char *source; // [esp+4h] [ebp-1Ch]
  char c; // [esp+Bh] [ebp-15h]
  SaveSourceBufferInfo *saveSourceBuffer; // [esp+Ch] [ebp-14h]
  int len; // [esp+10h] [ebp-10h]
  char *dest; // [esp+14h] [ebp-Ch]
  int i; // [esp+18h] [ebp-8h]
  char *sourceBuf; // [esp+1Ch] [ebp-4h]

  if ( !archive || !dword_9F7A82C[13 * inst] )
    return Scr_ReadFile(inst, filename, extFilename, codePos, archive);
  if ( !dword_9F7A830[13 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1010,
          0,
          "%s",
          "gScrParserGlob[inst].saveSourceBufferLookupLen > 0") )
  {
    __debugbreak();
  }
  --dword_9F7A830[13 * inst];
  saveSourceBuffer = (SaveSourceBufferInfo *)(dword_9F7A82C[13 * inst] + 8 * dword_9F7A830[13 * inst]);
  len = saveSourceBuffer->len;
  if ( len < -1
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 1014, 0, "%s", "len >= -1") )
  {
    __debugbreak();
  }
  if ( len >= 0 )
  {
    sourceBuf = (char *)Hunk_AllocateTempMemoryHigh(len + 1, "Scr_AddSourceBuffer1");
    source = saveSourceBuffer->sourceBuf;
    dest = sourceBuf;
    for ( i = 0; i < len; ++i )
    {
      c = *source++;
      if ( c )
        v6 = c;
      else
        v6 = 10;
      *dest++ = v6;
    }
    *dest = 0;
    if ( saveSourceBuffer->sourceBuf )
      Hunk_UserFree(g_DebugHunkUser, *(void **)(dword_9F7A82C[13 * inst] + 8 * dword_9F7A830[13 * inst]));
    Scr_AddSourceBufferInternal(inst, extFilename, codePos, sourceBuf, len, 1, archive);
  }
  else
  {
    sourceBuf = 0;
    Scr_AddSourceBufferInternal(inst, extFilename, codePos, 0, len, 1, archive);
  }
  return sourceBuf;
}

void __cdecl Scr_AddSourceBufferInternal(
        scriptInstance_t inst,
        const char *extFilename,
        const char *codePos,
        char *sourceBuf,
        int len,
        bool doEolFixup,
        bool archive)
{
  SourceBufferInfo *NewSourceBuffer; // eax
  const char *v8; // [esp+0h] [ebp-48h]
  char v9; // [esp+7h] [ebp-41h]
  char *v10; // [esp+Ch] [ebp-3Ch]
  const char *v11; // [esp+10h] [ebp-38h]
  unsigned int v12; // [esp+14h] [ebp-34h]
  const char *source; // [esp+2Ch] [ebp-1Ch]
  char c; // [esp+33h] [ebp-15h]
  char ca; // [esp+33h] [ebp-15h]
  char *buf; // [esp+34h] [ebp-14h]
  char *dest; // [esp+40h] [ebp-8h]
  int i; // [esp+44h] [ebp-4h]
  int ia; // [esp+44h] [ebp-4h]

  if ( gScrParserPub[inst].sourceBufferLookup )
  {
    v12 = strlen(extFilename);
    buf = (char *)Hunk_UserAlloc(g_DebugHunkUser, v12 + 1 + len + 2, 4, "Scr_AddSourceBuffer3");
    v11 = extFilename;
    v10 = buf;
    do
    {
      v9 = *v11;
      *v10++ = *v11++;
    }
    while ( v9 );
    if ( sourceBuf )
      v8 = &buf[v12 + 1];
    else
      v8 = 0;
    source = sourceBuf;
    dest = (char *)v8;
    if ( doEolFixup )
    {
      for ( i = 0; i <= len; ++i )
      {
        c = *source++;
        if ( c == 10 || c == 13 && *source != 10 )
          *dest = 0;
        else
          *dest = c;
        ++dest;
      }
    }
    else
    {
      for ( ia = 0; ia <= len; ++ia )
      {
        ca = *source++;
        *dest++ = ca;
      }
    }
    NewSourceBuffer = Scr_GetNewSourceBuffer(inst);
    NewSourceBuffer->codePos = codePos;
    NewSourceBuffer->buf = buf;
    NewSourceBuffer->sourceBuf = v8;
    NewSourceBuffer->len = len;
    NewSourceBuffer->sortedIndex = -1;
    NewSourceBuffer->archive = archive;
    BG_EvalVehicleName();
    if ( v8 )
      gScrParserPub[inst].sourceBuf = v8;
  }
  else
  {
    gScrParserPub[inst].sourceBuf = 0;
  }
}

SourceBufferInfo *__cdecl Scr_GetNewSourceBuffer(scriptInstance_t inst)
{
  unsigned __int8 *newSourceBufferInfo; // [esp+4h] [ebp-4h]

  if ( !gScrParserPub[inst].sourceBufferLookup
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          700,
          0,
          "%s",
          "gScrParserPub[inst].sourceBufferLookup") )
  {
    __debugbreak();
  }
  if ( !dword_9F7A820[13 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          701,
          0,
          "%s",
          "gScrParserGlob[inst].sourceBufferLookupMaxLen") )
  {
    __debugbreak();
  }
  if ( gScrParserPub[inst].sourceBufferLookupLen >= dword_9F7A820[13 * inst] )
  {
    dword_9F7A820[13 * inst] += 0x4000;
    if ( gScrParserPub[inst].sourceBufferLookupLen >= dword_9F7A820[13 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
            706,
            0,
            "%s",
            "gScrParserPub[inst].sourceBufferLookupLen < gScrParserGlob[inst].sourceBufferLookupMaxLen") )
    {
      __debugbreak();
    }
    newSourceBufferInfo = (unsigned __int8 *)Hunk_UserAlloc(
                                               g_DebugHunkUser,
                                               24 * dword_9F7A820[13 * inst],
                                               4,
                                               "Scr_AddSourceBuffer4");
    memset(newSourceBufferInfo, 0, 24 * dword_9F7A820[13 * inst]);
    Com_Memcpy(
      newSourceBufferInfo,
      gScrParserPub[inst].sourceBufferLookup,
      24 * gScrParserPub[inst].sourceBufferLookupLen);
    Hunk_UserFree(g_DebugHunkUser, gScrParserPub[inst].sourceBufferLookup);
    gScrParserPub[inst].sourceBufferLookup = (SourceBufferInfo *)newSourceBufferInfo;
  }
  return &gScrParserPub[inst].sourceBufferLookup[gScrParserPub[inst].sourceBufferLookupLen++];
}

char *__cdecl Scr_ReadFile(
        scriptInstance_t inst,
        const char *filename,
        char *extFilename,
        const char *codePos,
        bool archive)
{
  int file; // [esp+34h] [ebp-4h] BYREF

  if ( fs_gameDirVar && *(_BYTE *)fs_gameDirVar->current.integer )
  {
    if ( (FS_FOpenFileRead(extFilename, &file) & 0x80000000) != 0 )
    {
      return Scr_ReadFile_FastFile(inst, filename, extFilename, codePos, archive);
    }
    else
    {
      FS_FCloseFile(file);
      return Scr_ReadFile_LoadObj(inst, filename, extFilename, codePos, archive);
    }
  }
  else if ( useFastFile->current.enabled )
  {
    return Scr_ReadFile_FastFile(inst, filename, extFilename, codePos, archive);
  }
  else
  {
    return Scr_ReadFile_LoadObj(inst, filename, extFilename, codePos, archive);
  }
}

char *__cdecl Scr_ReadFile_FastFile(
        scriptInstance_t inst,
        const char *filename,
        const char *extFilename,
        const char *codePos,
        bool archive)
{
  unsigned int outlen; // [esp+14h] [ebp-18h] BYREF
  unsigned int inlen; // [esp+18h] [ebp-14h]
  RawFile *rawfile; // [esp+1Ch] [ebp-10h]
  const char *extension; // [esp+20h] [ebp-Ch]
  int len; // [esp+24h] [ebp-8h]
  char *sourceBuf; // [esp+28h] [ebp-4h]

  if ( useFastFile->current.enabled
    && (rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, extFilename, 1, -1).rawfile) != 0 )
  {
    extension = Com_GetExtensionSubString(extFilename);
    if ( I_stricmp(extension, ".gsc") && I_stricmp(extension, ".csc") )
    {
      sourceBuf = (char *)rawfile->buffer;
      len = strlen(sourceBuf);
    }
    else
    {
      len = *(unsigned int *)rawfile->buffer;
      outlen = len;
      inlen = *((unsigned int *)rawfile->buffer + 1);
      sourceBuf = (char *)Hunk_AllocateTempMemoryHigh(len, "Scr_ReadFile");
      uncompress((unsigned __int8 *)sourceBuf, &outlen, (unsigned __int8 *)rawfile->buffer + 8, inlen);
      if ( len != outlen
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
              882,
              0,
              "%s",
              "(uLongf)len==outlen") )
      {
        __debugbreak();
      }
    }
    Scr_AddSourceBufferInternal(inst, extFilename, codePos, sourceBuf, len, 1, archive);
    return sourceBuf;
  }
  else
  {
    Scr_AddSourceBufferInternal(inst, extFilename, codePos, 0, -1, 1, archive);
    return 0;
  }
}

char *__cdecl Scr_ReadFile_LoadObj(
        scriptInstance_t inst,
        const char *filename,
        char *extFilename,
        const char *codePos,
        bool archive)
{
  int len; // [esp+4h] [ebp-Ch]
  int f; // [esp+8h] [ebp-8h] BYREF
  char *sourceBuf; // [esp+Ch] [ebp-4h]

  len = FS_FOpenFileByMode(extFilename, &f, FS_READ);
  if ( len >= 0 )
  {
    if ( !fs_gameDirVar || !*(_BYTE *)fs_gameDirVar->current.integer )
      *(_BYTE *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8264) = 1;
    sourceBuf = (char *)Hunk_AllocateTempMemoryHigh(len + 1, "Scr_ReadFile");
    FS_Read((unsigned __int8 *)sourceBuf, len, f);
    sourceBuf[len] = 0;
    FS_FCloseFile(f);
    Scr_AddSourceBufferInternal(inst, extFilename, codePos, sourceBuf, len, 1, archive);
    return sourceBuf;
  }
  else
  {
    Scr_AddSourceBufferInternal(inst, extFilename, codePos, 0, -1, 1, archive);
    return 0;
  }
}

unsigned int __cdecl Scr_GetSourcePos(
        scriptInstance_t inst,
        unsigned int bufferIndex,
        unsigned int sourcePos,
        char *outBuf,
        unsigned int outBufLen)
{
  unsigned int lineNum; // [esp+4h] [ebp-40Ch]
  char line[1024]; // [esp+8h] [ebp-408h] BYREF
  int col; // [esp+40Ch] [ebp-4h] BYREF

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1190,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  lineNum = Scr_GetLineInfo(gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf, sourcePos, &col, line);
  if ( dword_9F7A82C[13 * inst] )
    Com_sprintf(
      outBuf,
      outBufLen,
      "%s // %s%s, line %d",
      line,
      gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf,
      " (savegame)",
      lineNum + 1);
  else
    Com_sprintf(
      outBuf,
      outBufLen,
      "%s // %s%s, line %d",
      line,
      gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf,
      &toastPopupTitle,
      lineNum + 1);
  return lineNum;
}

unsigned int __cdecl Scr_GetLineInfo(const char *buf, unsigned int sourcePos, int *col, char *line)
{
  const char *startLine; // [esp+0h] [ebp-8h] BYREF
  unsigned int lineNum; // [esp+4h] [ebp-4h]

  if ( buf )
  {
    lineNum = Scr_GetLineNumInternal(buf, sourcePos, &startLine, col);
  }
  else
  {
    lineNum = 0;
    startLine = &toastPopupTitle;
    *col = 0;
  }
  Scr_CopyFormattedLine(line, startLine);
  return lineNum;
}

void __cdecl Scr_CopyFormattedLine(char *line, const char *rawLine)
{
  char v2; // [esp+3h] [ebp-1Dh]
  int len; // [esp+18h] [ebp-8h]
  int i; // [esp+1Ch] [ebp-4h]

  len = strlen(rawLine);
  if ( len >= 1024 )
    len = 1023;
  for ( i = 0; i < len; ++i )
  {
    if ( rawLine[i] == 9 )
      v2 = 32;
    else
      v2 = rawLine[i];
    line[i] = v2;
  }
  if ( line[len - 1] == 13 )
    line[len - 1] = 0;
  line[len] = 0;
}

unsigned int __cdecl Scr_GetSourceBuffer(scriptInstance_t inst, const char *codePos)
{
  unsigned int bufferIndex; // [esp+0h] [ebp-4h]

  if ( !Scr_IsInOpcodeMemory(inst, codePos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1202,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, codePos )") )
  {
    __debugbreak();
  }
  if ( !gScrParserPub[inst].sourceBufferLookupLen
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1203,
          0,
          "%s",
          "gScrParserPub[inst].sourceBufferLookupLen > 0") )
  {
    __debugbreak();
  }
  for ( bufferIndex = gScrParserPub[inst].sourceBufferLookupLen - 1;
        bufferIndex
     && (!gScrParserPub[inst].sourceBufferLookup[bufferIndex].codePos
      || gScrParserPub[inst].sourceBufferLookup[bufferIndex].codePos > codePos);
        --bufferIndex )
  {
    ;
  }
  return bufferIndex;
}

void __cdecl Scr_PrintPrevCodePos(scriptInstance_t inst, int channel, char *codePos, unsigned int index)
{
  const char *v4; // eax
  unsigned int PrevSourcePos; // eax
  const char *v6; // eax
  unsigned int bufferIndex; // [esp+0h] [ebp-4h]

  if ( !codePos )
  {
    Com_PrintMessage(channel, "<frozen thread>\n", 0);
    return;
  }
  if ( codePos == &g_EndPos )
  {
    Com_PrintMessage(channel, "<removed thread>\n", 0);
  }
  else
  {
    if ( MEMORY[0xA05AB86][116 * inst] )
    {
      if ( MEMORY[0xA05ABC8][29 * inst] && Scr_IsInOpcodeMemory(inst, codePos) )
      {
        bufferIndex = Scr_GetSourceBuffer(inst, codePos - 1);
        PrevSourcePos = Scr_GetPrevSourcePos(inst, codePos - 1, index);
        Scr_PrintSourcePos(
          inst,
          channel,
          gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf,
          gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf,
          PrevSourcePos);
        return;
      }
    }
    else if ( Scr_IsInOpcodeMemory(inst, codePos - 1) )
    {
      v4 = va("@ %d\n", &codePos[-MEMORY[0xA05ABC8][29 * inst]]);
      Com_PrintMessage(channel, v4, 0);
      return;
    }
    v6 = va("%s\n\n", codePos);
    Com_PrintMessage(channel, v6, 0);
  }
}

void __cdecl Scr_PrintSourcePos(
        scriptInstance_t inst,
        int channel,
        const char *filename,
        const char *buf,
        unsigned int sourcePos)
{
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // [esp+0h] [ebp-418h]
  unsigned int lineNum; // [esp+4h] [ebp-414h]
  char line[1028]; // [esp+8h] [ebp-410h] BYREF
  int i; // [esp+410h] [ebp-8h]
  int col; // [esp+414h] [ebp-4h] BYREF

  if ( !filename
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 1136, 0, "%s", "filename") )
  {
    __debugbreak();
  }
  lineNum = Scr_GetLineInfo(buf, sourcePos, &col, line);
  if ( dword_9F7A82C[13 * inst] )
    v7 = " (savegame)";
  else
    v7 = &toastPopupTitle;
  v5 = va("(file '%s'%s, line %d)\n", filename, v7, lineNum + 1);
  Com_PrintMessage(channel, v5, 0);
  v6 = va("%s\n", line);
  Com_PrintMessage(channel, v6, 0);
  for ( i = 0; i < col; ++i )
    Com_PrintMessage(channel, " ", 0);
  Com_PrintMessage(channel, "*\n", 0);
}

char *__cdecl Scr_PrevCodePosFileName(scriptInstance_t inst, char *codePos)
{
  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1319,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  if ( !codePos )
    return "<frozen thread>";
  if ( codePos == &g_EndPos )
    return "<removed thread>";
  if ( MEMORY[0xA05ABC8][29 * inst] && Scr_IsInOpcodeMemory(inst, codePos) )
    return gScrParserPub[inst].sourceBufferLookup[Scr_GetSourceBuffer(inst, codePos - 1)].buf;
  return codePos;
}

const char *__cdecl Scr_PrevCodePosFunctionName(scriptInstance_t inst, char *codePos)
{
  unsigned int PrevSourcePos; // eax
  unsigned int bufferIndex; // [esp+0h] [ebp-8h]
  const char *startLine; // [esp+4h] [ebp-4h] BYREF

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1367,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  if ( !codePos )
    return "<frozen thread>";
  if ( codePos == &g_EndPos )
    return "<removed thread>";
  if ( !MEMORY[0xA05ABC8][29 * inst] || !Scr_IsInOpcodeMemory(inst, codePos) )
    return "<unknown>";
  bufferIndex = Scr_GetSourceBuffer(inst, codePos - 1);
  PrevSourcePos = Scr_GetPrevSourcePos(inst, codePos - 1, 0);
  Scr_GetFunctionLineNumInternal(
    gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf,
    PrevSourcePos,
    &startLine);
  return startLine;
}

bool __cdecl Scr_PrevCodePosFileNameMatches(scriptInstance_t inst, char *codePos, const char *fileName)
{
  char *codePosFileName; // [esp+0h] [ebp-4h]

  if ( !fileName
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 1422, 0, "%s", "fileName") )
  {
    __debugbreak();
  }
  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1423,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  codePosFileName = Scr_PrevCodePosFileName(inst, codePos);
  return codePosFileName && I_stristr(codePosFileName, fileName) != 0;
}

void __cdecl Scr_PrintPrevCodePosSpreadSheet(
        scriptInstance_t inst,
        int channel,
        char *codePos,
        bool summary,
        bool functionSummary)
{
  unsigned int PrevSourcePos; // eax
  const char *v6; // eax
  unsigned int bufferIndex; // [esp+0h] [ebp-4h]

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1438,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  if ( codePos )
  {
    if ( codePos == &g_EndPos )
    {
      Com_PrintMessage(channel, "<removed thread>\n", 0);
    }
    else if ( MEMORY[0xA05ABC8][29 * inst] && Scr_IsInOpcodeMemory(inst, codePos) )
    {
      bufferIndex = Scr_GetSourceBuffer(inst, codePos - 1);
      if ( summary )
      {
        Scr_GetPrevSourcePos(inst, codePos - 1, 0);
        Scr_PrintSourcePosSummary(inst, channel, gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf);
      }
      else
      {
        PrevSourcePos = Scr_GetPrevSourcePos(inst, codePos - 1, 0);
        if ( functionSummary )
          Scr_PrintFunctionPosSpreadSheet(
            inst,
            channel,
            gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf,
            gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf,
            PrevSourcePos);
        else
          Scr_PrintSourcePosSpreadSheet(
            inst,
            channel,
            gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf,
            gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf,
            PrevSourcePos);
      }
    }
    else
    {
      v6 = va("%s\n", codePos);
      Com_PrintMessage(channel, v6, 0);
    }
  }
  else
  {
    Com_PrintMessage(channel, "<frozen thread>\n", 0);
  }
}

void __cdecl Scr_PrintSourcePosSpreadSheet(
        scriptInstance_t inst,
        int channel,
        const char *filename,
        const char *buf,
        unsigned int sourcePos)
{
  const char *v5; // eax
  const char *v6; // [esp+0h] [ebp-410h]
  unsigned int lineNum; // [esp+4h] [ebp-40Ch]
  char line[1024]; // [esp+8h] [ebp-408h] BYREF
  int col; // [esp+40Ch] [ebp-4h] BYREF

  if ( !filename
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 1155, 0, "%s", "filename") )
  {
    __debugbreak();
  }
  lineNum = Scr_GetLineInfo(buf, sourcePos, &col, line);
  if ( dword_9F7A82C[13 * inst] )
    v6 = "(savegame)";
  else
    v6 = &toastPopupTitle;
  v5 = va("%s%s\t%d\t%s\t%d\n", filename, v6, lineNum + 1, line, col);
  Com_PrintMessage(channel, v5, 0);
}

void __cdecl Scr_PrintFunctionPosSpreadSheet(
        scriptInstance_t inst,
        int channel,
        const char *filename,
        const char *buf,
        unsigned int sourcePos)
{
  const char *v5; // eax
  const char *v6; // [esp+0h] [ebp-410h]
  unsigned int lineNum; // [esp+4h] [ebp-40Ch]
  char line[1028]; // [esp+8h] [ebp-408h] BYREF

  if ( !filename
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 1168, 0, "%s", "filename") )
  {
    __debugbreak();
  }
  lineNum = Scr_GetFunctionInfo(buf, sourcePos, line);
  if ( dword_9F7A82C[13 * inst] )
    v6 = "(savegame)";
  else
    v6 = &toastPopupTitle;
  v5 = va("%s%s\t%d\t%s\n", filename, v6, lineNum + 1, line);
  Com_PrintMessage(channel, v5, 0);
}

unsigned int __cdecl Scr_GetFunctionInfo(const char *buf, unsigned int sourcePos, char *line)
{
  const char *startLine; // [esp+0h] [ebp-8h] BYREF
  unsigned int lineNum; // [esp+4h] [ebp-4h]

  lineNum = Scr_GetFunctionLineNumInternal(buf, sourcePos, &startLine);
  Scr_CopyFormattedLine(line, startLine);
  return lineNum;
}

void __cdecl Scr_PrintSourcePosSummary(scriptInstance_t inst, int channel, const char *filename)
{
  const char *v3; // eax
  const char *v4; // [esp+0h] [ebp-4h]

  if ( !filename
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp", 1178, 0, "%s", "filename") )
  {
    __debugbreak();
  }
  if ( dword_9F7A82C[13 * inst] )
    v4 = "(savegame)";
  else
    v4 = &toastPopupTitle;
  v3 = va("%s%s\t\n", filename, v4);
  Com_PrintMessage(channel, v3, 0);
}

void __cdecl Scr_GetCodePos(
        scriptInstance_t inst,
        const char *codePos,
        unsigned int index,
        char *outBuf,
        unsigned int outBufLen)
{
  Scr_SourcePos_t pos; // [esp+0h] [ebp-Ch] BYREF

  if ( MEMORY[0xA05AB86][116 * inst] )
  {
    Scr_GetSourcePosOfType(inst, codePos, 4, &pos);
    Scr_GetSourcePos(inst, pos.bufferIndex, pos.sourcePos, outBuf, outBufLen);
  }
  else
  {
    if ( !Scr_IsInOpcodeMemory(inst, codePos)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
            1474,
            0,
            "%s",
            "Scr_IsInOpcodeMemory( inst, codePos )") )
    {
      __debugbreak();
    }
    Com_sprintf(outBuf, outBufLen, "@ %d", &codePos[-MEMORY[0xA05ABC8][29 * inst]]);
  }
}

void __cdecl Scr_GetFileAndLine(scriptInstance_t inst, const char *codePos, char **filename, int *linenum)
{
  unsigned int bufferIndex; // [esp+0h] [ebp-Ch]
  OpcodeLookup *opcodeLookup; // [esp+4h] [ebp-8h]
  unsigned int sourcePos; // [esp+8h] [ebp-4h]

  if ( !Scr_IsInOpcodeMemory(inst, codePos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1489,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, codePos )") )
  {
    __debugbreak();
  }
  opcodeLookup = Scr_GetPrevSourcePosOpcodeLookup(inst, codePos);
  if ( opcodeLookup )
  {
    sourcePos = *(unsigned int *)(dword_9F7A814[13 * inst] + 8 * opcodeLookup->sourcePosIndex);
    bufferIndex = Scr_GetSourceBuffer(inst, codePos);
    *linenum = Scr_GetLineNum(inst, bufferIndex, sourcePos) + 1;
    *filename = gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf;
  }
  else
  {
    *linenum = 0;
    *filename = (char *)&toastPopupTitle;
  }
}

void CompileError(scriptInstance_t inst, unsigned int sourcePos, const char *msg, ...)
{
  char line[1028]; // [esp+10h] [ebp-810h] BYREF
  int lineNumber; // [esp+414h] [ebp-40Ch]
  char text[1024]; // [esp+418h] [ebp-408h] BYREF
  int col; // [esp+81Ch] [ebp-4h] BYREF
  va_list va; // [esp+834h] [ebp+14h] BYREF

  va_start(va, msg);
  lineNumber = 0;
  _vsnprintf(text, 0x400u, msg, va);
  if ( MEMORY[0xA05AB88][116 * inst] )
  {
    if ( !MEMORY[0xA05AB8C][29 * inst] )
      MEMORY[0xA05AB8C][29 * inst] = (int)::va("%s", text);
  }
  else
  {
    Monkey_GrabComPrints(1);
    Scr_IgnoreLeaks(inst);
    Scr_ShutdownAllocNode(inst);
    Com_PrintError(24, "\n");
    if ( inst )
      Com_PrintError(24, "******* %s script compile error *******\n", "Client");
    else
      Com_PrintError(24, "******* %s script compile error *******\n", "Server");
    if ( MEMORY[0xA05AB86][116 * inst] && gScrParserPub[inst].sourceBuf )
    {
      Com_PrintError(24, "%s: ", text);
      Scr_PrintSourcePos(inst, 24, gScrParserPub[inst].scriptfilename, gScrParserPub[inst].sourceBuf, sourcePos);
      lineNumber = Scr_GetLineInfo(gScrParserPub[inst].sourceBuf, sourcePos, &col, line);
      Monkey_Error(0);
      if ( inst )
        Com_Error(ERR_SCRIPT_DROP, &byte_D26C38, "Client", text, gScrParserPub[inst].scriptfilename, lineNumber, line);
      else
        Com_Error(ERR_SCRIPT_DROP, &byte_D26C38, "Server", text, gScrParserPub[0].scriptfilename, lineNumber, line);
    }
    else
    {
      Com_PrintError(24, "%s\n", text);
      line[0] = 0;
      Com_Printf(24, "************************************\n");
      Monkey_Error(0);
      if ( inst )
        Com_Error(ERR_SCRIPT_DROP, &byte_D26C9C, "Client", text, line);
      else
        Com_Error(ERR_SCRIPT_DROP, &byte_D26C9C, "Server", text, line);
    }
  }
}

void __cdecl Scr_IgnoreLeaks(scriptInstance_t inst)
{
  if ( gScrStringDebugGlob[inst] )
    gScrStringDebugGlob[inst]->ignoreLeaks = 1;
  Scr_ClearScrVarDebugPub(inst);
}

void CompileError2(scriptInstance_t inst, char *codePos, const char *msg, ...)
{
  char line[1024]; // [esp+Ch] [ebp-808h] BYREF
  char text[1028]; // [esp+40Ch] [ebp-408h] BYREF
  va_list va; // [esp+828h] [ebp+14h] BYREF

  va_start(va, msg);
  if ( MEMORY[0xA05AB88][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1894,
          0,
          "%s",
          "!gScrVarPub[inst].evaluate") )
  {
    __debugbreak();
  }
  if ( !Scr_IsInOpcodeMemory(inst, codePos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1895,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, codePos )") )
  {
    __debugbreak();
  }
  Monkey_GrabComPrints(1);
  Scr_IgnoreLeaks(inst);
  Com_PrintError(24, "\n");
  if ( inst )
    Com_PrintError(24, "******* %s script compile error *******\n", "Client");
  else
    Com_PrintError(24, "******* %s script compile error *******\n", "Server");
  _vsnprintf(text, 0x400u, msg, va);
  Com_PrintError(24, "%s: ", text);
  Scr_PrintPrevCodePos(inst, 24, codePos, 0);
  Com_Printf(24, "************************************\n");
  Monkey_Error(0);
  Scr_GetTextSourcePos(inst, gScrParserPub[inst].sourceBuf, codePos, line);
  if ( inst )
    Com_Error(ERR_SCRIPT_DROP, &byte_D26D34, "Client", text, line);
  else
    Com_Error(ERR_SCRIPT_DROP, &byte_D26D34, "Server", text, line);
}

void __cdecl Scr_GetTextSourcePos(scriptInstance_t inst, const char *buf, char *codePos, char *line)
{
  unsigned int PrevSourcePos; // eax
  unsigned int bufferIndex; // [esp+0h] [ebp-8h]
  int col; // [esp+4h] [ebp-4h] BYREF

  if ( MEMORY[0xA05AB86][116 * inst]
    && codePos
    && codePos != &g_EndPos
    && MEMORY[0xA05ABC8][29 * inst]
    && Scr_IsInOpcodeMemory(inst, codePos) )
  {
    bufferIndex = Scr_GetSourceBuffer(inst, codePos - 1);
    PrevSourcePos = Scr_GetPrevSourcePos(inst, codePos - 1, 0);
    Scr_GetLineInfo(gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf, PrevSourcePos, &col, line);
  }
  else
  {
    *line = 0;
  }
}

void __cdecl RuntimeError(
        scriptInstance_t inst,
        char *codePos,
        unsigned int index,
        const char *msg,
        const char *dialogMessage)
{
  const char *v5; // [esp+4h] [ebp-428h]
  const char *v6; // [esp+8h] [ebp-424h]
  bool v7; // [esp+Ch] [ebp-420h]
  bool abort_on_error; // [esp+13h] [ebp-419h]
  int bufferIndex; // [esp+14h] [ebp-418h]
  unsigned int lineNum; // [esp+18h] [ebp-414h]
  char line[1028]; // [esp+1Ch] [ebp-410h] BYREF
  int sourcePos; // [esp+424h] [ebp-8h]
  int col; // [esp+428h] [ebp-4h] BYREF

  if ( MEMORY[0xA05AB86][116 * inst] )
    goto LABEL_30;
  if ( !Scr_IsInOpcodeMemory(inst, codePos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1962,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, codePos )") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AC9E][17192 * inst] )
  {
LABEL_30:
    if ( MEMORY[0xA05AC9C][17192 * inst] )
    {
      Com_Printf(24, "%s\n", msg);
      if ( !MEMORY[0xA05AC9E][17192 * inst] )
        return;
      goto error;
    }
    v7 = MEMORY[0xA05AC9D][17192 * inst] || MEMORY[0xA05AC9E][17192 * inst];
    abort_on_error = v7;
    if ( Scr_IgnoreErrors(inst) && inst != SCRIPTINSTANCE_CLIENT )
      abort_on_error = 0;
    RuntimeErrorInternal(inst, abort_on_error ? 24 : 6, codePos, index, msg);
    if ( abort_on_error )
    {
error:
      if ( MEMORY[0xA05AC9E][17192 * inst] )
        Scr_IgnoreLeaks(inst);
      Monkey_Error(0);
      bufferIndex = Scr_GetSourceBuffer(inst, codePos - 1);
      sourcePos = Scr_GetPrevSourcePos(inst, codePos - 1, index);
      lineNum = Scr_GetLineInfo(gScrParserPub[inst].sourceBufferLookup[bufferIndex].sourceBuf, sourcePos, &col, line);
      if ( dialogMessage )
        v6 = dialogMessage;
      else
        v6 = &toastPopupTitle;
      if ( dialogMessage )
        v5 = "\n";
      else
        v5 = &toastPopupTitle;
      if ( inst == SCRIPTINSTANCE_CLIENT )
        Com_Error(
          (errorParm_t)((MEMORY[0xA05EFC6] != 0) + 4),
          &byte_D26D70,
          "client",
          msg,
          v5,
          v6,
          gScrParserPub[1].sourceBufferLookup[bufferIndex].buf,
          lineNum,
          line);
      else
        Com_Error(
          (errorParm_t)((MEMORY[0xA05AC9E][17192 * inst] != 0) + 4),
          &byte_D26D70,
          "server",
          msg,
          v5,
          v6,
          gScrParserPub[inst].sourceBufferLookup[bufferIndex].buf,
          lineNum,
          line);
    }
  }
}

void __cdecl RuntimeErrorInternal(
        scriptInstance_t inst,
        int channel,
        char *codePos,
        unsigned int index,
        const char *msg)
{
  int i; // [esp+4h] [ebp-4h]

  if ( !Scr_IsInOpcodeMemory(inst, codePos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parser.cpp",
          1924,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, codePos )") )
  {
    __debugbreak();
  }
  Monkey_GrabComPrints(1);
  Com_PrintError(channel, "\n******* script runtime error *******\n%s: ", msg);
  Scr_PrintPrevCodePos(inst, channel, codePos, index);
  if ( MEMORY[0xA05AC90][4298 * inst] )
  {
    for ( i = MEMORY[0xA05AC90][4298 * inst] - 1; i >= 1; --i )
    {
      Com_PrintError(channel, "called from:\n");
      Scr_PrintPrevCodePos(
        inst,
        0,
        (char *)MEMORY[0xA05ACB0][4298 * inst + 6 * i],
        MEMORY[0xA05ACB0][4298 * inst + 1 + 6 * i] == 0);
    }
    Com_PrintError(channel, "started from:\n");
    Scr_PrintPrevCodePos(inst, 0, (char *)MEMORY[0xA05ACB0][4298 * inst], 1u);
  }
  Com_PrintError(channel, "************************************\n");
}

void __cdecl Scr_SetLoadedImpureScript(bool loadedImpureScript)
{
  *(_BYTE *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8264) = loadedImpureScript;
}

