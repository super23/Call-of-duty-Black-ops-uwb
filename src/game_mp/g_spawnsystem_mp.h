#pragma once

void __cdecl SpawnSystem_SortPoints();
void __cdecl SpawnSystem_SortPointsForTeamByScore(int team);
void __cdecl SpawnSystem_Init();
const dvar_s *SpawnSystem_RegisterDvars();
int __cdecl SpawnSystem_FindPointByEnt(int entNum);
void __cdecl SpawnSystem_SetRandomVariation(float variation);
void __cdecl SpawnSystem_ClearPoints();
char __cdecl SpawnSystem_AddPoint(unsigned int team, gentity_s *ent);
void __cdecl SpawnSystem_ClearPointsBaseWeight(int teammask);
void __cdecl SpawnSystem_SetPointBaseWeight(SpawnPoint *point, char teammask, float weight);
void __cdecl SpawnSystem_SetPointsBaseWeight(int teammask, float *objective_position, float angle, float score);
SpawnInfluencerPreset *__cdecl SpawnSystem_SetPresetData(
        int influencerIndex,
        eInfluencerType type,
        eInfluencerShape shape,
        float length,
        float radius,
        float score,
        eInfluencerScoreCurve score_curve);
void __cdecl SpawnSystem_InitInfluencer(
        SpawnInfluencer *influencer,
        eInfluencerType type,
        eInfluencerShape shape,
        const float *origin,
        const float *forward,
        const float *up,
        float radius,
        float axis_length,
        float score,
        eInfluencerScoreCurve score_curve,
        int teamMask,
        unsigned int entNum,
        int timeout);
int __cdecl SpawnSystem_AddSphereInfluencer(
        eInfluencerType type,
        float *origin,
        float radius,
        float score,
        eInfluencerScoreCurve score_curve,
        int teamMask,
        unsigned int entNum,
        int timeout);
int __cdecl SpawnSystem_FindFreeInfluencer();
void __cdecl SpawnSystem_ComputeInfluencerBounds(SpawnInfluencer *influencer);
void __cdecl SpawnSystem_InsertSortedInfluencer(SpawnInfluencer *influencer);
int __cdecl SpawnSystem_AddCylinderInfluencer(
        eInfluencerType type,
        float *origin,
        float *forward,
        float *up,
        float radius,
        float axis_length,
        float score,
        eInfluencerScoreCurve score_curve,
        int teamMask,
        unsigned int entNum,
        int timeout);
void __cdecl SpawnSystem_UpdateCylinderInfluencer(SpawnInfluencer *influencer);
void __cdecl SpawnSystem_CalculateCylinderLength(SpawnInfluencer *influencer);
void __cdecl SpawnSystem_CalculateCylinderAxis(SpawnInfluencer *influencer);
void __cdecl QuatMultiply(const float *in1, const float *in2, float *out);
void __cdecl SpawnSystem_CalculateCylinderAxisMidpoint(SpawnInfluencer *influencer);
char __cdecl SpawnSystem_RemoveInfluencer(unsigned int influencer_index);
void __cdecl SpawnSystem_RemoveSortedInfluencer(SpawnInfluencer *influencer);
void __cdecl SpawnSystem_RemoveSortedInfluencerByIndex(unsigned int influencer_index);
char __cdecl SpawnSystem_EnableInfluencer(unsigned int influencer_index, bool enabled);
char __cdecl SpawnSystem_SetInfluencerTeamMask(unsigned int influencer_index, int team_mask);
void __cdecl SpawnSystem_ClearEntityInfluencers(gentity_s *ent);
void __cdecl SpawnSystem_ClearTimedOutInfluencers();
SpawnInfluencer *__cdecl SpawnSystem_GetInfluencerList();
void __cdecl SpawnSystem_Update();
void SpawnSystem_SortInfluencers();
void SpawnSystem_UpdateInfluencerBounds();
void SpawnSystem_UpdateInfluencerOriginAngles();
double __cdecl SpawnSystem_InfluenceWeight(
        eInfluencerScoreCurve curve_type,
        float influencer_radius_sq,
        float distance_sq,
        float score);
double __cdecl SpawnSystem_ComputeSphereInfluence(SpawnInfluencer *influencer, SpawnPoint *point);
double __cdecl SpawnSystem_ComputeCylinderInfluence(SpawnInfluencer *influencer, SpawnPoint *point);
void __cdecl SpawnSystem_ComputePointWeight(
        gentity_s *player,
        unsigned int point_team,
        unsigned int influencer_team,
        SpawnPoint *point,
        bool bIsArchiveCall);
void __cdecl SpawnSystem_ComputePointWeights(gentity_s *player, unsigned int point_team, unsigned int influencer_team);
SpawnPoint *__cdecl SpawnSystem_GetSpawnPoints();
int __cdecl SpawnSystem_GetSpawnPointCount();
void __cdecl SpawnSystem_SortPointsByScore(unsigned int team);
int __cdecl SpawnSystem_UpdateSpawnPointsForTeam(unsigned int point_team, unsigned int influencer_team);
int __cdecl SpawnSystem_UpdateSpawnPointsForPlayer(
        gentity_s *ent,
        unsigned int point_team,
        unsigned int influencer_team);
int __cdecl SpawnSystem_GetSortedPointEntNum(unsigned int team, unsigned int index);
char __cdecl SpawnSystem_IsSpawnPointVisible(float *origin, float *angles, int other_team, gentity_s *ignore_entity);
char __cdecl SpawnSystem_IsSpawnPointVisible(
        float *origin,
        float *angles,
        int other_team,
        gentity_s *ignore_entity,
        bool render_mode_success);
void __cdecl SpawnSystem_DebugRenderVisibilityBox(float *point1, float *angles1, const float *color);
void __cdecl SpawnSystem_DebugRenderVisibilityCheck(
        float *point1,
        float *angles1,
        float *point2,
        float *angles2,
        const float *color,
        float height_offset,
        gentity_s *ignore_entity,
        bool collision_test);
void __cdecl SpawnSystem_DebugSideSwitch(bool enabled);
void SpawnSystem_DebugRender();
void __cdecl SpawnSystem_DebugRenderTeamPointWeights(int teamMask);
void __cdecl SpawnSystem_DebugRenderTeamPoints(int teamMask);
void __cdecl SpawnSystem_DebugRenderSpawnPointBox(SpawnPoint *sp, const float *color, int bDepthTest);
void __cdecl SpawnSystem_SetSpawnPointsOrder(int team);
void __cdecl SpawnSystem_DebugUpdateSpawnPoints(
        gentity_s *player,
        unsigned int point_team,
        unsigned int influencer_team);
