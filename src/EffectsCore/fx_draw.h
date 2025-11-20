#pragma once

int __cdecl FX_RegionForReflect(const FxReflectParams *reflect);
void __cdecl FX_GetNullReflection(FxReflectParams *params);
void __cdecl FX_GetReflection(bool on, float height, FxReflectParams *params);
void __cdecl FX_EvaluateVisAlpha(FxElemPreVisualState *preVisState, FxElemVisualState *visState);
unsigned __int8 __cdecl FX_InterpolateColor(
        const FxElemVisStateSample *refState,
        float valueLerp,
        float valueLerpInv,
        float sampleLerp,
        float sampleLerpInv,
        int channel);
void __cdecl FX_SetupVisualState(
        const FxElemDef *elemDef,
        const FxEffect *effect,
        int randomSeed,
        float normTimeUpdateEnd,
        FxElemPreVisualState *preVisState);
void __cdecl FX_EvaluateSize(FxElemPreVisualState *preVisState, FxElemVisualState *visState);
void __cdecl FX_EvaluateVisualState(
        int localClientNum,
        FxElemPreVisualState *preVisState,
        float msecLifeSpan,
        FxElemVisualState *visState);
double __cdecl FX_IntegrateRotationFromZero(
        const FxElemVisStateSample *refState,
        int randomSeed,
        FxRandKey randomKey,
        float sampleLerp,
        float msecLifeSpan);
double __cdecl FX_LerpFloat(float grid, float tint, float fraction);
void __cdecl FX_DrawElem_BillboardSprite(FxDrawState *draw);
bool __cdecl FX_CullElementForDraw_Sprite(const FxDrawState *draw);
unsigned int __cdecl FX_CullElementForDraw_FrustumPlaneCount(const FxDrawState *draw);
void __cdecl FX_DrawElem_BillboardSprite_NoCull(FxDrawState *draw);
void __cdecl FX_GenSpriteVerts(FxDrawState *draw, const float *tangent, const float *binormal, const float *normal);
void __cdecl FX_GetSpriteTexCoords(const FxDrawState *draw, float *s0, float *ds, float *t0, float *dt);
void __cdecl FX_DrawElem_OrientedSprite(FxDrawState *draw);
void __cdecl FX_DrawElem_OrientedSprite_NoCull(FxDrawState *draw);
void __cdecl FX_DrawElem_Tail(FxDrawState *draw);
bool __cdecl FX_CullElementForDraw_Tail(const FxDrawState *draw);
char __cdecl FX_CullCylinder(
        const FxCamera *camera,
        unsigned int frustumPlaneCount,
        const float *posWorld0,
        const float *posWorld1,
        float radius);
void __cdecl FX_DrawElem_Tail_Main(FxDrawState *draw);
void __cdecl FX_DrawElem_Line(FxDrawState *draw);
bool __cdecl FX_CullElement_Line(const FxDrawState *draw);
void __cdecl FX_DrawElem_Line_Main(FxDrawState *draw);
void __cdecl FX_DrawElem_RotatedSprite(FxDrawState *draw);
void __cdecl FX_DrawElem_RotatedSprite_NoCull(FxDrawState *draw);
void __cdecl FX_DrawElem_Cloud(FxDrawState *draw);
void __cdecl FX_DrawElem_Cloud_Main(FxDrawState *draw);
void __cdecl FX_SetPlacement(const FxDrawState *draw, GfxScaledPlacement *placement);
double __cdecl FX_GetMsecForSamplingAxis(float msecElapsed, float msecLifeSpan, int atRestFraction);
bool __cdecl FX_CullElementForDraw_Cloud(const FxDrawState *draw);
void __cdecl FX_DrawElem_Model(FxDrawState *draw);
void __cdecl FX_SetPlacementFromPhysics(const FxDrawState *draw, GfxPlacement *placement);
void __cdecl FX_DrawElem_Light(FxDrawState *draw);
bool __cdecl FX_CullElementForDraw_Light(const FxDrawState *draw);
void __cdecl FX_DrawElem_Light_NoCull(FxDrawState *draw);
void __cdecl FX_DrawElem_SpotLight(FxDrawState *draw);
void __cdecl FX_DrawElem_SpotLight_NoCull(FxDrawState *draw);
void __cdecl FX_DrawNonSpriteElems(FxSystem *system, const FxReflectParams *reflect, char cameraType);
void __cdecl FX_BeginIteratingOverEffects_Cooperative(FxSystem *system);
void __cdecl FX_DrawNonSpriteEffect(
        FxSystem *system,
        FxEffect *effect,
        unsigned int elemClass,
        int drawTime,
        const FxReflectParams *reflect,
        char cameraType);
void __cdecl FX_DrawElement(FxSystem *system, const FxElemDef *elemDef, FxElem *elem, FxDrawState *draw);
double __cdecl Vec4Length(const float *v);
bool __cdecl FX_ElemSpawnedVisibleToCamera(unsigned int elemSpawnVisBits, char cameraType);
void __cdecl FX_DrawSpotLight(FxSystem *system);
void __cdecl FX_DrawSpotLightEffect(FxSystem *system, FxEffect *effect, int drawTime);
void __cdecl FX_DrawSpriteElems(FxSystem *system, int drawTime, const FxReflectParams *reflect, char cameraType);
void __cdecl FX_DrawTrailsForEffect(FxSystem *system, FxEffect *effect, int drawTime, const FxReflectParams *reflect);
FxPool<FxTrail,FxTrail> *__cdecl FX_TrailFromHandle(FxSystem *system, unsigned __int16 handle);
void __cdecl FX_DrawTrail(FxSystem *system, FxDrawState *draw, FxTrail *trail);
void __cdecl FX_TrailElem_UncompressBasis(const char (*inBasis)[3], float (*basis)[3]);
FxPool<FxTrailElem,FxTrailElem> *__cdecl FX_TrailElemFromHandle(FxSystem *system, unsigned __int16 handle);
void __cdecl FX_GenTrail_IndsForSegment(
        FxDrawState *draw,
        unsigned __int16 reservedBaseVertex,
        r_double_index_t *outIndices);
void __cdecl Fx_GenTrail_PopulateSegmentDrawState(
        FxDrawState *draw,
        float spawnDist,
        float uCoordOffset,
        const float (*basis)[3],
        FxTrailSegmentDrawState *outState);
void __cdecl FX_GenTrail_VertsForSegment(const FxTrailSegmentDrawState *segmentDrawState, GfxPackedVertex *remoteVerts);
void __cdecl FX_DrawSpriteEffect(FxSystem *system, FxEffect *effect, int drawTime, const FxReflectParams *reflect);
void __cdecl FX_GenerateVerts(FxGenerateVertsCmd *cmd);
void __cdecl FX_FillGenerateVertsCmd(int localClientNum, FxGenerateVertsCmd *cmd);
FxPool<FxTrailElem,FxTrailElem> *__cdecl FX_PoolFromHandle_Generic<FxTrailElem,FxTrailElem,2048>(
        FxPool<FxTrailElem,FxTrailElem> *poolArray,
        unsigned __int16 handle);
FxPool<FxTrail,FxTrail> *__cdecl FX_PoolFromHandle_Generic<FxTrail,FxTrail,128>(
        FxPool<FxTrail,FxTrail> *poolArray,
        unsigned __int16 handle);
void __cdecl FX_EvaluateDistanceFade(FxDrawState *draw);
