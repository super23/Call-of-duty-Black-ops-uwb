#pragma once

void __cdecl DObjInit();
void __cdecl DObjShutdown();
void __cdecl DObjDumpInfo(const DObj *obj);
bool __cdecl DObjIgnoreCollision(const DObj *obj, char modelIndex);
void __cdecl DObjGetHierarchyBits(DObj *obj, int boneIndex, int *partBits);
bool __cdecl DObjSkelIsBoneUpToDate(DObj *obj, int boneIndex);
void __cdecl DObjSetTree(DObj *obj, XAnimTree_s *tree);
void __cdecl DObjCreate(
        DObjModel_s *dobjModels,
        unsigned int numModels,
        XAnimTree_s *tree,
        unsigned __int8 *buf,
        __int16 entnum);
void __cdecl DObjCreateDuplicateParts(DObj *obj, DObjModel_s *dobjModels, unsigned int numModels);
void __cdecl DObjDumpCreationInfo(DObjModel_s *dobjModels, unsigned int numModels);
void __cdecl DObjComputeBounds(DObj *obj);
void __cdecl DObjCreateExt(
        DObjModel_s *dobjModels,
        unsigned int numModels,
        XAnimTree_s *tree,
        unsigned __int8 *buf,
        __int16 entnum,
        bool isServer,
        bool isLocalPlayer,
        unsigned __int8 localClientIndex);
void __cdecl DObjFree(DObj *obj);
void __cdecl DObjGetCreateParms(
        const DObj *obj,
        DObjModel_s *dobjModels,
        unsigned __int16 *numModels,
        XAnimTree_s **tree,
        unsigned __int16 *entnum);
void __cdecl DObjCalcBounds(DObj *dobj, float *mins, float *maxs);
void __cdecl DObjArchive(DObj *obj);
void __cdecl DObjUnarchive(DObj *obj);
void __cdecl DObjSkelClear(const DObj *obj);
bool __cdecl DObjIsServer(const DObj *obj);
bool __cdecl DObjIsLocalPlayer(const DObj *obj);
bool __cdecl DObjIsPlayerShadow(const DObj *obj);
bool __cdecl DObjIsLeftHandGripIKActive(const DObj *obj);
int __cdecl DObjGetLocalClientIndex(const DObj *obj);
void __cdecl DObjGetBounds(const DObj *obj, float *mins, float *maxs);
void __cdecl DObjPhysicsGetBounds(const DObj *obj, float *mins, float *maxs);
double __cdecl DObjGetRadius(const DObj *obj);
PhysPreset *__cdecl DObjGetPhysPreset(const DObj *obj);
const char *__cdecl DObjGetName(const DObj *obj);
char *__cdecl DObjGetBoneName(const DObj *obj, int boneIndex);
char *__cdecl DObjGetModelParentBoneName(const DObj *obj, int modelIndex);
XAnimTree_s *__cdecl DObjGetTree(const DObj *obj);
int __cdecl DObjBad(const DObj *obj);
void __cdecl DObjTracelinePartBits(DObj *obj, int *partBits);
void __cdecl DObjTraceline(DObj *obj, float *start, float *end, unsigned __int8 *priorityMap, DObjTrace_s *trace);
void __cdecl InvMatrixTransformVectorQuatTrans(const float *in, const DObjAnimMat *mat, float *out);
void __cdecl DObjGeomTraceline(DObj *obj, float *localStart, float *localEnd, int contentmask, DObjTrace_s *results);
void __cdecl DObjGeomTracelinePartBits(DObj *obj, int contentmask, int *partBits);
int __cdecl DObjHasContents(DObj *obj, int contentmask);
int __cdecl DObjGetContents(const DObj *obj);
bool __cdecl DObjHasCollmap(const DObj *obj);
PhysGeomList *__cdecl DObjGetCollmap(DObj *obj);
XModel *__cdecl DObjGetXModel(DObj *obj, int index);
int __cdecl DObjSetLocalBoneIndex(DObj *obj, int *partBits, int boneIndex, const float *trans, const float *angles);
int __cdecl DObjGetModelBoneIndex(const DObj *obj, const char *modelName, unsigned int name, unsigned __int8 *index);
void __cdecl DObjGetBasePoseMatrix(DObj *obj, unsigned __int8 boneIndex, DObjAnimMat *outMat);
void __cdecl DObjSetHidePartBits(DObj *obj, const unsigned int *partBits);
bool __cdecl DObjHasFlag(const DObj *obj, unsigned __int8 flag);
void __cdecl DObjSetFlag(DObj *obj, unsigned __int8 flag, int val);
bool __cdecl DObjHasIKActiveFlag(const DObj *obj);
void __cdecl DObjSetIKActiveFlag(DObj *obj, int val);
void __cdecl DObjSetIKState(DObj *obj, unsigned __int8 *pIKState);
int __cdecl DObjGetModelBoneIndex(const DObj *obj, int model_index, unsigned int name, unsigned __int8 *index);
