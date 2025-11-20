#pragma once

void __cdecl Scr_InitOpcodeLookup(scriptInstance_t inst);
void __cdecl Scr_ShutdownOpcodeLookup(scriptInstance_t inst);
void __cdecl AddOpcodePos(scriptInstance_t inst, unsigned int sourcePos, int type);
void __cdecl RemoveOpcodePos(scriptInstance_t inst);
void __cdecl AddThreadStartOpcodePos(scriptInstance_t inst, unsigned int sourcePos);
const char *__cdecl Scr_GetOpcodePosOfType(
        scriptInstance_t inst,
        unsigned int bufferIndex,
        unsigned int startSourcePos,
        unsigned int endSourcePos,
        int type,
        unsigned int *sourcePos);
unsigned int __cdecl Scr_GetClosestSourcePosOfType(
        scriptInstance_t inst,
        unsigned int bufferIndex,
        unsigned int sourcePos,
        int type);
unsigned int __cdecl Scr_GetPrevSourcePos(scriptInstance_t inst, const char *codePos, unsigned int index);
OpcodeLookup *__cdecl Scr_GetPrevSourcePosOpcodeLookup(scriptInstance_t inst, const char *codePos);
unsigned int __cdecl Scr_GetLineNum(scriptInstance_t inst, unsigned int bufferIndex, unsigned int sourcePos);
unsigned int __cdecl Scr_GetLineNumInternal(const char *buf, unsigned int sourcePos, const char **startLine, int *col);
unsigned int __cdecl Scr_GetFunctionLineNumInternal(const char *buf, unsigned int sourcePos, const char **startLine);
int __cdecl Scr_GetSourcePosOfType(scriptInstance_t inst, const char *codePos, int type, Scr_SourcePos_t *pos);
OpcodeLookup *__cdecl Scr_GetSourcePosOpcodeLookup(scriptInstance_t inst, const char *codePos);
void __cdecl Scr_SendSource(scriptInstance_t inst);
char *__cdecl Scr_AddSourceBuffer(
        scriptInstance_t inst,
        const char *filename,
        char *extFilename,
        const char *codePos,
        bool archive);
void __cdecl Scr_AddSourceBufferInternal(
        scriptInstance_t inst,
        const char *extFilename,
        const char *codePos,
        char *sourceBuf,
        int len,
        bool doEolFixup,
        bool archive);
SourceBufferInfo *__cdecl Scr_GetNewSourceBuffer(scriptInstance_t inst);
char *__cdecl Scr_ReadFile(
        scriptInstance_t inst,
        const char *filename,
        char *extFilename,
        const char *codePos,
        bool archive);
char *__cdecl Scr_ReadFile_FastFile(
        scriptInstance_t inst,
        const char *filename,
        const char *extFilename,
        const char *codePos,
        bool archive);
char *__cdecl Scr_ReadFile_LoadObj(
        scriptInstance_t inst,
        const char *filename,
        char *extFilename,
        const char *codePos,
        bool archive);
unsigned int __cdecl Scr_GetSourcePos(
        scriptInstance_t inst,
        unsigned int bufferIndex,
        unsigned int sourcePos,
        char *outBuf,
        unsigned int outBufLen);
unsigned int __cdecl Scr_GetLineInfo(const char *buf, unsigned int sourcePos, int *col, char *line);
void __cdecl Scr_CopyFormattedLine(char *line, const char *rawLine);
unsigned int __cdecl Scr_GetSourceBuffer(scriptInstance_t inst, const char *codePos);
void __cdecl Scr_PrintPrevCodePos(scriptInstance_t inst, int channel, char *codePos, unsigned int index);
void __cdecl Scr_PrintSourcePos(
        scriptInstance_t inst,
        int channel,
        const char *filename,
        const char *buf,
        unsigned int sourcePos);
char *__cdecl Scr_PrevCodePosFileName(scriptInstance_t inst, char *codePos);
const char *__cdecl Scr_PrevCodePosFunctionName(scriptInstance_t inst, char *codePos);
bool __cdecl Scr_PrevCodePosFileNameMatches(scriptInstance_t inst, char *codePos, const char *fileName);
void __cdecl Scr_PrintPrevCodePosSpreadSheet(
        scriptInstance_t inst,
        int channel,
        char *codePos,
        bool summary,
        bool functionSummary);
void __cdecl Scr_PrintSourcePosSpreadSheet(
        scriptInstance_t inst,
        int channel,
        const char *filename,
        const char *buf,
        unsigned int sourcePos);
void __cdecl Scr_PrintFunctionPosSpreadSheet(
        scriptInstance_t inst,
        int channel,
        const char *filename,
        const char *buf,
        unsigned int sourcePos);
unsigned int __cdecl Scr_GetFunctionInfo(const char *buf, unsigned int sourcePos, char *line);
void __cdecl Scr_PrintSourcePosSummary(scriptInstance_t inst, int channel, const char *filename);
void __cdecl Scr_GetCodePos(
        scriptInstance_t inst,
        const char *codePos,
        unsigned int index,
        char *outBuf,
        unsigned int outBufLen);
void __cdecl Scr_GetFileAndLine(scriptInstance_t inst, const char *codePos, char **filename, int *linenum);
void CompileError(scriptInstance_t inst, unsigned int sourcePos, const char *msg, ...);
void __cdecl Scr_IgnoreLeaks(scriptInstance_t inst);
void CompileError2(scriptInstance_t inst, char *codePos, const char *msg, ...);
void __cdecl Scr_GetTextSourcePos(scriptInstance_t inst, const char *buf, char *codePos, char *line);
void __cdecl RuntimeError(
        scriptInstance_t inst,
        char *codePos,
        unsigned int index,
        const char *msg,
        const char *dialogMessage);
void __cdecl RuntimeErrorInternal(
        scriptInstance_t inst,
        int channel,
        char *codePos,
        unsigned int index,
        const char *msg);
void __cdecl Scr_SetLoadedImpureScript(bool loadedImpureScript);
