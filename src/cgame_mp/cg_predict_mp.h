#pragma once

void __thiscall colgeom_visitor_inlined_t<200>::update(
        colgeom_visitor_inlined_t<200> *this,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        int mask);
int __cdecl CG_ItemListLocalClientNum();
void __cdecl CG_ClearItemList();
void __cdecl CG_BuildItemList(int localClientNum, const snapshot_s *nextSnap);
void __cdecl MovieCameraMove(int localClientNum, usercmd_s *cmd);
void __cdecl FollowCameraMove(cg_s *cgameGlob, centity_s *cent, usercmd_s *cmd);
void __cdecl CG_ClearSavedPlayerState(int localClientNum);
void __cdecl CG_SavePlayerState(int localClientNum);
void __cdecl CG_PredictPlayerState(int localClientNum);
void __cdecl CG_PredictPlayerState_Internal(int localClientNum);
void __cdecl CG_TouchItemPrediction(int localClientNum);
void __cdecl CG_TouchItem(cg_s *cgameGlob, centity_s *cent);
void __cdecl CG_InterpolatePlayerStateViewAngles(int localClientNum, playerState_s *out, usercmd_s *cmd);
char __cdecl CG_ShouldInterpolatePlayerStateViewClamp(int localClientNum, const snapshot_s *prevSnap);
void __cdecl CG_InterpolatePlayerState(int localClientNum, int grabAngles);
void __cdecl CG_CopyFlagsFromSnapshotEntity(int localClientNum);
pmove_t *__thiscall pmove_t::pmove_t(pmove_t *this);
void __thiscall colgeom_visitor_inlined_t<200>::visit(
        colgeom_visitor_inlined_t<200> *this,
        const CollisionAabbTree *tree);
void __thiscall colgeom_visitor_inlined_t<200>::visit(colgeom_visitor_inlined_t<200> *this, const cbrush_t *brush);
void __thiscall colgeom_visitor_inlined_t<200>::update(
        colgeom_visitor_inlined_t<200> *this,
        const float *_mn,
        const float *_mx,
        int mask,
        const float *expand_vec);
colgeom_visitor_t *__thiscall colgeom_visitor_t::colgeom_visitor_t(colgeom_visitor_t *this);
pmove_t *__thiscall pmove_t::pmove_t(pmove_t *this, const pmove_t *__that);
colgeom_visitor_t *__thiscall colgeom_visitor_t::colgeom_visitor_t(
        colgeom_visitor_t *this,
        const colgeom_visitor_t *__that);
