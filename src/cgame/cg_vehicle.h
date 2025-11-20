#pragma once

void __cdecl CG_UpdateVehicleWaterFX(int localClientNum, centity_s *cent);
// local variable allocation has failed, the output may be wrong!
void  CG_Vehicle_PreControllers(
        cStaticModel_s *a1@<ebp>,
        int a2@<esi>,
        int localClientNum,
        const DObj *obj,
        centity_s *cent);
void  VEH_UpdateMaterialTime(float a1@<ebp>, centity_s *cent, int dt, float speed);
int __cdecl GetSoundInfoForVehicle(int localClientNum, const centity_s *cent, float *slideRate);
void __cdecl GetSoundOriginForVehicle(const centity_s *cent, float *origin);
void __cdecl CG_CrossFadeVehicleMaterialEffects(
        int localClientNum,
        const centity_s *cent,
        const vehicle_info_t *info,
        unsigned int groundEffectIndex,
        int surfType,
        float ratio,
        int fadeTime);
void __cdecl CG_UpdateVehicleNotifies(int localClientNum, centity_s *cent);
void __cdecl CG_UpdateVehicleSounds(int localClientNum, centity_s *cent);
void __cdecl CG_UpdateSurfaceSounds(int localClientNum, centity_s *cent, const vehicle_info_t *info);
void  CG_UpdateWheeledSurfaceSounds(
        int a1@<ebp>,
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info);
// local variable allocation has failed, the output may be wrong!
void  CG_UpdateTankSurfaceSounds(
        unsigned int a1@<ebp>,
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info);
void __cdecl CG_UpdateFireSounds(
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle);
void __cdecl CG_UpdateBoostSounds(
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle);
void  CG_UpdateVehicleEngineSounds(
        int a1@<ebp>,
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle);
void __cdecl CG_UpdateTurretAndOverheatSounds(
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle);
