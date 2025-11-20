#include "actor_negotiation.h"

char __fastcall Actor_Negotiation_Start(actor_s *pSelf, ai_state_t ePrevState)
{
  if ( !Path_HasNegotiationNode(&pSelf->Path) )
    return 0;
  Actor_SetOrientMode(pSelf, AI_ORIENT_DONT_CHANGE);
  pSelf->eTraverseMode = AI_TRAVERSE_NOGRAVITY;
  pSelf->prevMoveDir[0] = *(float *)&FLOAT_0_0;
  pSelf->prevMoveDir[1] = *(float *)&FLOAT_0_0;
  return 1;
}

int __fastcall Actor_Negotiation_Think(actor_s *pSelf)
{
  const char *v1; // eax
  char *v2; // eax
  const char *v3; // eax
  ai_animmode_t eAnimMode; // [esp+34h] [ebp-4h]

  PIXBeginNamedEvent(-1, "negotiationthink");
  eAnimMode = AI_ANIM_USE_BOTH_DELTAS_NOGRAVITY;
  if ( pSelf->eTraverseMode == AI_TRAVERSE_GRAVITY )
  {
    eAnimMode = AI_ANIM_USE_BOTH_DELTAS;
  }
  else if ( pSelf->eTraverseMode == AI_TRAVERSE_NOCLIP )
  {
    eAnimMode = AI_ANIM_USE_BOTH_DELTAS_NOCLIP;
  }
  else if ( pSelf->eTraverseMode != AI_TRAVERSE_NOGRAVITY )
  {
    v1 = va("pSelf->eTraverseMode is unhandled %i", pSelf->eTraverseMode);
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_negotiation.cpp", 55, 0, v1) )
      __debugbreak();
  }
  if ( !pSelf->AnimScriptSpecific.func )
  {
    v2 = SL_ConvertToString(pSelf->AnimScriptSpecific.name, SCRIPTINSTANCE_SERVER);
    v3 = va("Invalid negotiation script %s", v2);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_negotiation.cpp",
            58,
            0,
            "%s\n\t%s",
            "pSelf->AnimScriptSpecific.func",
            v3) )
      __debugbreak();
  }
  Actor_AnimSpecific(pSelf, &pSelf->AnimScriptSpecific, eAnimMode, 1);
  pSelf->pushable = 0;
  if ( Actor_IsAnimScriptAlive(pSelf) )
  {
    pSelf->pszDebugInfo = "negotiation";
    Actor_PreThink(pSelf);
    Actor_UpdateOriginAndAngles(pSelf);
    BG_Dog_UpdateAnimationState(-1, &pSelf->ent->s, &level_bgs.actorinfo[pSelf - level.actors]);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
    return 0;
  }
  else
  {
    if ( Path_HasNegotiationNode(&pSelf->Path) )
    {
      if ( pSelf->Path.wNegotiationStartNode >= pSelf->Path.wPathLen
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_negotiation.cpp",
              68,
              0,
              "%s",
              "pSelf->Path.wNegotiationStartNode < pSelf->Path.wPathLen") )
      {
        __debugbreak();
      }
      if ( *((int *)&pSelf->Physics.proximity_data.prims[199].tree + 7 * pSelf->Path.wNegotiationStartNode) < 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_negotiation.cpp",
              69,
              0,
              "%s",
              "pSelf->Path.pts[pSelf->Path.wNegotiationStartNode - 1].iNodeNum >= 0") )
      {
        __debugbreak();
      }
      pSelf->sentient->pNearestNode = Path_ConvertIndexToNode(*((unsigned int *)&pSelf->Physics.proximity_data.prims[199].tree
                                                              + 7 * pSelf->Path.wNegotiationStartNode));
      pSelf->sentient->bNearestNodeBad = 0;
      pSelf->sentient->bNearestNodeValid = 1;
    }
    pSelf->Physics.vWishDelta[0] = *(float *)&FLOAT_0_0;
    pSelf->Physics.vWishDelta[1] = *(float *)&FLOAT_0_0;
    pSelf->Physics.vWishDelta[2] = *(float *)&FLOAT_0_0;
    pSelf->Physics.vVelocity[0] = *(float *)&FLOAT_0_0;
    pSelf->Physics.vVelocity[1] = *(float *)&FLOAT_0_0;
    pSelf->Physics.vVelocity[2] = *(float *)&FLOAT_0_0;
    YawVectors2D(pSelf->fDesiredBodyYaw, pSelf->prevMoveDir, 0);
    Actor_RecalcPath(pSelf);
    Actor_PopState(pSelf);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
    return 1;
  }
}

