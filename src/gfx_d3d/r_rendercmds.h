#pragma once

void __cdecl R_InitRenderCommands();
void __cdecl R_ShutdownRenderCommands();
void __cdecl R_RecreateGlassRenderBuffers();
void __cdecl R_InitRenderBuffers();
void __cdecl R_InitDynamicMesh(
        GfxMeshData *mesh,
        unsigned int indexCount,
        unsigned int vertCount,
        unsigned int vertSize);
unsigned int R_InitSpotShadowMeshes();
void __cdecl R_InitGlassRenderBuffers(unsigned int numIndices, unsigned int numVerts, unsigned int vertexSize);
void __cdecl R_FreeGlassRenderBuffers();
void __cdecl R_ShutdownDynamicMesh(GfxMeshData *mesh);
void __cdecl R_ReleaseGlassRenderBuffers();
void __cdecl R_ShutdownRenderBuffers();
void R_ShutdownSpotShadowMeshes();
void __cdecl R_InitTempSkinBuf();
void __cdecl R_ShutdownTempSkinBuf();
void __cdecl R_InitRenderThread();
void __cdecl R_SyncRenderThread();
GfxCmdArray *R_ClearCmdList();
void __cdecl R_IssueRenderCommands(unsigned int type);
unsigned intR_PerformanceCounters();
__int64 R_UpdateSkinCacheUsage();
char __cdecl R_HandOffToBackend(char type);
void __cdecl R_ToggleSmpFrameCmd(char type);
void __cdecl R_AbortRenderCommands();
void __cdecl R_BeginClientCmdList2D();
void __cdecl R_ClearClientCmdList2D();
void __cdecl R_BeginSharedCmdList();
void __cdecl R_BeginCompositingCmdList();
void __cdecl R_AddCmdEndOfList();
GfxCmdHeader *__cdecl R_GetCommandBuffer(GfxRenderCommand renderCmd, int bytes);
unsigned intR_ToggleSmpFrame();
void R_FreeTempSkinBuffer();
unsigned int __cdecl R_GetFrameCount();
GfxViewParms *__cdecl R_AllocViewParms();
void __cdecl R_AddCmdDrawStretchComposite(
        float x,
        float y,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        const float *color,
        GfxImage *composite);
void __cdecl R_AddCmdDrawStretchPicW(
        float x,
        float y,
        float w0,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        const float *color,
        Material *material);
bool __cdecl Material_HasAnyFogableTechnique(const Material *material);
void __cdecl R_AddCmdDrawStretchPic(
        float x,
        float y,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        const float *color,
        Material *material);
void __cdecl R_AddCmdDrawStretchPicRotateXYW(
        float x,
        float y,
        float w0,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        float angle,
        const float *color,
        Material *material);
void __cdecl R_AddCmdDrawStretchPicRotateXY(
        float x,
        float y,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        float angle,
        const float *color,
        Material *material);
void __cdecl R_AddCmdDrawStretchPicRotateST(
        float x,
        float y,
        float w,
        float h,
        float centerS,
        float centerT,
        float radiusST,
        float scaleFinalS,
        float scaleFinalT,
        float angle,
        const float *color,
        Material *material);
void __cdecl R_AddCmdDrawTextWithCursor(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float w,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        int cursorPos,
        char cursor);
GfxCmdDrawText2D *__cdecl AddBaseDrawTextCmd(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float w,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        int cursorPos,
        char cursor);
void __cdecl R_AddCmdDrawText(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style);
void __cdecl R_AddCmdDrawTextW(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float w,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style);
void __cdecl R_AddCmdDrawTextSubtitle(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        const float *glowColor,
        bool cinematic);
