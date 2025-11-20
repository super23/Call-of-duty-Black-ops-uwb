#pragma once

bool __cdecl ValidGamePadButtonIcon(unsigned int letter);
void __cdecl RB_CopyBackendStats();
void __cdecl RB_SetIdentity();
void __cdecl R_SetVertex2d(GfxVertex *vert, float x, float y, float s, float t, unsigned int color);
void __cdecl RB_DrawFullSceneQuad(
        const Material *material,
        unsigned __int8 renderTargetId,
        float s0,
        float t0,
        float s1,
        float t1,
        GfxPrimStatsTarget statsTarget);
void __cdecl RB_DrawSW4Quads(const Material *material, int ParticleCount);
void __cdecl RB_DrawStretchPic(
        const Material *material,
        float x,
        float y,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        unsigned int color,
        GfxPrimStatsTarget statsTarget);
void __cdecl RB_DrawStretchPicW(
        const Material *material,
        float x,
        float y,
        float w0,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        unsigned int color,
        GfxPrimStatsTarget statsTarget);
void __cdecl R_SetVertex4d(GfxVertex *vert, float x, float y, float z, float w, float s, float t, unsigned int color);
void __cdecl RB_DrawStretchPicZ(
        const Material *material,
        float x,
        float y,
        float z,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        unsigned int color,
        GfxPrimStatsTarget statsTarget);
void __cdecl R_SetVertex3d(GfxVertex *vert, float x, float y, float z, float s, float t, const unsigned __int8 *color);
void __cdecl RB_DrawFullScreenColoredQuad(
        const Material *material,
        float s0,
        float t0,
        float s1,
        float t1,
        unsigned int color);
void __cdecl RB_FullScreenColoredFilter(const Material *material, unsigned int color);
void __cdecl RB_FullScreenFilter(const Material *material);
void __cdecl RB_Filter(const Material *material, const GfxViewInfo *viewInfo);
void __cdecl RB_ColoredFilter(const Material *material, const GfxViewInfo *viewInfo, unsigned int color);
void __cdecl RB_SplitScreenFilter(const Material *material, const GfxViewInfo *viewInfo, unsigned int color);
void __cdecl RB_SplitScreenTexCoords(float x, float y, float w, float h, float *s0, float *t0, float *s1, float *t1);
void __fastcall R_ResolveIntZ_PC(bool a1);
NVDX_ObjectHandle__ *__thiscall R_GetIntZHandles(NVDX_ObjectHandle__ *this);
void __cdecl R_Resolve(GfxCmdBufContext context, GfxImage *image);
void __cdecl RB_StretchCompositeCmd(GfxRenderCommandExecState *execState);
void __cdecl R_SetCodeImageSamplerState(
        GfxCmdBufSourceState *source,
        unsigned int codeTexture,
        unsigned __int8 samplerState);
