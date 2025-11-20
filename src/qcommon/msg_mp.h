#pragma once

float (*__cdecl MSG_GetMapCenter())[3];
int __cdecl GetMinBitCountForNum(unsigned int num);
void __cdecl MSG_Init(msg_t *buf, unsigned __int8 *data, int length);
void __cdecl MSG_InitReadOnly(msg_t *buf, unsigned __int8 *data, int length);
void __cdecl MSG_InitReadOnlySplit(msg_t *buf, unsigned __int8 *data, int length, unsigned __int8 *data2, int length2);
void __cdecl MSG_BeginReading(msg_t *msg);
void __cdecl MSG_Discard(msg_t *msg);
int __cdecl MSG_GetUsedBitCount(const msg_t *msg);
int __cdecl MSG_GetNumBitsRead(const msg_t *msg);
void __cdecl MSG_SetBookmark(const msg_t *msg, msg_bookmark_t *bookmark);
void __cdecl MSG_GotoBookmark(msg_t *msg, const msg_bookmark_t *bookmark);
void __cdecl MSG_WriteBits(msg_t *msg, int value, unsigned int bits);
void __cdecl MSG_WriteBit0(msg_t *msg);
void __cdecl MSG_WriteBit1(msg_t *msg);
int __cdecl MSG_ReadBits(msg_t *msg, unsigned int bits);
int __cdecl MSG_GetByte(msg_t *msg, int where);
int __cdecl MSG_ReadBit(msg_t *msg);
unsigned int __cdecl MSG_CompressWithZLib(
        unsigned __int8 *from,
        unsigned int inSizeBytes,
        unsigned __int8 *to,
        unsigned int outSizeBytes);
int __cdecl MSG_WriteBitsCompress(
        bool trainHuffman,
        unsigned __int8 *from,
        int fromSizeBytes,
        unsigned __int8 *to,
        int toSizeBytes);
void __cdecl showHuffmanData();
unsigned int __cdecl MSG_DecompressWithZLib(
        unsigned __int8 *from,
        unsigned int inSizeBytes,
        unsigned __int8 *to,
        unsigned int outSizeBytes);
unsigned int __cdecl MSG_ReadBitsCompress(
        unsigned __int8 *from,
        int fromSizeBytes,
        unsigned __int8 *to,
        unsigned int toSizeBytes);
void __cdecl MSG_Embed(msg_t *dest, msg_t *src);
void __cdecl MSG_UnEmbed(msg_t *msg);
void __cdecl MSG_WriteByte(msg_t *msg, unsigned __int8 c);
void __cdecl MSG_WriteData(msg_t *buf, unsigned __int8 *data, unsigned int length);
void __cdecl MSG_WriteShort(msg_t *msg, __int16 c);
void __cdecl MSG_WriteLong(msg_t *msg, int c);
void __cdecl MSG_WriteFloat(msg_t *sb, int f);
void __cdecl MSG_WriteInt64(msg_t *msg, unsigned __int64 c);
void __cdecl MSG_WriteString(msg_t *sb, const char *s);
void __cdecl MSG_WriteBigString(msg_t *sb, char *s);
void __cdecl MSG_WriteAngle(msg_t *sb, unsigned int bits, float f);
void __cdecl MSG_WriteAngle16(msg_t *sb, float f);
void __cdecl MSG_WriteRangedFloatBits(msg_t *sb, float f, float begin, float end, unsigned int bits);
int __cdecl MSG_ReadByte(msg_t *msg);
int __cdecl MSG_ReadShort(msg_t *msg);
int __cdecl MSG_ReadLong(msg_t *msg);
unsigned __int64 __cdecl MSG_ReadInt64(msg_t *msg);
double __cdecl MSG_ReadFloat(msg_t *msg);
char *__cdecl MSG_ReadString(msg_t *msg, char *string, unsigned int maxChars);
char *__cdecl MSG_ReadBigString(msg_t *msg);
char *__cdecl MSG_ReadStringLine(msg_t *msg, char *string, unsigned int maxChars);
double __cdecl MSG_ReadAngle(msg_t *msg, unsigned int bits);
double __cdecl MSG_ReadAngle16(msg_t *msg);
double __cdecl MSG_ReadRangedFloatBits(msg_t *sb, float begin, float end, unsigned int bits);
void __cdecl MSG_ReadData(msg_t *msg, unsigned __int8 *data, int len);
void __cdecl MSG_WriteDeltaKey(msg_t *msg, int key, int oldV, int newV, unsigned int bits);
unsigned int __cdecl MSG_ReadDeltaKey(msg_t *msg, int key, int oldV, unsigned int bits);
void __cdecl MSG_WriteKey(msg_t *msg, int key, int newV, unsigned int bits);
unsigned int __cdecl MSG_ReadKey(msg_t *msg, int key, unsigned int bits);
void __cdecl MSG_WriteDeltaKeyByte(msg_t *msg, char key, char oldV, char newV);
int __cdecl MSG_ReadDeltaKeyByte(msg_t *msg, unsigned __int8 key, int oldV);
void __cdecl MSG_WriteDeltaKeyShort(msg_t *msg, __int16 key, __int16 oldV, __int16 newV);
int __cdecl MSG_ReadDeltaKeyShort(msg_t *msg, __int16 key, int oldV);
void __cdecl MSG_SetDefaultUserCmd(playerState_s *ps, usercmd_s *cmd);
void __cdecl MSG_WriteDeltaUsercmdKey(
        msg_t *msg,
        int key,
        const usercmd_s *from,
        const usercmd_s *to,
        int forceSendAngles);
