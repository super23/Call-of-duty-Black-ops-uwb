#pragma once

int __cdecl XAnimTreeSize();
bool __cdecl XAnimTreeHasInfo(const XAnimTree_s *tree);
void __cdecl XAnimInit();
void __cdecl XAnimShutdown();
void __cdecl XAnimFree(XAnimParts *parts, scriptInstance_t inst);
unsigned __int8 *__cdecl Hunk_AllocXAnimPrecache(unsigned int size);
XAnimParts *__cdecl XAnimPrecache(char *name, void *(__cdecl *Alloc)(int));
XAnimParts *__cdecl XAnimFindData_LoadObj(char *name, void *(__cdecl *Alloc)(int));
XAnimParts *__cdecl XAnimFindData_FastFile(const char *name);
XAnimParts *__cdecl XAnimClone(XAnimParts *fromParts, void *(__cdecl *Alloc)(int));
void __cdecl XAnimCreate(XAnim_s *anims, unsigned int animIndex, char *name);
void __cdecl XAnimBlend(
        XAnim_s *anims,
        unsigned int animIndex,
        const char *name,
        unsigned int children,
        unsigned int num,
        unsigned int flags);
bool __cdecl IsNodeAdditive(const XAnimEntry *node);
bool __cdecl IsLeafNode(const XAnimEntry *anim);
void __cdecl XAnimSetParamValue(XAnim_s *anims, unsigned int paramIndex, char *strParamName, float paramValue);
void __cdecl XAnimSetValueSection(
        XAnim_s *anims,
        unsigned int animIndex,
        __int16 iFirstValueIndex,
        unsigned __int16 iValueCount);
XAnim_s *__cdecl XAnimCreateAnims(const char *debugName, int size, void *(__cdecl *Alloc)(int));
XAnim_s *__cdecl XAnimCreateAnimsWithValues(
        const char *debugName,
        int size,
        unsigned int iTotalValueCount,
        void *(__cdecl *Alloc)(int));
void __cdecl XAnimFreeList(XAnim_s *anims);
XAnimTree_s *__cdecl XAnimCreateTree(XAnim_s *anims, void *(__cdecl *Alloc)(int));
void __cdecl XAnimFreeTree(
        XAnimTree_s *tree,
        void (__cdecl *Free)(void *, int, scriptInstance_t),
        scriptInstance_t inst);
XAnim_s *__cdecl XAnimGetAnims(const XAnimTree_s *tree);
void __cdecl XAnimInitModelMap(XModel *const *models, unsigned int numModels, XModelNameMap *modelMap);
unsigned int __cdecl XAnimGetAnimMap(const XAnimParts *parts, const XModelNameMap *modelMap);
void __cdecl XAnimCalcRelDeltaParts(
        const XAnimParts *parts,
        float weightScale,
        float time1,
        float time2,
        XAnimSimpleRotPos *rotPos,
        int quatIndex);
