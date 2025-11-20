#pragma once

void __cdecl SND_RvParamsDefault(snd_rv_params *params);
double __cdecl SND_RvValidateRange(float value, float min, float max);
void __cdecl SND_RvParamsValidate(snd_rv_params *params);
void __cdecl SND_RvFrameParam(snd_rv_params *params, snd_rv_state *state, unsigned int frameCount);
double __cdecl I_flerp(float a, float b, float w);
void __cdecl SND_RvDelayInit(snd_rv_state *state, unsigned int values);
void __cdecl SND_RvFrame(
        snd_rv_params *params,
        snd_rv_state *state,
        const float *count,
        const float *inLF,
        const float *inRF,
        const float *inLS,
        const float *inRS,
        float *outLF,
        float *outRF,
        float *outLS,
        float *outRS);