void __cdecl MSG_ReadDeltaUsercmdKey(msg_t *msg, int key, const usercmd_s *from, usercmd_s *to);
void __cdecl MSG_ClearLastReferencedEntity(msg_t *msg);
double __cdecl MSG_ReadDemoRoundedFloat(
        msg_t *msg,
        int bits,
        float oldValue,
        unsigned int diffBits,
        unsigned int fullBits);
double __cdecl MSG_ReadRangedFloat(
        msg_t *msg,
        float oldValue,
        float begin,
        float end,
        int precision,
        bool oftenZeroOrOne);
int __cdecl MSG_ReadEntityIndex(msg_t *msg, unsigned int indexBits);
void __cdecl MSG_ReadDeltaField(
        msg_t *msg,
        int time,
        char *from,
        char *to,
        const NetField *field,
        int print,
        bool noXor);
int __cdecl MSG_ReadDeltaTime(msg_t *msg, int timeBase);
int __cdecl MSG_ReadDeltaGroundEntity(msg_t *msg);
int __cdecl MSG_ReadDeltaEventField(msg_t *msg);
int __cdecl MSG_ReadDeltaEventParamField(msg_t *msg);
int __cdecl MSG_ReadEFlags(msg_t *msg, int oldFlags);
double __cdecl MSG_ReadOriginFloat(int bits, msg_t *msg, float oldValue);
double __cdecl MSG_ReadOriginZFloat(msg_t *msg, float oldValue);
int __cdecl MSG_ReadDeltaEntityStruct(
        msg_t *msg,
        int time,
        char *from,
        char *to,
        unsigned int number,
        int totalFields,
        int skippedFieldBits);
int __cdecl MSG_ReadDeltaArchivedEntity(
        msg_t *msg,
        int time,
        const archivedEntity_s *from,
        archivedEntity_s *to,
        unsigned int number);
int __cdecl MSG_ReadAppendedDeltaStruct(
        msg_t *msg,
        int time,
        char *from,
        char *to,
        unsigned int number,
        int numFields,
        char indexBits,
        const NetField *stateFields,
        int skippedFieldBits);
void __cdecl MSG_ReadDeltaPlayerstate(
        int localClientNum,
        msg_t *msg,
        playerState_s *from,
        bool predictedFieldsIgnoreXor);
void __cdecl MSG_ReadDeltaHudElems(msg_t *msg, int time, const hudelem_s *from, hudelem_s *to, int count);
void __cdecl MSG_InitHuffman();
void MSG_initHuffmanInternal();
void __cdecl MSG_DumpNetFieldChanges_f();
char __thiscall bitarray<51>::areAllBitsEqual(
        bitarray<51> *this,
        const bitarray<51> *otherBitSet,
        const bitarray<51> *ignoreMaskBitSet);
