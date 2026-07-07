#include "bg_vehicle_anim.h"
#include "bg_local.h"
#include <universal/dvar.h>
#include <cstring>
#include <universal/com_math_anglevectors.h>
#include <universal/com_math.h>
#include "bg_misc.h"
#include <xanim/xanim.h>

vehAnimClient_t clVehAnimClients[1];
vehAnimClient_t svVehAnimClients[32];
const dvar_t *vehanim_enable;
const dvar_t *vehanim_debug;

// Decomp: CoDMPServer.c:153311  (RVA 0044EDF0)
void __cdecl VehAnim_RegisterDvars()
{
    vehanim_enable = _Dvar_RegisterBool("vehanim_enable", 1, 0x80u, "Enable player vehicle anims");
    vehanim_debug = _Dvar_RegisterBool("vehanim_debug", 0, 0x80u, "Show debug information for vehicle anims");
}

// Decomp: CoDMPServer.c:153322  (RVA 0044EE50)
void __cdecl VehAnim_Init()
{
    *(unsigned int *)&clVehAnimClients[0].initialized = 0;
    clVehAnimClients[0].curSeat = VEH_POS_DRIVER;
    clVehAnimClients[0].prevSeat = VEH_POS_DRIVER;
    *(unsigned int *)&clVehAnimClients[0].moving = 0;
    clVehAnimClients[0].animStage = 0;
    clVehAnimClients[0].animIndex = 0;
    clVehAnimClients[0].tagName = 0;
    memset((unsigned __int8 *)svVehAnimClients, 0, sizeof(svVehAnimClients));
}

// Decomp: CoDMPServer.c:153341  (RVA 0044EE90)
void __cdecl VehAnim_UpdatePosRot(
                int localClientNum,
                const entityState_s *es,
                clientInfo_t *ci,
                int handler,
                float *origin,
                float *angles)
{
    float *axis_row;
    float trans_along_axis;
    float finalAxis[3][3];
    float trans[3];
    float tempAxis[3][3];
    float yaw;
    float rot[2];
    int i;
    float axis[3][3];
    if ( vehanim_enable->current.enabled )
    {
        VehAnim_GetAnimDelta(ci->pXAnimTree, es->animState.state & 0xFFFFFBFF, rot, trans);
        AnglesToAxis(angles, axis);
        for ( i = 0; i < 3; ++i )
        {
            axis_row = axis[i];
            trans_along_axis = trans[i];
            *origin = (float)(trans_along_axis * *axis_row) + *origin;
            origin[1] = (float)(trans_along_axis * axis_row[1]) + origin[1];
            origin[2] = (float)(trans_along_axis * axis_row[2]) + origin[2];
        }
        yaw = RotationToYaw(rot);
        YawToAxis(yaw, tempAxis);
        MatrixMultiply(tempAxis, axis, finalAxis);
        AxisToAngles(finalAxis, angles);
    }
}

// Decomp: CoDMPServer.c:153373  (RVA 0044EFE0)
void __cdecl VehAnim_GetAnimDelta(XAnimTree_s *tree, unsigned int animIndex, float *rot, float *trans)
{
    float frac;
    XAnim_s *pXAnims;
    BG_CheckThread();
    iassert(bgs);
    //if ( !*(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8)
    //    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_vehicle_anim.cpp", 144, 0, "%s", "bgs") )
    //{
    //    __debugbreak();
    //}
    //pXAnims = *(XAnim_s **)(**(unsigned int **)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) + 578412);
    pXAnims = bgs->animData->animScriptData.animTree.anims;
    frac = XAnimGetTime(tree, animIndex);
    XAnimGetAbsDelta(pXAnims, animIndex, rot, trans, frac);
}