char __cdecl SetDrawText2DGlowParms(GfxCmdDrawText2D *cmd, const float *color, const float *glowColor);
void __cdecl R_AddCmdDrawTextWithEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float w,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
char __cdecl SetDrawText2DTypewriterFXParms(
        GfxCmdDrawText2D *cmd,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl R_AddCmdDrawTextWithCOD7DecodeEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
char __cdecl SetDrawText2DCOD7DecodeFXParms(
        GfxCmdDrawText2D *cmd,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl R_AddCmdDrawTextWithCOD7TypeWriterEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
char __cdecl SetDrawText2DCOD7TypeWriterFXParms(
        GfxCmdDrawText2D *cmd,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl R_AddCmdDrawTextWithRedactEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        int fxRedactDecayStartTime,
        int fxRedactDecayDuration);
char __cdecl SetDrawText2DRedactFXParms(
        GfxCmdDrawText2D *cmd,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        int fxRedactDecayStartTime,
        int fxRedactDecayDuration);
void __cdecl R_AddCmdDrawTextWithPopInEffects(
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        float rotation,
        const float *color,
        int style,
        const float *glowColor,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
char __cdecl SetDrawText2DPopInWriteFXParms(
        GfxCmdDrawText2D *cmd,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl R_AddCmdDrawConsoleText(
        char *textPool,
        int poolSize,
        int firstChar,
        int charCount,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style);
GfxCmdDrawText2D *__cdecl AddBaseDrawConsoleTextCmd(
        char *textPool,
        int poolSize,
        int firstChar,
        int charCount,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style);
void __cdecl CopyPoolTextToCmd(char *textPool, int poolSize, int firstChar, int charCount, GfxCmdDrawText2D *cmd);
void __cdecl R_AddCmdDrawConsoleTextTypewriterFX(
        char *textPool,
        int poolSize,
        int firstChar,
        int charCount,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl R_AddCmdDrawConsoleTextSubtitle(
        char *textPool,
        int poolSize,
        int firstChar,
        int charCount,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor);
void __cdecl R_AddCmdDrawConsoleTextPulseFX(
        char *textPool,
        int poolSize,
        int firstChar,
        int charCount,
        Font_s *font,
        float x,
        float y,
        float xScale,
        float yScale,
        const float *color,
        int style,
        const float *glowColor,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        Material *fxMaterial,
        Material *fxMaterialGlow);
char __cdecl SetDrawText2DPulseFXParms(
        GfxCmdDrawText2D *cmd,
        Material *fxMaterial,
        Material *fxMaterialGlow,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration);
void __cdecl R_AddCmdDrawQuadPicW(const float (*verts)[2], float w, const float *color, Material *material);
void __cdecl R_AddCmdDrawQuadPic(const float (*verts)[2], const float *color, Material *material);
void __cdecl R_AddCmdSetCustomConstant(unsigned int type, const float *vec);
void __cdecl R_AddCmdSetScissorValues(bool enabled, int x, int y, int width, int height);
void __cdecl R_AddCmdResolveComposite(void (__cdecl *callback)(GfxImage *));
void __cdecl R_AddCmdPCCopyImageGenMIP(void (__cdecl *callback)(void *), GfxImage *image, void *job);
void __cdecl R_BeginFrame();
const dvar_s *R_UpdateFrontEndDvarOptions();
bool __cdecl R_LightTweaksModified();
bool __cdecl R_GpuSyncModified();
void R_SetTestLods();
bool __cdecl R_AreAnyImageOverridesActive();
void R_SetOutdoorFeatherConst();
void R_EnvMapOverrideConstants();
void __cdecl R_EndFrame();
void __cdecl R_AddCmdClearScreen(int whichToClear, const float *color, float depth, unsigned __int8 stencil);
GfxCmdDrawQuadList2D *__cdecl R_AddCmdDrawQuadList2D(Material *materialHandle, int quadCount);
void __cdecl R_AddCmdDrawEmblemLayer(
        Material *materialHandle,
        const GfxImage *image,
        int colorIdx,
        float outlineSize,
        float smoothSize,
        GfxQuadVertex *verts);
void __cdecl R_AddCmdProjectionSet2D();
void __cdecl R_AddCmdProjectionSet(GfxProjectionTypes projection);
void __cdecl R_AddCmdDrawFramed(
        float x,
        float y,
        float w,
        float h,
        float thicknessW,
        float thicknessH,
        float thicknessTex,
        int sides,
        const float *color,
        Material *material);
void __cdecl R_BeginRemoteScreenUpdate();
void __cdecl R_EndRemoteScreenUpdate(void (__cdecl *pumpfunc)());
void __cdecl R_PushRemoteScreenUpdate(int remoteScreenUpdateNesting);
int __cdecl R_PopRemoteScreenUpdate();
bool __cdecl R_IsInRemoteScreenUpdate();
bool __cdecl R_IsRemoteScreenUpdateActive();
GfxViewInfo *__thiscall GfxViewInfo::GfxViewInfo(GfxViewInfo *this);