void __cdecl TransformToQuatRefFrame(const float *rot, float *trans);
void __cdecl XAnimCalcAbsDeltaParts(const XAnimParts *parts, float weightScale, float time, XAnimSimpleRotPos *rotPos);
void __cdecl XAnimAddClientNotify(unsigned int notetrackName, float frac, unsigned int notifyType);
void __cdecl AddSignalToNoteList(XAnimClientNotifyList *pList, const ClientNotifyData *ourNote);
void __cdecl XAnimAddClientNotifyNamed(unsigned int notetrackName, float frac, unsigned int notifyName);
void  XAnimResetAnimMap(XModelNameMap a1@<ebp>, const DObj *obj, unsigned int infoIndex);
void __cdecl XAnimResetAnimMap_r(XModelNameMap *modelMap, unsigned int infoIndex);
void __cdecl XAnimResetAnimMapLeaf(const XModelNameMap *modelMap, unsigned int infoIndex);
double __cdecl XAnimGetLength(const XAnim_s *anims, unsigned int animIndex);
int __cdecl XAnimGetFrameCount(const XAnim_s *anims, unsigned int animIndex);
int __cdecl XAnimGetLengthMsec(const XAnim_s *anims, unsigned int anim);
unsigned int __cdecl XAnimGetInfoIndex(const XAnimTree_s *tree, unsigned int animIndex);
unsigned int __cdecl XAnimGetInfoIndex_r(unsigned int animIndex, unsigned int infoIndex);
double __cdecl XAnimGetTime(const XAnimTree_s *tree, unsigned int animIndex);
double __cdecl XAnimGetWeight(const XAnimTree_s *tree, unsigned int animIndex);
bool __cdecl XAnimHasFinished(const XAnimTree_s *tree, unsigned int animIndex);
int __cdecl XAnimGetNumChildren(const XAnim_s *anims, unsigned int animIndex);
unsigned int __cdecl XAnimGetChildAt(const XAnim_s *anims, unsigned int animIndex, unsigned int childIndex);
const char *__cdecl XAnimGetAnimName(const XAnim_s *anims, unsigned int animIndex);
char *__cdecl XAnimGetAnimDebugName(const XAnim_s *anims, unsigned int animIndex);
unsigned int __cdecl XAnimGetAnimTreeSize(const XAnim_s *anims);
void __cdecl XAnimInitInfo(XAnimInfo *info);
void __cdecl XAnimUpdateOldTime(
        DObj *obj,
        unsigned int infoIndex,
        XAnimState *syncState,
        float dtime,
        bool parentHasWeight,
        bool *childHasTimeForParent);
unsigned int __cdecl XAnimInitTime(XAnimTree_s *tree, unsigned int infoIndex, float goalTime, int cmdIndex);
void __cdecl XAnimResetTime(unsigned int infoIndex);
void __cdecl XAnimResetTimeInternal(unsigned int infoIndex);
unsigned int __cdecl XAnimCloneInitTime(
        XAnimTree_s *tree,
        unsigned int infoIndex,
        unsigned int parentIndex,
        int cmdIndex);
void __cdecl DObjInitServerTime(DObj *obj, float dtime);
void __cdecl DObjUpdateClientInfo(DObj *obj, float dtime, int notifyFlags);
void __cdecl XAnimUpdateTimeAndNotetrack(const DObj *obj, unsigned int infoIndex, float dtime, int notifyFlags);
void __cdecl XAnimCheckFreeInfo(XAnimTree_s *tree, unsigned int infoIndex, int hasWeight);
void __cdecl XAnimFreeInfo(XAnimTree_s *tree, unsigned int infoIndex);
void __cdecl XAnimClearServerNotify(XAnimInfo *info, scriptInstance_t inst);
double __cdecl XAnimGetAverageRateFrequency(const XAnimTree_s *tree, unsigned int infoIndex);
void __cdecl XAnimUpdateTimeAndNotetrackLeaf(
        const DObj *obj,
        const XAnimParts *parts,
        unsigned int infoIndex,
        float dtime,
        char notifyFlags);
void __cdecl XAnimProcessClientNotify(XAnimInfo *info, float dtime);
signed int __cdecl XAnimGetNextNotifyIndex(const XAnimParts *parts, float time);
double __cdecl XAnimGetNotifyFracLeaf(const XAnimState *state, const XAnimState *nextState, float time, float dtime);
void __cdecl XAnimUpdateTimeAndNotetrackSyncSubTree(
        const DObj *obj,
        unsigned int infoIndex,
        float dtime,
        int notifyFlags);
void __cdecl XAnimUpdateInfoSync(
        const DObj *obj,
        unsigned int infoIndex,
        int notifyFlags,
        XAnimState *syncState,
        float dtime);
