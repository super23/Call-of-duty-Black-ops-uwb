#pragma once

void __cdecl CL_CompositeSetupImageCallback(unsigned int *param);
GfxImage *__cdecl CL_CompositeSetupImage();
void __cdecl CL_CompositeReleaseImage(GfxImage *image);
bool __cdecl CL_CompositeValidateImage(GfxImage *image);
int __cdecl CL_CompositePushEmblem(CompositeEmblemLayer *layers, int layerCount);
char __cdecl CL_CompositePushCancel(int id);
char __cdecl CL_CompositeIsComplete(int id);
GfxImage *__cdecl CL_CompositePopImage(int id);
bool __cdecl CL_CompositeUseGenerated(float w, float h);
bool __cdecl CL_CompositeCheckStreaming(CompositeEmblemLayer *layers, int layerCount);
void __cdecl CL_CompositeEmblemCallback(GfxImage *result);
void __cdecl R_HW_InsertFence(IDirect3DQuery9 **fence);
void __cdecl CL_PCCopyImageGenMIPCallback(unsigned int *job);
void __cdecl CL_CompositeRender();
bool __cdecl R_HW_IsFencePending(IDirect3DQuery9 *const *fence);
// local variable allocation has failed, the output may be wrong!
char  CL_CompositeDrawEmblemPhysical@<al>(
        GfxColor a1@<ebp>,
        float x,
        float y,
        float w,
        float h,
        const float *color,
        CompositeEmblemLayer *layers,
        int layerCount);
void __cdecl UV_Transform(const float (*mat)[3], float *vec);
bool __cdecl CL_CompositeIsLayerEmpty(CompositeEmblemLayer *layer);
void __cdecl CL_CompositeResetLayer(CompositeEmblemLayer *layer);
void __cdecl CL_CompositeClearLayer(CompositeEmblemLayer *layer);
