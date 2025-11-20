#pragma once

void __cdecl CG_ClearCompassPingData();
void __cdecl RadarPingEnemyPlayer(CompassActor *actor, int time, unsigned int PerkToIgnore);
bool __cdecl BG_HasPerk(const unsigned int *perks, unsigned int perkIndex);
void __cdecl RadarPingEnemyTurrets(CompassTurrets *turret, int time);
void __cdecl SatellitePingEnemyPlayer(CompassActor *actor, int time, unsigned int PerkToIgnore);
void __cdecl CG_CompassUpdateVehicleInfo(int localClientNum, int entityIndex);
CompassVehicle *__cdecl GetVehicle(int localClientNum, int entityNum);
void __cdecl CG_CompassUpdateHelicopterInfo(int localClientNum, int entityIndex);
CompassHelicopter *__cdecl GetHelicopter(int localClientNum, int entityNum);
void __cdecl CG_CompassUpdateGuidedMissileInfo(int localClientNum, int entityIndex);
CompassGuidedMissile *__cdecl GetGuidedMissile(int localClientNum, int entityNum);
void __cdecl CG_CompassUpdateVehicleOccupantInfo(int localClientNum, int entityIndex);
void __cdecl ActorUpdatePos(int localClientNum, CompassActor *actor, const float *newPos, int actorClientIndex);
bool __cdecl DoesMovementCrossRadar(cg_s *cgameGlob, float radarProgress, const float *p1, const float *p2);
void __cdecl GetRadarLine(cg_s *cgameGlob, float radarProgress, float *line);
double __cdecl GetRadarLineMargin(cg_s *cgameGlob);
bool __cdecl DoesMovementCrossLocalRadar(
        const float *radarPos,
        float localRadarProgress,
        const float *p1,
        const float *p2);
double __cdecl GetLocalRadarRadius(float radarProgress);
bool __cdecl CanLocalPlayerHearActorFootsteps(int localClientNum, const float *actorPos, int actorClientIndex);
void __cdecl CG_CompassUpdateDogInfo(int localClientNum, int entityIndex);
CompassDogs *__cdecl GetDogs(int localClientNum, int entityNum, int time);
void __cdecl CG_CompassUpdateTurretInfo(int localClientNum, int entityIndex);
CompassTurrets *__cdecl GetCompassTurrets(int localClientNum, int entityNum, int time);
void __cdecl CG_CompassRadarPingEnemyTurrets(
        int localClientNum,
        float oldRadarProgress,
        float newRadarProgress,
        float oldLocalRadarProgress,
        float newLocalRadarProgress);
bool __cdecl DoLinesSurroundPoint(cg_s *cgameGlob, float *radarLine1, float *radarLine2, float *pos);
bool __cdecl DoRadiiSurroundPoint(const float *radarPos, float radius1, float radius2, float *pos);
void __cdecl CG_CompassRadarPingEnemyPlayers(
        int localClientNum,
        float oldRadarProgress,
        float newRadarProgress,
        float oldLocalRadarProgress,
        float newLocalRadarProgress);
void __cdecl CG_CompassIncreaseRadarTime(int localClientNum);
void __cdecl CG_AddTurretWeaponPingInfo(int localClientNum, const centity_s *cent, const float *origin, int msec);
void __cdecl CG_CompassAddWeaponPingInfo(int localClientNum, const centity_s *cent, const float *origin, int msec);
void __cdecl CG_CompassAddVehicleWeaponPingInfo(
        int localClientNum,
        const centity_s *cent,
        const float *origin,
        int msec);
void __cdecl CG_CompassAddFakeFirePingInfo(
        int localClientNum,
        unsigned int ownerNum,
        int entNum,
        const float *origin,
        const float *dir,
        int msec);
CompassFakeFire *__cdecl CG_FindFakeFireActor(int localClientNum, int playerIndex, int entNum);
void __cdecl CG_AddEnemyScrambler(int localClientNum, centity_s *scramblerEnt);
void __cdecl CG_ClearNearestEnemyScrambler(int localClientNum);
void __cdecl CG_AddFriendlyScrambler(int localClientNum, float x, float y, int handle);
void __cdecl CG_RemoveFriendlyScrambler(int localClientNum, int handle);
void __cdecl CG_RemoveAllFriendlyScramblers(int localClientNum);
void __cdecl CG_CompassUpdateActors(int localClientNum);
void __cdecl CG_CompassDrawFriendlies(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color);
void __cdecl CG_AddArtilleryPing(int localClientNum, float *position);
void __cdecl CG_CompassDrawArtilleryIcon(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color);
void __cdecl CG_CompassDrawIconAtLocation(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color,
        float *vPos,
        Material *material);
void __cdecl CG_CompassDrawIncomingArtilleryIcon(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color);
void __cdecl GetPositionFromPackedBits(int localClientNum, char bitsPerAxis, int bitMask, int bits, float *vPos);
void __cdecl CG_CompassDrawEnemies(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color);
Material *__cdecl GetCompassPingMaterialForClient(int localClientNum, const clientInfo_t *clientInfo);
Material *__cdecl GetCompassFiringMaterialForClient(int localClientNum, const clientInfo_t *clientInfo);
void __cdecl CG_CompassDrawFakeFire(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color);
void __cdecl CG_CompassDrawRadarEffects(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color);
void __cdecl CG_CompassDrawFlicker(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *color,
        Material *material);
void __cdecl CG_CompassDrawVehicles(
        int localClientNum,
        CompassType compassType,
        int eType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        const float *color);
void __cdecl CG_CompassDrawDogs(
        int localClientNum,
        CompassType compassType,
        int eType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *dogMaterial,
        const float *color);
void __cdecl CG_CompassDrawTurrets(
        int localClientNum,
        CompassType compassType,
        int eType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        const float *color);
Material *__cdecl CG_GetTurretCompassMaterial(centity_s *cent, bool friendly, bool isFiring);
void __cdecl CG_CompassDrawHelicopter(
        int localClientNum,
        CompassType compassType,
        int eType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        const float *color);
void __cdecl CG_CompassDrawGuidedMissile(
        int localClientNum,
        CompassType compassType,
        int eType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        const float *color);
void __cdecl CG_CompassDrawScramblerFriendly(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        const float *color);
void __cdecl CG_ResetCompassData(int localClientNum, int time);