void __cdecl XAnimProcessServerNotify(const DObj *obj, XAnimInfo *info, float time);
XAnimParts *__cdecl XAnimGetPartsForNotify(const XAnimTree_s *tree, XAnimInfo *info);
void __cdecl NotifyServerNotetrack(
        const DObj *obj,
        unsigned int notifyName,
        unsigned int notetrackName,
        float fPercentage);
void __cdecl DObjClearServerNotifies();
void __cdecl DObjSetServerNotifies(XAnimServerNotifyList *pNotifyList);
void __cdecl DObjClearClientNotifies();
void __cdecl DObjSetClientNotifies(XAnimClientNotifyList *pNotifyList);
const XAnimClientNotifyList *__cdecl DObjGetClientNotifies();
void __cdecl DObjUpdateServerInfo(DObj *obj, float dtime, int notifyFlags);
void __cdecl DObjDisplayAnimToBuffer(const DObj *obj, const char *header, char *buffer, int bufferSize);
void __cdecl XAnimDisplay(
        const XAnimTree_s *tree,
        unsigned int infoIndex,
        int depth,
        char *buffer,
        int bufferSize,
        int *bufferPos);
void __cdecl DObjDisplayAnim(const DObj *obj, const char *header);
void __cdecl XAnimCalcDelta(DObj *obj, unsigned int animIndex, float *rot, float *trans, bool bUseGoalWeight);
void __cdecl XAnimCalcDeltaTree(
        const DObj *obj,
        unsigned int infoIndex,
        float weightScale,
        XAnimDeltaInfo deltaInfo,
        XAnimSimpleRotPos *rotPos);
void __cdecl XAnimCalcAbsDelta(DObj *obj, unsigned int animIndex, float *rot, float *trans);
void __cdecl XAnimGetRelDelta(
        const XAnim_s *anims,
        unsigned int animIndex,
        float *rot,
        float *trans,
        float time1,
        float time2);
void __cdecl XAnimGetAbsDelta(const XAnim_s *anims, unsigned int animIndex, float *rot, float *trans, float time);
unsigned int __cdecl XAnimAllocInfoWithParent(
        XAnimTree_s *tree,
        unsigned __int16 animToModel,
        unsigned int animIndex,
        unsigned int parentInfoIndex,
        int after);
unsigned int  XAnimAllocInfoIndex@<eax>(
        unsigned int a1@<ebp>,
        DObj *obj,
        unsigned int animIndex,
        int after,
        int cmdIndex);
unsigned int __cdecl XAnimEnsureGoalWeightParent(DObj *obj, unsigned int animIndex, int cmdIndex);
void __cdecl XAnimClearGoalWeight(
        XAnimTree_s *tree,
        unsigned int animIndex,
        float blendTime,
        unsigned __int16 cmdIndex);
void __cdecl XAnimClearGoalWeightInternal(
        XAnimTree_s *tree,
        unsigned int infoIndex,
        float blendTime,
        int forceBlendTime,
        unsigned __int16 cmdIndex);
void __cdecl XAnimClearTreeGoalWeightsInternal(
        XAnimTree_s *tree,
        unsigned int infoIndex,
        float blendTime,
        int forceBlendTime,
        int cmdIndex);
void __cdecl XAnimClearTreeGoalWeights(XAnimTree_s *tree, unsigned int animIndex, float blendTime, int cmdIndex);
void __cdecl XAnimClearTreeGoalWeightsStrict(XAnimTree_s *tree, unsigned int animIndex, float blendTime, int cmdIndex);
int __cdecl XAnimSetCompleteGoalWeightKnob(
        DObj *obj,
        unsigned int animIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        int bRestart,
        int cmdIndex);
void __cdecl XAnimClearGoalWeightKnobInternal(
        XAnimTree_s *tree,
        unsigned int infoIndex,
        float goalWeight,
        float goalTime,
        unsigned __int16 cmdIndex);
int __cdecl XAnimSetCompleteGoalWeightNode(
        XAnimTree_s *tree,
        unsigned int infoIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        unsigned __int16 cmdIndex);
