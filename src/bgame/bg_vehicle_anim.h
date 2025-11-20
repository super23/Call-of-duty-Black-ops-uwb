#pragma once

void __cdecl VehAnim_RegisterDvars();
void __cdecl VehAnim_Init();
void __cdecl VehAnim_UpdatePosRot(
        int localClientNum,
        const entityState_s *es,
        clientInfo_t *ci,
        int handler,
        float *origin,
        float *angles);
void __cdecl VehAnim_GetAnimDelta(XAnimTree_s *tree, unsigned int animIndex, float *rot, float *trans);
