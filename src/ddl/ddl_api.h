#pragma once

void __cdecl DDL_Init();
int __cdecl DDL_GetTotalBufferBitSize(ddlDef_t *ddl);
void DDL_Printf(const char *fmt, ...);
void DDL_NoCheckPrintf(const char *fmt, ...);
void DDL_PrintWarning(const char *fmt, ...);
void DDL_NoCheckPrintWarning(const char *fmt, ...);
void DDL_PrintError(const char *fmt, ...);
void DDL_NoCheckPrintError(const char *fmt, ...);
int __cdecl DDL_IsStateReadyForIO(const ddlState_t *state);
int __cdecl DDL_AssociateBuffer(char *buffer, int bufferSize, ddlDef_t *ddl);
int __cdecl DDL_FixBufferVersion(
        char *buffer,
        ddlDef_t *ddlTo,
        const char *ddlName,
        char *backupBuffer,
        int bufferSize);
ddlDef_t *__cdecl DDL_LoadAssetWithVersion(const char *fileName, int version);
ddlDef_t *__cdecl DDL_LoadAsset(const char *fileName);
void __cdecl DDL_CopyFromBaseState(ddlState_t *newState, const ddlState_t *baseState);
void __cdecl DDL_Reset(ddlState_t *searchState, ddlDef_t *ddl);
int DDL_MoveTo(const ddlState_t *searchState, ddlState_t *resultState, int argNum, ...);
int __cdecl DDL_MoveToPath(const ddlState_t *searchState, ddlState_t *resultState, int depth, const char **ddlPath);
bool __cdecl DDL_MoveToStart(ddlState_t *searchState, ddlDef_t *ddl);
int __cdecl DDL_MoveToName(const ddlState_t *searchState, ddlState_t *resultState, const char *name);
int __cdecl DDL_MoveToIndex(
        const ddlState_t *searchState,
        ddlState_t *resultState,
        int arrayIndex,
        int checkValidIndex);
int __cdecl DDL_MoveNext(const ddlState_t *searchState, ddlState_t *resultState);
int __cdecl DDL_IterateNext(const ddlState_t *searchState, ddlState_t *resultState);
int __cdecl DDL_IteratePrev(const ddlState_t *searchState, ddlState_t *resultState);
int __cdecl DDL_IterateFirst(const ddlState_t *searchState, ddlState_t *resultState);
int __cdecl DDL_SetRawBytes(char *buffer, int offset, int size, const char *valueToInsert);
void __cdecl DDL_SetValueChanged(char *buffer);
bool __cdecl DDL_WriteIntValueInternal(
        ddlMemberDef_t *member,
        int absoluteOffset,
        unsigned int valueToInsert,
        char *buffer);
int __cdecl DDL_WriteIntValueIfChanged(char *buffer, int offset, int size, unsigned int valueToInsert);
bool __cdecl DDL_WriteInt64ValueInternal(
        ddlMemberDef_t *member,
        int absoluteOffset,
        unsigned __int64 valueToInsert,
        char *buffer);
int __cdecl DDL_WriteStringValueInternal(ddlMemberDef_t *member, int absoluteOffset, const char *value, char *buffer);
int __cdecl DDL_WriteValueInternal(ddlMemberDef_t *member, int absoluteOffset, const char *valueToInsert, char *buffer);
bool __cdecl DDL_SetInt(const ddlState_t *searchState, unsigned int value, char *buffer);
bool __cdecl DDL_SetInt64(const ddlState_t *searchState, unsigned __int64 value, char *buffer);
int __cdecl DDL_SetString(const ddlState_t *searchState, const char *value, char *buffer);
int __cdecl DDL_SetValue(const ddlState_t *searchState, const char *value, char *buffer);
void __cdecl DDL_SetUserFlag(char *buffer, int flagIndex, bool state);
unsigned int __cdecl DDL_ReadIntValueInternal(ddlMemberDef_t *member, int absoluteOffset, char *buffer);
unsigned int __cdecl DDL_ReadInt64ValueInternal(ddlMemberDef_t *member, int absoluteOffset, char *buffer);
char *__cdecl DDL_ReadStringValueInternal(ddlMemberDef_t *member, int absoluteOffset, char *buffer);
int __cdecl DDL_ReadValueInternal(ddlMemberDef_t *member, int absoluteOffset, ddlResult_t *result, char *buffer);
unsigned int __cdecl DDL_GetInt64(const ddlState_t *searchState, char *buffer);
unsigned int __cdecl DDL_GetInt(const ddlState_t *searchState, char *buffer);
char *__cdecl DDL_GetString(const ddlState_t *searchState, char *buffer);
int __cdecl DDL_GetValue(const ddlState_t *searchState, ddlResult_t *result, char *buffer);
int __cdecl DDL_GetBits(char *buffer, int bitOffset, unsigned int bitSize, unsigned int *value);
int __cdecl DDL_GetHeaderVersion(char *buffer);
bool __cdecl DDL_GetUserFlag(char *buffer, int flagIndex);