int __cdecl XAnimSetGoalWeightKnobAll(
        DObj *obj,
        unsigned int animIndex,
        unsigned int rootIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        int bRestart,
        int cmdIndex);
int __cdecl XAnimSetCompleteGoalWeightKnobAll(
        DObj *obj,
        unsigned int animIndex,
        unsigned int rootIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        int bRestart,
        int cmdIndex);
int __cdecl XAnimSetGoalWeightKnob(
        DObj *obj,
        unsigned int animIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        int bRestart,
        int cmdIndex);
void __cdecl XAnimClearTree(XAnimTree_s *tree);
int __cdecl XAnimSetGoalWeightNode(
        XAnimTree_s *tree,
        unsigned int infoIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        unsigned __int16 cmdIndex);
unsigned int __cdecl XAnimGetDescendantWithGreatestWeight(const XAnimTree_s *tree, unsigned int infoIndex);
void __cdecl  XAnimSetupSyncNodes(XAnim_s *anims);
void __cdecl XAnimSetupSyncNodes_r(XAnim_s *anims, unsigned int animIndex, int parFlags);
bool __cdecl XAnimHasTime(const XAnim_s *anims, unsigned int animIndex);
bool __cdecl XAnimIsPrimitive(XAnim_s *anims, unsigned int animIndex);
void __cdecl XAnimSetTime(XAnimTree_s *tree, unsigned int animIndex, float time, unsigned __int16 cmdIndex);
unsigned int __cdecl XAnimRestart(XAnimTree_s *tree, unsigned int infoIndex, float goalTime, int cmdIndex);
int __cdecl XAnimSetGoalWeight(
        DObj *obj,
        unsigned int animIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        int bRestart,
        int cmdIndex);
void __cdecl XAnimSetAnimRate(XAnimTree_s *tree, unsigned int animIndex, float rate);
bool __cdecl XAnimIsLooped(const XAnim_s *anims, unsigned int animIndex);
unsigned __int8 __cdecl XAnimGetBoneCount(const XAnim_s *anims, unsigned int animIndex);
int __cdecl XAnimHasBone(const XAnim_s *anims, unsigned int animIndex, unsigned __int16 boneName);
bool __cdecl XAnimIsClientNode(const XAnim_s *anims, unsigned int animIndex);
bool __cdecl XAnimIsClientNode(XAnimTree_s *tree, unsigned int animIndex);
char __cdecl XAnimNotetrackExists(const XAnim_s *anims, unsigned int animIndex, unsigned int name);
void __cdecl XAnimAddNotetrackTimesToScriptArray(const XAnim_s *anims, unsigned int animIndex, unsigned int name);
int __cdecl XAnimSetCompleteGoalWeight(
        DObj *obj,
        unsigned int animIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        int bRestart,
        int cmdIndex);
void __cdecl XAnimCloneAnimInfo(XAnimTree_s *toTree, const XAnimInfo *from, XAnimInfo *to);
void __cdecl XAnimCloneAnimTree(const XAnimTree_s *from, XAnimTree_s *to);
void __cdecl XAnimCloneAnimTree_r(
        const XAnimTree_s *from,
        XAnimTree_s *to,
        unsigned int fromInfoIndex,
        unsigned int toInfoParentIndex);
XAnimInfo *__cdecl GetAnimInfo(int infoIndex);
char __cdecl XAnimGetParamValue(const XAnim_s *anims, unsigned int animIndex, const char *paramName, float *outValue);
bool __cdecl XAnimGetParamValue(XAnimTree_s *tree, unsigned int animIndex, const char *paramName, float *outValue);
bool __cdecl XAnimGetParamValueTree(
        const XAnim_s *anims,
        unsigned int animIndex,
        const char *paramName,
        float *outValue);
void __thiscall bitarray<160>::setBit(bitarray<160> *this, unsigned int pos);
void __cdecl XAnim_SetTime(float time, int frameCount, XAnimTime *animTime);