void __cdecl RB_StretchPicCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_StretchPicCmdFlipST(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawStretchPicFlipST(
        const Material *material,
        float x,
        float y,
        float w,
        float h,
        float s0,
        float t0,
        float s1,
        float t1,
        unsigned int color,
        GfxPrimStatsTarget statsTarget);
void __cdecl RB_StretchPicRotateXYCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_StretchPicRotateSTCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawQuadPicCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawQuadList2DCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawEmblemLayer(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawFullScreenColoredQuadCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_StretchRawCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_StretchRaw(int x, int y, int w, int h, int cols, int rows, const unsigned __int8 *data);
void __cdecl RB_DrawFramedCmd(GfxRenderCommandExecState *execState);
void __cdecl TessQuad(unsigned __int16 v0, unsigned __int16 v1, unsigned __int16 v2, unsigned __int16 v3);
void __cdecl RB_SetEyeOffsetConstant(GfxCmdBufSourceState *source, const float *drawSurfListViewOrigin);
void __cdecl R_DrawSurfs(GfxCmdBufContext context, GfxCmdBufState *prepassState, const GfxDrawSurfListInfo *info);
unsigned int __cdecl R_RenderDrawSurfListMaterial(const GfxDrawSurfListArgs *listArgs, GfxCmdBufContext prepassContext);
unsigned int __cdecl R_SkipDrawSurfListMaterial(const GfxDrawSurf *drawSurfList, unsigned int drawSurfCount);
void __cdecl R_TessEnd(GfxCmdBufContext context, GfxCmdBufContext prepassContext);
void __cdecl RB_ClearScreenCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_SetGammaRamp(const GfxGammaRamp *gammaTable);
void __cdecl RB_BlendSavedScreenFlashedCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawPointsCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawPoints2D(const GfxCmdDrawPoints *cmd);
void __cdecl RB_DrawPoints3D(const GfxCmdDrawPoints *cmd);
void __cdecl RB_DrawLines2D(int count, int width, const GfxPointVertex *verts);
void __cdecl RB_DrawLines3D(int count, int width, const GfxPointVertex *verts, bool depthTest);
void __cdecl RB_DrawLinesCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawTrianglesCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawTriangles_Internal(
        const Material *material,
        unsigned __int8 techType,
        __int16 indexCount,
        const unsigned __int16 *indices,
        __int16 vertexCount,
        const float (*xyzw)[4],
        const float (*normal)[3],
        const GfxColor *color,
        const float (*st)[2]);
void __cdecl R_SetVertex4dWithNormal(
        GfxVertex *vert,
        float x,
        float y,
        float z,
        float w,
        float nx,
        float ny,
        float nz,
        float s,
        float t,
        const unsigned __int8 *color);
void __cdecl RB_SetCustomConstantCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_SetMaterialColorCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_SetViewportCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_SetScissorCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_ResolveCompositeCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_PCCopyImageGenMIPCmd(GfxRenderCommandExecState *execState);
void __cdecl GetShiftColor(
        GfxColor targetColor,
        int fxBirthTime,
        int fxDecayStart,
        int fxDecayDuration,
        GfxColor startColor,
        GfxColor *shiftColor);
void __cdecl RB_DrawText(const char *text, Font_s *font, float x, float y, GfxColor color);
void __cdecl DrawText2D(
        const char *text,
        float x,
        float y,
        float w,
        Font_s *font,
        float xScale,
        float yScale,
        float sinAngle,
        float cosAngle,
        GfxColor color,
        int maxLength,
        int renderFlags,
        int cursorPos,
        char cursorLetter,
        float padding,
        GfxColor glowForcedColor,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        int fxRedactDecayStartTime,
        int fxRedactDecayDuration,
        const Material *fxMaterial,
        const Material *fxMaterialGlow);
void __cdecl RB_DrawStretchPicRotate(
        const Material *material,
        float x,
        float y,
        float w,
        float width,
        float height,
        float s0,
        float t0,
        float s1,
        float t1,
        float sinAngle,
        float cosAngle,
        unsigned int color,
        GfxPrimStatsTarget statsTarget);
void __cdecl RB_LookupColor(unsigned __int8 c, unsigned __int8 *color);
double __cdecl RB_DrawHudIcon(
        const char *text,
        float x,
        float y,
        float sinAngle,
        float cosAngle,
        Font_s *font,
        float xScale,
        float yScale,
        unsigned int color);
GfxColor __cdecl RB_ProcessLetterColor(unsigned int color, unsigned int letter);
void __cdecl RB_DrawCursor(
        const Material *material,
        unsigned __int8 cursor,
        float x,
        float y,
        float sinAngle,
        float cosAngle,
        Font_s *font,
        float xScale,
        float yScale,
        unsigned int color);
void __cdecl RotateXY(
        float cosAngle,
        float sinAngle,
        float pivotX,
        float pivotY,
        float x,
        float y,
        float *outX,
        float *outY);
double __cdecl GetMonospaceWidth(Font_s *font, char renderFlags);
void __cdecl GlowColor(GfxColor *result, GfxColor baseColor, GfxColor forcedGlowColor, char renderFlags);
char __cdecl SetupFadeinFXVars(
        const char *text,
        int maxLength,
        int renderFlags,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        bool *resultDrawRandChar,
        int *resultRandSeed,
        bool *resultDecaying,
        int *resultdecayTimeElapsed);
char __cdecl SetupCOD7DecodeFXVars(
        const char *text,
        int maxLength,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        int *randSeed,
        int *resultRandomCharsLength,
        int *resultMaxLength,
        bool *resultDecaying,
        int *resultDecayTimeElapsed);
char __cdecl SetupTypewriterFXVars(
        const char *text,
        int maxLength,
        int renderFlags,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        bool *resultDrawRandChar,
        int *resultRandSeed,
        int *resultMaxLength,
        bool *resultDecaying,
        int *resultdecayTimeElapsed);
char __cdecl SetupPopInFXVars(
        const char *text,
        int maxLength,
        int renderFlags,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        float *sizeIncrease,
        int *resultRandSeed,
        int *resultMaxLength,
        bool *resultDecaying,
        int *resultdecayTimeElapsed);
char __cdecl SetupPulseFXVars(
        const char *text,
        int maxLength,
        char renderFlags,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        bool *resultDrawRandChar,
        int *resultRandSeed,
        int *resultMaxLength,
        bool *resultDecaying,
        int *resultdecayTimeElapsed);
char __cdecl SetupRedactTextFXVars(
        const char *text,
        int maxLength,
        int renderFlags,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int fxDecayDuration,
        bool *resultDrawRandChar,
        int *resultRandSeed,
        int *resultMaxLength,
        bool *resultDecaying,
        int *resultdecayTimeElapsed);
void __cdecl SetupRedactFXVars(
        int *randSeed,
        int fxBirthTime,
        int maxLength,
        int fxRedactDecayStartTime,
        int fxRedactDecayDuration,
        int mode,
        float *src_rect,
        float *max_rect,
        float *out_rect,
        float *u,
        Material **mat);
void __cdecl GetDecayingStringAlphaInfo(
        int decayTimeElapsed,
        int fxDecayDuration,
        unsigned __int8 alpha,
        unsigned __int8 *resultAlpha);
void __cdecl DrawTextFxExtraCharacter(
        const Material *material,
        int charIndex,
        float x,
        float y,
        float w,
        float h,
        float sinAngle,
        float cosAngle,
        unsigned int color);
int __cdecl ModulateByteColors(unsigned __int8 colorA, unsigned __int8 colorB);
void __cdecl RB_DrawTextInSpace(
        const char *text,
        Font_s *font,
        const float *org,
        const float *xPixelStep,
        const float *yPixelStep,
        unsigned int color);
void __cdecl RB_DrawCharInSpace(
        const Material *material,
        float *xyz,
        const float *dx,
        const float *dy,
        const Glyph *glyph,
        unsigned int color);
void __cdecl RB_DrawText2DCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_DrawText3DCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_ProjectionSetCmd(GfxRenderCommandExecState *execState);
void __cdecl RB_ResetStatTracking(int viewIndex);
void __cdecl RB_BeginFrame(const GfxBackEndData *data);
const GfxBackEndData *RB_PatchStaticModelCache();
void __cdecl RB_EndFrame(char drawType);
void RB_SwapBuffers();
void RB_UpdateBackEndDvarOptions();
void __cdecl RB_ExecuteRenderCommandsLoop(const void *cmds, int *ui3dTextureWindow);
void __cdecl RB_Draw3D();
void __cdecl RB_CallExecuteRenderCommands();
void __cdecl RB_UpdateDynamicBuffers(GfxBackEndData *backendData);
void   RB_RenderThread(int a1@<esi>, unsigned int threadContext);
void __cdecl RB_RenderCommandFrame(const GfxBackEndData *data);
bool __cdecl RB_BackendTimeout(int gpuIndex);
void __cdecl RB_InitBackendGlobalStructs();
void __cdecl RB_SetBspImages();
void __cdecl RB_InitCodeImages();
void __cdecl RB_InitSingleCodeImage(
        unsigned int codeTexture,
        const GfxImage *image,
        unsigned __int8 samplerState,
        const char *name);
void __cdecl RB_RegisterBackendAssets();
void __cdecl RB_SaveScreen_BlendBlurred(const GfxBlendSaveScreenBlurredParam *p, const GfxViewInfo *viewInfo);
void __cdecl RB_SaveScreen_BlendFlashed(const GfxBlendSaveScreenFlashedParam *p, const GfxViewInfo *viewInfo);
void __cdecl RB_SaveScreen(const GfxSaveScreenParam *p, const GfxViewInfo *viewInfo);
void __cdecl R_ResolveSection(GfxCmdBufContext context, GfxImage *image);
