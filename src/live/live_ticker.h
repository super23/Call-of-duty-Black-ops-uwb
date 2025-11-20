#pragma once

char *__cdecl LiveTicker_GetStreamNameTextInternal(streamName_t streamName);
void __cdecl LiveTicker_DumpTickerInternal();
void __cdecl LiveTicker_SetDefaultDisplayOrder();
void __cdecl LiveTicker_InitializeInternal();
void __cdecl LiveTicker_InsertMessageInternal(const char *text, streamName_t streamName);
char __cdecl LiveTicker_IsStreamPopulated(streamName_t streamName);
int __cdecl LiveTicker_GetTickerMessageTextWidth(const char *text, int maxChars, Font_s *font, float scale);
void __cdecl LiveTicker_MoveToFirstStream();
void __cdecl LiveTicker_MoveToNextStream();
void __cdecl LiveTicker_MoveToNextMessage(streamName_t streamName);
void __cdecl LiveTicker_ReplaceMessages(const char *text, streamName_t streamName, bool isMultiple);
void __cdecl LiveTicker_AddMessages(const char *text, streamName_t streamName, bool isMultiple);
void __cdecl LiveTicker_RemoveStream(streamName_t streamName);
char *__cdecl LiveTicker_GetCurrentMessage(
        float scrX,
        float baseX,
        float baseY,
        float baseAlpha,
        Font_s *const font,
        float scale,
        float *xCoord,
        float *yCoord,
        float *alpha);
char *__cdecl LiveTicker_GetCurrentMessageCategory(
        float baseAlpha,
        float baseX,
        float baseWidth,
        Font_s *font,
        float baseScale,
        float *outXCoord,
        float *outAlpha,
        float *outScale);
void __cdecl LiveTicker_SetBatchSize(streamName_t streamName, int batchSize);
void __cdecl LiveTicker_SetContractMsgsBatchSize_f();
void __cdecl LiveTicker_SetEventMsgsBatchSize_f();
void __cdecl LiveTicker_SetCODMsgsBatchSize_f();
void __cdecl LiveTicker_SetFriendMsgsBatchSize_f();
void __cdecl LiveTicker_SetClanMsgsBatchSize_f();
void __cdecl LiveTicker_DumpTickerInternal_f();
void __cdecl LiveTicker_Init();
