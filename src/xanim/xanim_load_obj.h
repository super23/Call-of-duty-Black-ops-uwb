#pragma once

unsigned __int8 *__cdecl GetDeltaQuaternions(
        XAnimDeltaPart *deltaPart,
        void *(__cdecl *Alloc)(int),
        unsigned __int8 *pos,
        unsigned __int16 numloopframes,
        bool useSmallIndices);
void __cdecl ConsumeQuat2(const unsigned __int8 **pos, __int16 *out);
unsigned __int8 *__cdecl GetDeltaTranslations(
        const char *name,
        XAnimDeltaPart *deltaPart,
        void *(__cdecl *Alloc)(int),
        const unsigned __int8 *pos,
        unsigned __int16 numloopframes,
        bool useSmallIndices);
unsigned __int8 *__cdecl LoadTrans(
        void *(__cdecl *Alloc)(int),
        unsigned __int8 *pos,
        unsigned __int16 numTransIndices,
        XAnimPartTrans *trans);
XAnimParts *__cdecl XAnimLoadFile(char *name, void *(__cdecl *Alloc)(int), bool streamedVersion);
void __cdecl ReadNoteTracks(const char *name, unsigned __int16 **pos, XAnimParts *parts, void *(__cdecl *Alloc)(int));
unsigned __int8 *__cdecl GetQuaternions(
        XAnimPartQuatPtr *part,
        unsigned __int8 *pos,
        bool bFlipQuat,
        bool bSimpleQuat,
        unsigned __int16 numloopframes,
        bool useSmallIndices);
void __cdecl ConsumeQuat(const unsigned __int8 **pos, __int16 *out);
void *__cdecl XAnimTempAlloc(int size);
unsigned __int8 *__cdecl GetTranslations(
        XAnimPartTransPtr *part,
        const unsigned __int8 *pos,
        unsigned __int16 numloopframes,
        bool useSmallIndices);
unsigned int __cdecl XAnimGetPartQuatType(unsigned int animPartIndex);
unsigned int __cdecl XAnimGetPartTransType(unsigned int animPartIndex);
int __cdecl XAnimCompareQuatParts(unsigned __int16 *p0, unsigned __int16 *p1);
int __cdecl XAnimCompareTransParts(unsigned __int16 *p0, unsigned __int16 *p1);
int __cdecl XAnimIsHighPrecisionPart(const char *name);
void __cdecl XAnimEmitFrameIndices(
        unsigned int tableSize,
        unsigned __int8 **dataByte,
        __int16 **dataShort,
        XAnimIndices *indices,
        XAnimDynamicIndices *frameIndices,
        bool useSmallIndices);
