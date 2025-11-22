#pragma once
#include <universal/q_shared.h>
#include <qcommon/bitarray.h>
#include "ent.h"

enum netsrc_t : __int32
{                                       // XREF: msg_t/r netchan_t/r ...
    NS_CLIENT1    = 0x0,
    NS_SERVER     = 0x1,
    NS_MAXCLIENTS = 0x1,
    NS_PACKET     = 0x2,
};

struct __declspec(align(4)) msg_bookmark_t // sizeof=0x18
{                                       // XREF: demoKeyFrame/r
                                        // ?Demo_ProcessPlayback@@YAXXZ/r
    int overflowed;
    int cursize;
    int bit;
    int lastEntityRef;
    int readcount;
    unsigned __int8 bitByte;
    // padding byte
    // padding byte
    // padding byte
};

struct msg_t // sizeof=0x30
{                                       // XREF: .data:msg_t fakemsg/r
                                        // .data:g_archiveMsg/r ...
    int overflowed;                     // XREF: BG_EmblemsWriteString(short,CompositeEmblemLayer *,int)+44/r
                                        // BG_EmblemsWriteString(short,CompositeEmblemLayer *,int):loc_41A8E0/r ...
    int readOnly;
    unsigned __int8 *data;              // XREF: BG_EmblemsWriteString(short,CompositeEmblemLayer *,int)+88/r
                                        // CL_WritePacket(int):loc_552720/r ...
    unsigned __int8 *splitData;
    int maxsize;                        // XREF: CL_ReadDemoNetworkPacket+116/r
    int cursize;                        // XREF: BG_EmblemsWriteString(short,CompositeEmblemLayer *,int)+84/r
                                        // BG_EmblemsReadString(char const *,short *,CompositeEmblemLayer *,int)+43/w ...
    int splitSize;
    int readcount;                      // XREF: CL_ReadDemoNetworkPacket+180/w
                                        // CL_ParseServerMessage(int,msg_t *)+103/r ...
    int bit;                            // XREF: SV_SendClientVoiceData(client_t *):loc_71EFFC/r
                                        // Demo_Forward_f(void)+4CA/r ...
    int lastEntityRef;
    int flush;
    netsrc_t targetLocalNetID;
};

struct NetField // sizeof=0x1C
{                                       // XREF: .rdata:helicopterEntityStateFields/r
                                        // .rdata:planeStateFields/r ...
    const char *name;
    int offset;
    int size;
    int bits;
    unsigned __int8 changeHints;
    // padding byte
    // padding byte
    // padding byte
    const char *bitsStr;
    const char *changeHintsStr;
};

struct archivedEntityShared_t // sizeof=0x20
{                                       // XREF: archivedEntity_s/r
    int svFlags;                        // XREF: SV_ArchiveSnapshot(msg_t *)+86A/w
                                        // SV_ArchiveSnapshot(msg_t *)+87F/r ...
    int clientMask[1];                  // XREF: SV_ArchiveSnapshot(msg_t *)+89A/w
    float absmin[3];                    // XREF: SV_ArchiveSnapshot(msg_t *)+8BC/w
                                        // SV_ArchiveSnapshot(msg_t *)+8CF/w ...
    float absmax[3];                    // XREF: SV_ArchiveSnapshot(msg_t *)+906/w
                                        // SV_ArchiveSnapshot(msg_t *)+919/w ...
};

struct archivedEntity_s // sizeof=0x100
{                                       // XREF: svEntity_s/r
                                        // archivedEntity_t/r ...
    entityState_s s;                    // XREF: CM_GetWaterHeight(float const * const,float,float)+107/o
                                        // CM_UnlinkEntity(svEntity_s *)+75/o ...
    archivedEntityShared_t r;           // XREF: SV_ArchiveSnapshot(msg_t *)+86A/w
                                        // SV_ArchiveSnapshot(msg_t *)+87F/r ...
};

struct __declspec(align(2)) usercmd_s // sizeof=0x34
{                                       // XREF: clientSession_t/r
                                        // clientSession_t/r ...
    int serverTime;                     // XREF: CG_CalcVehicleViewValues(int)+1543/r
                                        // CG_Calc3rdPersonVehicleViewValues(int)+29F/r ...
    bitarray<51> button_bits;           // XREF: PM_AirMove+15/o
                                        // PM_SwimMove+15/o ...
    int angles[3];                      // XREF: CG_CheckPlayerMovement+B/o
                                        // CG_CheckPlayerMovement+E/o
    unsigned __int16 weapon;
    unsigned __int16 offHandIndex;
    unsigned __int16 lastWeaponAltModeSwitch;
    char forwardmove;                   // XREF: CG_RadiantCamCalcView(int)+73/r
                                        // CG_CheckPlayerMovement:loc_49A9CE/r
    char rightmove;                     // XREF: CG_RadiantCamCalcView(int)+81/r
                                        // CG_CheckPlayerMovement+26/r
    char upmove;
    char pitchmove;                     // XREF: CG_CalcVehicleViewValues(int)+1516/r
                                        // CG_Calc3rdPersonVehicleViewValues(int)+249/r ...
    char yawmove;                       // XREF: CG_CalcVehicleViewValues(int)+130D/r
                                        // CG_CalcVehicleViewValues(int)+131D/r ...
    // padding byte
    float meleeChargeYaw;
    unsigned __int8 meleeChargeDist;
    // padding byte
    // padding byte
    // padding byte
    float rollmove;
    char selectedLocation[2];
    unsigned __int8 selectedYaw;
    // padding byte
};

struct playerState_s;
struct hudelem_s;

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
