#include "cg_vehicle.h"

void __cdecl CG_UpdateVehicleWaterFX(int localClientNum, centity_s *cent)
{
  float axis[3][3]; // [esp+1Ch] [ebp-30h] BYREF
  float point[3]; // [esp+40h] [ebp-Ch] BYREF

  if ( CL_LocalClient_IsFirstActive(localClientNum) )
  {
    AnglesToAxis(cent->pose.angles, axis);
    point[0] = (float)(dist1 * axis[0][0]) + cent->pose.origin[0];
    point[1] = (float)(dist1 * axis[0][1]) + cent->pose.origin[1];
    point[2] = (float)(dist1 * axis[0][2]) + cent->pose.origin[2];
    R_WaterSimulationSetHeight(cent->pose.origin, radius, depth, lerpRate_0);
    point[0] = (float)(dist2 * axis[0][0]) + cent->pose.origin[0];
    point[1] = (float)(dist2 * axis[0][1]) + cent->pose.origin[1];
    point[2] = (float)(dist2 * axis[0][2]) + cent->pose.origin[2];
    R_WaterSimulationSetHeight(point, radius2, depth2, lerpRate2);
  }
}

// local variable allocation has failed, the output may be wrong!
void  CG_Vehicle_PreControllers(
        cStaticModel_s *a1@<ebp>,
        int a2@<esi>,
        int localClientNum,
        const DObj *obj,
        centity_s *cent)
{
  const XModel *Model; // eax
  float v6; // xmm0_4
  long double v7; // [esp+8h] [ebp-5BCh]
  long double v8; // [esp+10h] [ebp-5B4h]
  int v9; // [esp+1Ch] [ebp-5A8h]
  cg_s **v10; // [esp+24h] [ebp-5A0h]
  unsigned int v11[3]; // [esp+2Ch] [ebp-598h] BYREF
  col_context_t context; // [esp+38h] [ebp-58Ch] BYREF
  float expand_vec[3]; // [esp+60h] [ebp-564h]
  int contents_mask; // [esp+6Ch] [ebp-558h]
  float *v15; // [esp+70h] [ebp-554h]
  float *v16; // [esp+74h] [ebp-550h]
  _BYTE v17[12]; // [esp+78h] [ebp-54Ch] BYREF
  float trace_points[6][2][3]; // [esp+84h] [ebp-540h] BYREF
  float wheelPos[3]; // [esp+114h] [ebp-4B0h]
  const DObjAnimMat *mtx; // [esp+120h] [ebp-4A4h] BYREF
  unsigned int boneIndex; // [esp+124h] [ebp-4A0h]
  int presence_mask; // [esp+128h] [ebp-49Ch]
  float mx[3]; // [esp+12Ch] [ebp-498h] BYREF
  float mn[3]; // [esp+138h] [ebp-48Ch] BYREF
  int bitmask; // [esp+144h] [ebp-480h]
  float angles_vec3[3]; // [esp+14Ch] [ebp-478h] BYREF
  float axis_loc_vec3[3]; // [esp+158h] [ebp-46Ch] BYREF
  phys_vec3 axis_loc; // [esp+164h] [ebp-460h]
  float m_wheel_pos; // [esp+174h] [ebp-450h]
  float v30; // [esp+178h] [ebp-44Ch]
  float v31; // [esp+17Ch] [ebp-448h]
  float rotation; // [esp+180h] [ebp-444h]
  float v33; // [esp+184h] [ebp-440h]
  float v34; // [esp+188h] [ebp-43Ch]
  float v35; // [esp+18Ch] [ebp-438h]
  float lerp; // [esp+190h] [ebp-434h]
  float v37; // [esp+194h] [ebp-430h]
  const VehicleParameter *v38; // [esp+198h] [ebp-42Ch]
  float m_wheel_displaced_center_dist; // [esp+19Ch] [ebp-428h]
  float lerpRate; // [esp+1A0h] [ebp-424h]
  rigid_body_constraint_wheel *v41; // [esp+1A4h] [ebp-420h]
  const VehicleParameter *v42; // [esp+1A8h] [ebp-41Ch]
  float v43; // [esp+1ACh] [ebp-418h]
  float v44; // [esp+1B0h] [ebp-414h]
  rigid_body_constraint_wheel *v45; // [esp+1B4h] [ebp-410h]
  float v46; // [esp+1B8h] [ebp-40Ch]
  const VehicleParameter *v47; // [esp+1BCh] [ebp-408h]
  rigid_body_constraint_wheel *wheel2; // [esp+1C0h] [ebp-404h]
  float newWheelHeight; // [esp+1C4h] [ebp-400h]
  float m_wheel_radius; // [esp+1C8h] [ebp-3FCh]
  const VehicleParameter *m_parameter; // [esp+1CCh] [ebp-3F8h]
  rigid_body_constraint_wheel *wheelRadiusDelta; // [esp+1D0h] [ebp-3F4h]
  int j; // [esp+1D4h] [ebp-3F0h]
  const DObjAnimMat *BasePose; // [esp+1D8h] [ebp-3ECh]
  rigid_body_constraint_wheel *wheel; // [esp+1DCh] [ebp-3E8h]
  float *v56; // [esp+1E0h] [ebp-3E4h]
  float boneMtxList[3]; // [esp+1E4h] [ebp-3E0h] OVERLAPPED BYREF
  float axis[4][3]; // [esp+1F0h] [ebp-3D4h] BYREF
  const WeaponDef *suspTravel; // [esp+220h] [ebp-3A4h]
  int v60; // [esp+224h] [ebp-3A0h]
  int *v61; // [esp+228h] [ebp-39Ch]
  const WeaponDef *turretWeapon; // [esp+22Ch] [ebp-398h]
  float v63; // [esp+230h] [ebp-394h]
  const clientInfo_t *ci; // [esp+234h] [ebp-390h]
  int clientNum; // [esp+238h] [ebp-38Ch]
  float v66; // [esp+23Ch] [ebp-388h]
  float v67; // [esp+240h] [ebp-384h]
  float v68; // [esp+244h] [ebp-380h]
  float v69; // [esp+248h] [ebp-37Ch]
  hudelem_color_t v70; // [esp+24Ch] [ebp-378h]
  float v71; // [esp+250h] [ebp-374h]
  float v72; // [esp+254h] [ebp-370h]
  float v73; // [esp+258h] [ebp-36Ch]
  float *v74; // [esp+25Ch] [ebp-368h]
  float v75[2]; // [esp+260h] [ebp-364h] BYREF
  float v76; // [esp+268h] [ebp-35Ch] BYREF
  float v77; // [esp+26Ch] [ebp-358h]
  float v78; // [esp+270h] [ebp-354h]
  float fc; // [esp+274h] [ebp-350h]
  float v80; // [esp+278h] [ebp-34Ch]
  float v81; // [esp+27Ch] [ebp-348h]
  float f; // [esp+280h] [ebp-344h]
  float v83; // [esp+284h] [ebp-340h]
  hudelem_color_t v84; // [esp+288h] [ebp-33Ch]
  float v85; // [esp+28Ch] [ebp-338h]
  float v86; // [esp+290h] [ebp-334h]
  float v87; // [esp+294h] [ebp-330h]
  float v88; // [esp+298h] [ebp-32Ch]
  float v89; // [esp+29Ch] [ebp-328h]
  float v90; // [esp+2A0h] [ebp-324h]
  __int16 v91; // [esp+2A6h] [ebp-31Eh]
  float v92; // [esp+2A8h] [ebp-31Ch]
  float v93; // [esp+2ACh] [ebp-318h]
  float v94; // [esp+2B0h] [ebp-314h]
  float v95; // [esp+2B4h] [ebp-310h]
  hudelem_color_t v96; // [esp+2B8h] [ebp-30Ch]
  float v97; // [esp+2BCh] [ebp-308h]
  float v98; // [esp+2C0h] [ebp-304h]
  float v99; // [esp+2C4h] [ebp-300h]
  float v100; // [esp+2C8h] [ebp-2FCh]
  hudelem_color_t v101; // [esp+2CCh] [ebp-2F8h]
  float v102; // [esp+2D0h] [ebp-2F4h]
  float v103; // [esp+2D4h] [ebp-2F0h]
  float v104; // [esp+2D8h] [ebp-2ECh]
  float v105; // [esp+2DCh] [ebp-2E8h]
  hudelem_color_t v106; // [esp+2E0h] [ebp-2E4h]
  float v107; // [esp+2E4h] [ebp-2E0h]
  float v108; // [esp+2E8h] [ebp-2DCh]
  float v109; // [esp+2ECh] [ebp-2D8h]
  float v110; // [esp+2F0h] [ebp-2D4h]
  const vehicle_info_t *v111; // [esp+2F4h] [ebp-2D0h]
  __int16 vehicleInfoIndex; // [esp+2F8h] [ebp-2CCh]
  float v113; // [esp+2FCh] [ebp-2C8h]
  const vehicle_info_t *vehicleInfo; // [esp+300h] [ebp-2C4h]
  float infoIdx; // [esp+304h] [ebp-2C0h] OVERLAPPED
  hudelem_color_t fromColor; // [esp+308h] [ebp-2BCh]
  float v117; // [esp+30Ch] [ebp-2B8h]
  float v118; // [esp+310h] [ebp-2B4h]
  float v119; // [esp+314h] [ebp-2B0h]
  float v120; // [esp+318h] [ebp-2ACh]
  float v121; // [esp+31Ch] [ebp-2A8h]
  float v122; // [esp+320h] [ebp-2A4h]
  float v123; // [esp+324h] [ebp-2A0h]
  float v124; // [esp+328h] [ebp-29Ch]
  float v125; // [esp+32Ch] [ebp-298h]
  float v126; // [esp+330h] [ebp-294h]
  const LerpEntityState *v127; // [esp+334h] [ebp-290h]
  float v128; // [esp+338h] [ebp-28Ch] BYREF
  float newViewAngles[3]; // [esp+344h] [ebp-280h] BYREF
  float newViewAxis[3][3]; // [esp+350h] [ebp-274h] BYREF
  float yawAxis[3][3]; // [esp+374h] [ebp-250h] BYREF
  unsigned int yawAngles[5]; // [esp+398h] [ebp-22Ch] OVERLAPPED BYREF
  float vehAxis[3][3]; // [esp+3ACh] [ebp-218h] BYREF
  float vehYaw; // [esp+3D0h] [ebp-1F4h]
  float vehAngles[3]; // [esp+3D4h] [ebp-1F0h]
  float v136; // [esp+3E0h] [ebp-1E4h]
  float v137; // [esp+3E4h] [ebp-1E0h]
  float ofsPitch; // [esp+3E8h] [ebp-1DCh]
  hudelem_color_t localYaw; // [esp+3ECh] [ebp-1D8h]
  float v140; // [esp+3F0h] [ebp-1D4h]
  float absYaw; // [esp+3F4h] [ebp-1D0h]
  float v142; // [esp+3F8h] [ebp-1CCh]
  float v143; // [esp+3FCh] [ebp-1C8h]
  float v144; // [esp+400h] [ebp-1C4h]
  float v145; // [esp+404h] [ebp-1C0h]
  float v146; // [esp+408h] [ebp-1BCh]
  float v147; // [esp+40Ch] [ebp-1B8h]
  float v148[3]; // [esp+410h] [ebp-1B4h] BYREF
  float turretAnglesAxis[3][3]; // [esp+41Ch] [ebp-1A8h] BYREF
  __int128 turretAngles; // [esp+440h] [ebp-184h] OVERLAPPED BYREF
  float invMat[3][3]; // [esp+450h] [ebp-174h] BYREF
  float baseAnglesAxis[3][3]; // [esp+474h] [ebp-150h] BYREF
  float baseAngles[3]; // [esp+498h] [ebp-12Ch]
  float v154; // [esp+4A4h] [ebp-120h]
  float v155; // [esp+4A8h] [ebp-11Ch]
  float v156; // [esp+4ACh] [ebp-118h]
  float v157; // [esp+4B0h] [ebp-114h]
  float v158; // [esp+4B4h] [ebp-110h]
  float v159; // [esp+4B8h] [ebp-10Ch]
  float v160; // [esp+4BCh] [ebp-108h] BYREF
  float v161; // [esp+4C0h] [ebp-104h]
  float tagAngles[3]; // [esp+4C8h] [ebp-FCh] BYREF
  float tagAxis[4][3]; // [esp+4D4h] [ebp-F0h] BYREF
  DObjAnimMat animMat; // [esp+504h] [ebp-C0h] BYREF
  unsigned __int16 *p_tag_gunner_barrel1; // [esp+524h] [ebp-A0h]
  int gunnerIndex; // [esp+528h] [ebp-9Ch]
  DObj *obja; // [esp+52Ch] [ebp-98h]
  unsigned __int16 *gunnerViewTags[4]; // [esp+530h] [ebp-94h]
  colgeom_visitor_inlined_t<200> *p_proximity_data; // [esp+540h] [ebp-84h]
  colgeom_visitor_inlined_t<200>_vtbl *v170; // [esp+544h] [ebp-80h]
  int i; // [esp+548h] [ebp-7Ch]
  const vehicle_info_t *v172; // [esp+54Ch] [ebp-78h]
  float m_forward_vel; // [esp+550h] [ebp-74h]
  int numWheels; // [esp+554h] [ebp-70h]
  const vehicle_info_t *info; // [esp+558h] [ebp-6Ch] BYREF
  float speed; // [esp+55Ch] [ebp-68h]
  float v177; // [esp+560h] [ebp-64h]
  float forward[3]; // [esp+564h] [ebp-60h]
  const playerState_s *ps; // [esp+570h] [ebp-54h]
  const entityState_s *ns; // [esp+574h] [ebp-50h]
  const LerpEntityState *v181; // [esp+578h] [ebp-4Ch]
  cg_s *cgameGlob; // [esp+57Ch] [ebp-48h] BYREF
  cg_s *v183; // [esp+580h] [ebp-44h] OVERLAPPED
  int ignoreGunner; // [esp+584h] [ebp-40h]
  trace_t trace; // [esp+588h] [ebp-3Ch]
  int retaddr; // [esp+5C4h] [ebp+0h]

  trace.staticModel = a1;
  trace.hitPartition = retaddr;
  LODWORD(v7) = a2;
  cgameGlob = *(cg_s **)&FLOAT_0_0;
  v183 = *(cg_s **)&FLOAT_0_0;
  ignoreGunner = *(unsigned int *)&FLOAT_0_0;
  trace.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp", 339, 0, "%s", "obj") )
    __debugbreak();
  v181 = (const LerpEntityState *)-1;
  HIBYTE(ns) = 0;
  if ( !cent->vehicle )
  {
    cent->vehicle = (cgVehicle_s *)MT_Alloc(84, 21, SCRIPTINSTANCE_SERVER);
    if ( !cent->vehicle
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp", 347, 0, "%s", "cent->vehicle") )
    {
      __debugbreak();
    }
    memset((unsigned __int8 *)cent->vehicle, 0, sizeof(cgVehicle_s));
  }
  ps = (const playerState_s *)CG_GetLocalClientGlobals(localClientNum);
  LODWORD(forward[2]) = &cent->currentState;
  LODWORD(forward[1]) = &cent->nextState;
  LODWORD(forward[0]) = &ps[26].hud.current[28].duration;
  AngleVectors(cent->pose.angles, (float *)&info, 0, 0);
  numWheels = LODWORD(forward[1]) + 36;
  m_forward_vel = (float)((float)(*(float *)(LODWORD(forward[1]) + 36) * *(float *)&info)
                        + (float)(*(float *)(LODWORD(forward[1]) + 40) * speed))
                + (float)(*(float *)(LODWORD(forward[1]) + 44) * v177);
  if ( cent->nitrousVeh )
    m_forward_vel = cent->nitrousVeh->m_forward_vel;
  VEH_UpdateMaterialTime(cent, ps[26].hud.current[28].fadeStartTime, m_forward_vel);
  v172 = CG_GetVehicleInfo(cent->nextState.un2.vehicleState.vehicleInfoIndex);
  i = 0;
  if ( v172->type == 1 )
  {
    i = 2;
  }
  else if ( v172->type )
  {
    if ( v172->type == 2 )
      i = 6;
  }
  else
  {
    i = 4;
  }
  if ( !cent->vehicle->vehicle_cache && i > 0 )
  {
    cent->vehicle->vehicle_cache = (vehicle_cache_t *)MT_Alloc(1928, 21, SCRIPTINSTANCE_SERVER);
    if ( !cent->vehicle->vehicle_cache
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
            382,
            0,
            "%s",
            "cent->vehicle->vehicle_cache") )
    {
      __debugbreak();
    }
    if ( (_S1_2 & 1) == 0 )
    {
      _S1_2 |= 1u;
      colgeom_visitor_t::colgeom_visitor_t(&dummy);
      dummy.__vftable = (colgeom_visitor_inlined_t<200>_vtbl *)&colgeom_visitor_inlined_t<200>::`vftable';
      colgeom_visitor_inlined_t<500>::reset(&dummy);
      atexit(CG_Vehicle_PreControllers_::_19_::_dynamic_atexit_destructor_for__dummy__);
    }
    v170 = dummy.__vftable;
    p_proximity_data = &cent->vehicle->vehicle_cache->proximity_data;
    p_proximity_data->__vftable = dummy.__vftable;
    colgeom_visitor_inlined_t<500>::reset(&cent->vehicle->vehicle_cache->proximity_data);
    for ( gunnerViewTags[3] = 0; (int)gunnerViewTags[3] < 6; ++gunnerViewTags[3] )
    {
      cent->vehicle->vehicle_cache->hit_indices[(int)gunnerViewTags[3]] = -1;
      gunnerViewTags[2] = (unsigned __int16 *)cent->vehicle->vehicle_cache;
      *(float *)gunnerViewTags[2] = FLOAT_3_4028235e38;
      *((float *)gunnerViewTags[2] + 1) = FLOAT_3_4028235e38;
      *((float *)gunnerViewTags[2] + 2) = FLOAT_3_4028235e38;
      gunnerViewTags[1] = (unsigned __int16 *)cent->vehicle->vehicle_cache->lastAngles;
      *(float *)gunnerViewTags[1] = FLOAT_3_4028235e38;
      *((float *)gunnerViewTags[1] + 1) = FLOAT_3_4028235e38;
      *((float *)gunnerViewTags[1] + 2) = FLOAT_3_4028235e38;
    }
  }
  if ( (*(unsigned int *)(LODWORD(forward[0]) + 224) & 0x4000) != 0
    && *(__int16 *)(LODWORD(forward[0]) + 1088) == cent->nextState.number )
  {
    if ( *(int *)(LODWORD(forward[0]) + 1092) >= 1
      && *(int *)(LODWORD(forward[0]) + 1092) <= 4
      && gVehicleRelativeGunnerAngles )
    {
      p_tag_gunner_barrel1 = &scr_const.tag_gunner_barrel1;
      gunnerIndex = (int)&scr_const.tag_gunner_barrel2;
      obja = (DObj *)&scr_const.tag_gunner_barrel3;
      gunnerViewTags[0] = &scr_const.tag_gunner_barrel4;
      LODWORD(animMat.transWeight) = Com_GetClientDObj(cent->nextState.number, localClientNum);
      if ( LODWORD(animMat.transWeight) )
      {
        LODWORD(animMat.trans[2]) = *(unsigned int *)(LODWORD(forward[0]) + 1092) - 1;
        HIBYTE(animMat.trans[1]) = 0;
        DObjGetBoneIndex(
          (const DObj *)LODWORD(animMat.transWeight),
          *(&p_tag_gunner_barrel1)[LODWORD(animMat.trans[2])],
          (unsigned __int8 *)&animMat.trans[1] + 3,
          -1);
        DObjGetBasePoseMatrix(
          (const DObj *)LODWORD(animMat.transWeight),
          HIBYTE(animMat.trans[1]),
          (DObjAnimMat *)tagAxis[3]);
        QuatToAxis(tagAxis[3], (float (*)[3])tagAngles);
        AxisToAngles((const float (*)[3])tagAngles, &v160);
        v159 = *(float *)(LODWORD(forward[0]) + 384) + v160;
        v158 = floor(v159 * 182.04445 + 0.5);
        cent->currentState.u.vehicle.gunnerAngles[LODWORD(animMat.trans[2])].pitch = (int)v158;
        v157 = *(float *)(LODWORD(forward[0]) + 388) + v161;
        v156 = floor(v157 * 182.04445 + 0.5);
        cent->currentState.u.vehicle.gunnerAngles[LODWORD(animMat.trans[2])].yaw = (int)v156;
        v155 = *(float *)(LODWORD(forward[0]) + 384) + v160;
        v154 = floor(v155 * 182.04445 + 0.5);
        cent->nextState.lerp.u.vehicle.gunnerAngles[LODWORD(animMat.trans[2])].pitch = (int)v154;
        baseAngles[2] = *(float *)(LODWORD(forward[0]) + 388) + v161;
        baseAngles[1] = floor(baseAngles[2] * 182.04445 + 0.5);
        cent->nextState.lerp.u.vehicle.gunnerAngles[LODWORD(animMat.trans[2])].yaw = (int)baseAngles[1];
      }
    }
    else if ( !vehLockTurretToPlayerView->current.enabled || *(unsigned int *)(LODWORD(forward[0]) + 1092) )
    {
      v143 = (float)*(__int16 *)(LODWORD(forward[1]) + 112) * 0.0054931641;
      v142 = (float)*(__int16 *)(LODWORD(forward[2]) + 108) * 0.0054931641;
      absYaw = v142 + *(float *)(LODWORD(forward[2]) + 60);
      v140 = v143 + *(float *)(LODWORD(forward[1]) + 64);
      localYaw = ps[26].hud.current[28].fromColor;
      ofsPitch = AngleNormalize180(v140 - absYaw) * *(float *)&localYaw.rgba + absYaw;
      v137 = cent->pose.angles[1];
      v136 = AngleNormalize180(ofsPitch - v137);
      vehAngles[2] = *(float *)&FLOAT_0_0;
      vehAngles[1] = floor(v136 * 182.04445 + 0.5);
      cent->pose.vehicle.yaw = (int)vehAngles[1];
      if ( *(int *)(LODWORD(forward[0]) + 1092) >= 1 && *(int *)(LODWORD(forward[0]) + 1092) <= 4 )
      {
        LODWORD(vehAngles[0]) = cent->pose.angles;
        *(_QWORD *)&vehAxis[2][1] = *(_QWORD *)cent->pose.angles;
        vehYaw = cent->pose.angles[2];
        vehAxis[2][0] = vehAxis[2][2];
        vehAxis[2][2] = *(float *)&FLOAT_0_0;
        AnglesToAxis(&vehAxis[2][1], (float (*)[3])&yawAngles[2]);
        yawAngles[1] = *(unsigned int *)(LODWORD(forward[0]) + 388);
        *(float *)yawAngles = AngleNormalize180(*(float *)&yawAngles[1] - vehAxis[2][0]);
        *(_QWORD *)&yawAxis[2][0] = __PAIR64__(yawAngles[0], *(unsigned int *)&FLOAT_0_0);
        yawAxis[2][2] = *(float *)&FLOAT_0_0;
        AnglesToAxis(yawAxis[2], (float (*)[3])newViewAxis[2]);
        MatrixMultiply(
          (const float (*)[3])newViewAxis[2],
          (const float (*)[3])&yawAngles[2],
          (float (*)[3])newViewAngles);
        AxisToAngles((const float (*)[3])newViewAngles, &v128);
        vehAngles[2] = AngleNormalize180(v128);
        *(float *)&ps[46].hud.current[14].color.rgba = vehAngles[2];
        v127 = (const LerpEntityState *)(*(unsigned int *)(LODWORD(forward[0]) + 1092) - 1);
        v126 = *(float *)(LODWORD(forward[0]) + 384);
        v125 = AngleNormalize180(v126 - vehAngles[2]);
        v124 = floor(v125 * 182.04445 + 0.5);
        cent->pose.vehicle.gunnerPitch[(unsigned int)v127] = (int)v124;
        v123 = *(float *)(LODWORD(forward[0]) + 388);
        v122 = AngleNormalize180(v123 - ofsPitch);
        v121 = floor(v122 * 182.04445 + 0.5);
        cent->pose.vehicle.gunnerYaw[(unsigned int)v127] = (int)v121;
        v181 = v127;
      }
      DObjLock(obj);
      DObjSkelClear(obj);
      DObjUnlock(obj);
    }
    else
    {
      LODWORD(baseAngles[0]) = cent->pose.angles;
      *(_QWORD *)&baseAnglesAxis[2][0] = *(_QWORD *)cent->pose.angles;
      baseAnglesAxis[2][2] = cent->pose.angles[2];
      AnglesToAxis(baseAnglesAxis[2], (float (*)[3])invMat[2]);
      MatrixTranspose((const float (*)[3])invMat[2], (float (*)[3])((char *)&turretAngles + 4));
      LODWORD(turretAngles) = LODWORD(forward[0]) + 384;
      *(_QWORD *)&turretAnglesAxis[2][0] = *(_QWORD *)(LODWORD(forward[0]) + 384);
      turretAnglesAxis[2][2] = *(float *)(LODWORD(forward[0]) + 392);
      turretAnglesAxis[2][2] = cent->pose.angles[2];
      AnglesToAxis(turretAnglesAxis[2], (float (*)[3])v148);
      MatrixMultiply((const float (*)[3])v148, (const float (*)[3])((char *)&turretAngles + 4), (float (*)[3])invMat[2]);
      AxisToAngles((const float (*)[3])invMat[2], turretAnglesAxis[2]);
      v147 = turretAnglesAxis[2][1];
      v146 = floor(turretAnglesAxis[2][1] * 182.04445 + 0.5);
      cent->pose.vehicle.yaw = (int)v146;
      v145 = turretAnglesAxis[2][0];
      v144 = floor(turretAnglesAxis[2][0] * 182.04445 + 0.5);
      cent->pose.vehicle.barrelPitch = (int)v144;
      DObjLock(obj);
      DObjSkelClear(obj);
      DObjUnlock(obj);
      HIBYTE(ns) = 1;
    }
  }
  else
  {
    v120 = (float)*(__int16 *)(LODWORD(forward[1]) + 112) * 0.0054931641;
    v119 = (float)*(__int16 *)(LODWORD(forward[2]) + 108) * 0.0054931641;
    v118 = v119 + *(float *)(LODWORD(forward[2]) + 60);
    v117 = v120 + *(float *)(LODWORD(forward[1]) + 64);
    fromColor = ps[26].hud.current[28].fromColor;
    infoIdx = AngleNormalize180(v117 - v118) * *(float *)&fromColor.rgba + v118;
    *(float *)&vehicleInfo = infoIdx - cent->pose.angles[1];
    v113 = floor(*(float *)&vehicleInfo * 182.04445 + 0.5);
    cent->pose.vehicle.yaw = (int)v113;
  }
  if ( cent->nitrousVeh )
  {
    v90 = floor(0.0 * 182.04445 + 0.5);
    cent->pose.vehicle.pitch = (int)v90;
    v89 = floor(0.0 * 182.04445 + 0.5);
    cent->pose.vehicle.roll = (int)v89;
    v88 = floor(0.0 * 182.04445 + 0.5);
    cent->pose.vehicle.steerPitch = (int)v88;
    v87 = floor(0.0 * 182.04445 + 0.5);
    cent->pose.vehicle.steerYaw = (int)v87;
  }
  else
  {
    vehicleInfoIndex = cent->nextState.un2.vehicleState.vehicleInfoIndex;
    v111 = CG_GetVehicleInfo(vehicleInfoIndex);
    if ( v111->isNitrous )
    {
      v110 = floor(0.0 * 182.04445 + 0.5);
      cent->pose.vehicle.pitch = (int)v110;
      v109 = floor(0.0 * 182.04445 + 0.5);
      cent->pose.vehicle.roll = (int)v109;
    }
    else
    {
      v108 = (float)*(__int16 *)(LODWORD(forward[1]) + 108) * 0.0054931641;
      v107 = (float)*(__int16 *)(LODWORD(forward[2]) + 104) * 0.0054931641;
      v106 = ps[26].hud.current[28].fromColor;
      v105 = AngleNormalize180(v108 - v107) * *(float *)&v106.rgba + v107;
      v104 = floor(v105 * 182.04445 + 0.5);
      cent->pose.vehicle.pitch = (int)v104;
      v103 = *(float *)(LODWORD(forward[2]) + 84);
      v102 = *(float *)(LODWORD(forward[1]) + 88);
      v101 = ps[26].hud.current[28].fromColor;
      v100 = AngleNormalize180(v102 - v103) * *(float *)&v101.rgba + v103;
      v99 = floor(v100 * 182.04445 + 0.5);
      cent->pose.vehicle.roll = (int)v99;
    }
    v98 = *(float *)(LODWORD(forward[2]) + 80);
    v97 = *(float *)(LODWORD(forward[1]) + 84);
    v96 = ps[26].hud.current[28].fromColor;
    v95 = AngleNormalize180(v97 - v98) * *(float *)&v96.rgba + v98;
    v94 = floor(v95 * 182.04445 + 0.5);
    cent->pose.vehicle.steerYaw = (int)v94;
    if ( v172->wheelRotRate == 0.0 )
    {
      cent->pose.vehicle.steerPitch = 0;
    }
    else
    {
      v93 = (float)((float)((float)ps[26].hud.current[28].fadeStartTime * 0.001) * v172->wheelRotRate) * m_forward_vel;
      v92 = floor(v93 * 182.04445 + 0.5);
      v91 = (int)v92;
      cent->pose.vehicle.steerPitch += v91;
    }
  }
  if ( !HIBYTE(ns) )
  {
    v86 = (float)*(__int16 *)(LODWORD(forward[1]) + 110) * 0.0054931641;
    v85 = (float)*(__int16 *)(LODWORD(forward[2]) + 106) * 0.0054931641;
    v84 = ps[26].hud.current[28].fromColor;
    v83 = AngleNormalize180(v86 - v85) * *(float *)&v84.rgba + v85;
    f = floor(v83 * 182.04445 + 0.5);
    cent->pose.vehicle.barrelPitch = (int)f;
  }
  if ( cent->vehicle->fireTime && cent->vehicle->fireTime > ps[26].hud.current[28].scaleStartTime - recoilDuration )
  {
    if ( cent->vehicle->fireTime <= ps[26].hud.current[28].scaleStartTime - recoilBackTime )
    {
      fc = 1.0
         - (float)((float)(ps[26].hud.current[28].scaleStartTime - cent->vehicle->fireTime - recoilBackTime)
                 / (float)(recoilDuration - recoilBackTime));
    }
    else
    {
      v81 = (float)(ps[26].hud.current[28].scaleStartTime - cent->vehicle->fireTime) / (float)recoilBackTime;
      v80 = recoilBackPow;
      __libm_sse2_pow(v7, v8);
      fc = v81;
    }
    if ( (float)(fc - 1.0) < 0.0 )
      v78 = fc;
    else
      v78 = FLOAT_1_0;
    if ( (float)(0.0 - fc) < 0.0 )
      v77 = v78;
    else
      v77 = *(float *)&FLOAT_0_0;
    LODWORD(v75[1]) = &v76;
    v74 = v75;
    v73 = (float)(v77 * 3.1415927) - 1.5707964;
    v76 = cos(v73);
    v75[0] = sin(v73);
    fc = (float)(v75[0] + 1.0) / 2.0;
    cent->pose.vehicle.barrelRecoil = fc * recoilDist;
  }
  else
  {
    cent->pose.vehicle.barrelRecoil = *(float *)&FLOAT_0_0;
  }
  for ( gunnerViewTags[3] = 0; (int)gunnerViewTags[3] < 4; ++gunnerViewTags[3] )
  {
    if ( v181 != (const LerpEntityState *)gunnerViewTags[3] )
    {
      v72 = (float)*(__int16 *)(LODWORD(forward[1]) + 4 * (int)gunnerViewTags[3] + 92) * 0.0054931641;
      v71 = (float)*(__int16 *)(LODWORD(forward[2]) + 4 * (int)gunnerViewTags[3] + 88) * 0.0054931641;
      v70 = ps[26].hud.current[28].fromColor;
      v69 = AngleNormalize180(v72 - v71) * *(float *)&v70.rgba + v71;
      v68 = floor(v69 * 182.04445 + 0.5);
      cent->pose.vehicle.gunnerPitch[(int)gunnerViewTags[3]] = (int)v68;
      v67 = (float)*(__int16 *)(LODWORD(forward[1]) + 4 * (int)gunnerViewTags[3] + 94) * 0.0054931641;
      v66 = (float)*(__int16 *)(LODWORD(forward[2]) + 4 * (int)gunnerViewTags[3] + 90) * 0.0054931641;
      clientNum = ps[26].hud.current[28].fromColor.rgba;
      *(float *)&ci = AngleNormalize180(v67 - v66) * *(float *)&clientNum + v66;
      v63 = floor(*(float *)&ci * 182.04445 + 0.5);
      cent->pose.vehicle.gunnerYaw[(int)gunnerViewTags[3]] = (int)v63;
    }
  }
  DObjGetBoneIndex(obj, scr_const.tag_body, &cent->pose.vehicle.tag_body, -1);
  DObjGetBoneIndex(obj, scr_const.tag_turret, &cent->pose.vehicle.tag_turret, -1);
  DObjGetBoneIndex(obj, scr_const.tag_barrel, &cent->pose.vehicle.tag_barrel, -1);

  if ( v172->type != 6 )
    DObjGetBoneIndex(obj, scr_const.tag_barrel_recoil, &cent->pose.vehicle.tag_barrel_recoil, -1);
  cent->pose.vehicle.tag_minigun_spin = -2;
  cent->pose.vehicle.minigun_rotation = 0;
  for ( gunnerViewTags[3] = 0; (int)gunnerViewTags[3] < 4; ++gunnerViewTags[3] )
  {
    DObjGetBoneIndex(
      obj,
      *g_gunnerBarrelTags[(int)gunnerViewTags[3]],
      &cent->pose.vehicle.tag_gunner_barrel[(unsigned int)gunnerViewTags[3]],
      -1);
    DObjGetBoneIndex(
      obj,
      *g_gunnerTurretTags[(int)gunnerViewTags[3]],
      &cent->pose.vehicle.tag_gunner_turret[(unsigned int)gunnerViewTags[3]],
      -1);
  }
  for ( turretWeapon = 0; (int)turretWeapon < 32; turretWeapon = (const WeaponDef *)((char *)turretWeapon + 1) )
  {
    v61 = &ps[39].hud.current[3].fadeStartTime + 370 * (unsigned int)turretWeapon;
    if ( *v61 )
    {
      if ( v61[343] == cent->nextState.number && v61[344] >= 1 && v61[344] <= 4 )
      {
        v60 = v61[344] - 1;
        suspTravel = BG_GetWeaponDef(v172->gunnerWeaponIndex[v60]);
        if ( suspTravel->fireType == WEAPON_FIRETYPE_MINIGUN )
        {
          DObjGetBoneIndex(obj, scr_const.tag_minigun_right_barrel, &cent->pose.vehicle.tag_minigun_spin, -1);
          LODWORD(axis[3][2]) = v61[365];
          axis[3][1] = floor(axis[3][2] * 182.04445 + 0.5);
          cent->pose.vehicle.minigun_rotation = (int)axis[3][1];
          break;
        }
      }
    }
  }
  cent->pose.vehicle.extra_wheel_rot_scale = v172->extraWheelRotScale;
  for ( gunnerViewTags[3] = 0; (int)gunnerViewTags[3] < 4; ++gunnerViewTags[3] )
  {
    if ( v172->extraWheelTags[(int)gunnerViewTags[3]] )
      DObjGetBoneIndex(
        obj,
        v172->extraWheelTags[(int)gunnerViewTags[3]],
        &cent->pose.vehicle.tag_extra_tank_wheels[(unsigned int)gunnerViewTags[3]],
        -1);
    else
      *((_BYTE *)gunnerViewTags[3] + (unsigned int)cent + 236) = -2;
  }
  if ( cent->pose.cullIn == 2 )
  {
    axis[3][0] = cent->pose.vehicle.time;
    AnglesToAxis(cent->pose.angles, (float (*)[3])boneMtxList);
    v56 = axis[2];
    wheel = (rigid_body_constraint_wheel *)cent->pose.origin;
    axis[2][0] = cent->pose.origin[0];
    axis[2][1] = cent->pose.origin[1];
    axis[2][2] = cent->pose.origin[2];
    Model = DObjGetModel(obj, 0);
    BasePose = XModelGetBasePose(Model);
    if ( *g_wheelTags[0] != scr_const.tag_wheel_front_left
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
            667,
            0,
            "%s",
            "*g_wheelTags[TAG_WHEEL_FRONT_LEFT] == scr_const.tag_wheel_front_left") )
    {
      __debugbreak();
    }
    if ( *g_wheelTags[1] != scr_const.tag_wheel_front_right
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
            668,
            0,
            "%s",
            "*g_wheelTags[TAG_WHEEL_FRONT_RIGHT] == scr_const.tag_wheel_front_right") )
    {
      __debugbreak();
    }
    if ( cent->nitrousVeh )
    {
      for ( j = 0; j < 6; ++j )
      {
        cent->vehicle->wheelSurfType[j] = cent->nitrousVeh->m_wheel_surf_types[j];
        wheelRadiusDelta = cent->nitrousVeh->m_wheels[j];
        if ( wheelRadiusDelta )
        {
          m_parameter = cent->nitrousVeh->m_parameter;
          m_wheel_radius = wheelRadiusDelta->m_wheel_radius;
          newWheelHeight = m_parameter->m_wheel_radius - m_wheel_radius;
          wheel2 = (rigid_body_constraint_wheel *)LODWORD(wheelRadiusDelta->m_wheel_displaced_center_dist);
          v47 = cent->nitrousVeh->m_parameter;
          v46 = (float)(*(float *)&wheel2 - v47->m_susp_adj) + newWheelHeight;
          if ( v172->type == 1 )
          {
            if ( j )
            {
              if ( j == 2 )
              {
                v41 = cent->nitrousVeh->m_wheels[3];
                lerpRate = wheelRadiusDelta->m_wheel_displaced_center_dist;
                m_wheel_displaced_center_dist = v41->m_wheel_displaced_center_dist;
                v38 = cent->nitrousVeh->m_parameter;
                v46 = (float)((float)((float)(lerpRate + m_wheel_displaced_center_dist) * 0.5) - v38->m_susp_adj)
                    + newWheelHeight;
                DObjGetBoneIndex(obj, scr_const.tag_wheel_back, &cent->pose.vehicle.wheelBoneIndex[2], -1);
              }
            }
            else
            {
              v45 = cent->nitrousVeh->m_wheels[1];
              v44 = wheelRadiusDelta->m_wheel_displaced_center_dist;
              v43 = v45->m_wheel_displaced_center_dist;
              v42 = cent->nitrousVeh->m_parameter;
              v46 = (float)((float)((float)(v44 + v43) * 0.5) - v42->m_susp_adj) + newWheelHeight;
              DObjGetBoneIndex(obj, scr_const.tag_wheel_front, cent->pose.vehicle.wheelBoneIndex, -1);
            }
          }
          else
          {
            DObjGetBoneIndex(obj, *g_wheelTags[j], &cent->pose.vehicle.wheelBoneIndex[j], -1);
          }
          if ( cent->nitrousVeh->m_owner )
          {
            cent->pose.vehicle.wheelHeight[j] = v46;
          }
          else
          {
            v37 = FLOAT_30_0;
            lerp = (float)((float)ps[26].hud.current[28].fadeStartTime * 0.001) * 30.0;
            if ( (float)(lerp - 1.0) < 0.0 )
              v35 = lerp;
            else
              v35 = FLOAT_1_0;
            if ( (float)(0.0 - lerp) < 0.0 )
              v34 = v35;
            else
              v34 = *(float *)&FLOAT_0_0;
            v33 = v34;
            cent->pose.vehicle.wheelHeight[j] = (float)((float)(v46 - cent->pose.vehicle.wheelHeight[j]) * v34)
                                              + cent->pose.vehicle.wheelHeight[j];
          }
          rotation = v172->nitrousVehParams.m_susp_min_height;
          v31 = cent->pose.vehicle.wheelHeight[j];
          if ( (float)(rotation - v31) < 0.0 )
            v6 = v31;
          else
            v6 = rotation;
          v30 = v6;
          cent->pose.vehicle.wheelHeight[j] = v6;
          m_wheel_pos = wheelRadiusDelta->m_wheel_pos;
          cent->pose.vehicle.nitrousWheelRotation[j] = m_wheel_pos * 57.295776;
          if ( cent->nitrousVeh->m_vehicle_info->type == 2 || (unsigned int)j > 1 )
          {
            cent->pose.vehicle.nitrousWheelYaw[j] = *(float *)&FLOAT_0_0;
          }
          else
          {
            axis_loc_vec3[0] = wheelRadiusDelta->m_b1_wheel_axis_loc.x;
            axis_loc_vec3[1] = wheelRadiusDelta->m_b1_wheel_axis_loc.y;
            axis_loc_vec3[2] = wheelRadiusDelta->m_b1_wheel_axis_loc.z;
            axis_loc.x = wheelRadiusDelta->m_b1_wheel_axis_loc.w;
            Phys_NitrousVecToVec3((const phys_vec3 *)axis_loc_vec3, angles_vec3);
            vectoangles(angles_vec3, &mn[2]);
            cent->pose.vehicle.nitrousWheelYaw[j] = *(float *)&bitmask - 90.0;
          }
        }
      }
      if ( v172->type == 1 )
      {
        if ( (_S1_2 & 2) == 0 )
        {
          _S1_2 |= 2u;
          dword_F5C5D8 = SL_GetLowercaseString("tag_steering_wheel", 0, SCRIPTINSTANCE_SERVER);
        }
        if ( !DObjGetBoneIndex(obj, dword_F5C5D8, &cent->pose.vehicle.wheelBoneIndex[1], -1)
          || SLOWORD(ps[26].hud.archival[7].color.rgba) == cent->nextState.number )
        {
          cent->pose.vehicle.wheelBoneIndex[1] = -2;
        }
        else
        {
          cent->pose.vehicle.nitrousWheelYaw[1] = cent->pose.vehicle.nitrousWheelYaw[0];
          cent->pose.vehicle.nitrousWheelRotation[1] = *(float *)&FLOAT_0_0;
          cent->pose.vehicle.wheelHeight[1] = *(float *)&FLOAT_0_0;
        }
      }
      if ( cent->nitrousVeh->m_vehicle_info->type == 2 )
        R_FoliageNotifyVehiclePosition(localClientNum, cent->pose.origin);
    }
    else
    {
      mn[1] = *(float *)&cent->vehicle->vehicle_cache;
      if ( LODWORD(mn[1]) )
      {
        if ( VecNCompareCustomEpsilon((const float *)LODWORD(mn[1]), cent->pose.origin, 0.001, 3)
          && VecNCompareCustomEpsilon((const float *)(LODWORD(mn[1]) + 12), cent->pose.angles, 0.001, 3) )
        {
          LODWORD(mn[0]) = 1;
          gunnerViewTags[3] = 0;
          while ( (int)gunnerViewTags[3] < i )
          {
            if ( (LODWORD(mn[0]) & *(unsigned int *)(LODWORD(mn[1]) + 1924)) != 0 )
            {
              cent->pose.vehicle.wheelHeight[(int)gunnerViewTags[3]] = *(float *)(LODWORD(mn[1])
                                                                                + 4 * (int)gunnerViewTags[3]
                                                                                + 144);
              cent->vehicle->wheelSurfType[(int)gunnerViewTags[3]] = (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20]
                                                                                           & *(unsigned int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 48)) >> 20);
            }
            ++gunnerViewTags[3];
            LODWORD(mn[0]) *= 2;
          }
        }
        else
        {
          mx[0] = FLOAT_3_4028235e38;
          mx[1] = FLOAT_3_4028235e38;
          mx[2] = FLOAT_3_4028235e38;
          mtx = (const DObjAnimMat *)LODWORD(FLOAT_N3_4028235e38);
          boneIndex = LODWORD(FLOAT_N3_4028235e38);
          presence_mask = LODWORD(FLOAT_N3_4028235e38);
          wheelPos[2] = 0.0;
          LODWORD(mn[0]) = 1;
          gunnerViewTags[3] = 0;
          while ( (int)gunnerViewTags[3] < i )
          {
            if ( DObjGetBoneIndex(
                   obj,
                   *g_wheelTags[(int)gunnerViewTags[3]],
                   &cent->pose.vehicle.wheelBoneIndex[(unsigned int)gunnerViewTags[3]],
                   -1) )
            {
              LODWORD(wheelPos[2]) |= LODWORD(mn[0]);
              LODWORD(wheelPos[1]) = *((unsigned __int8 *)gunnerViewTags[3] + (unsigned int)cent + 144);
              LODWORD(wheelPos[0]) = BasePose[LODWORD(wheelPos[1])].quat;
              MatrixTransformVector43(
                (const float *)(LODWORD(wheelPos[0]) + 16),
                (const float (*)[3])boneMtxList,
                trace_points[5][1]);
              v16 = (float *)&v17[24 * (int)gunnerViewTags[3]];
              v15 = axis[1];
              *v16 = (float)(40.0 * axis[1][0]) + trace_points[5][1][0];
              v16[1] = (float)(40.0 * v15[1]) + trace_points[5][1][1];
              v16[2] = (float)(40.0 * v15[2]) + trace_points[5][1][2];
              contents_mask = (int)trace_points[(int)gunnerViewTags[3]];
              LODWORD(expand_vec[2]) = axis[1];
              LODWORD(expand_vec[1]) = LODWORD(axis[3][0]) ^ _mask__NegFloat_;
              *(float *)contents_mask = (float)(COERCE_FLOAT(LODWORD(axis[3][0]) ^ _mask__NegFloat_) * axis[1][0])
                                      + trace_points[5][1][0];
              *(float *)(contents_mask + 4) = (float)(expand_vec[1] * *(float *)(LODWORD(expand_vec[2]) + 4))
                                            + trace_points[5][1][1];
              *(float *)(contents_mask + 8) = (float)(expand_vec[1] * *(float *)(LODWORD(expand_vec[2]) + 8))
                                            + trace_points[5][1][2];
              Vec3Min((const float *)&v17[24 * (int)gunnerViewTags[3]], mx, mx);
              Vec3Min(trace_points[(int)gunnerViewTags[3]][0], mx, mx);
              Vec3Max((const float *)&v17[24 * (int)gunnerViewTags[3]], (const float *)&mtx, (float *)&mtx);
              Vec3Max(trace_points[(int)gunnerViewTags[3]][0], (const float *)&mtx, (float *)&mtx);
            }
            ++gunnerViewTags[3];
            LODWORD(mn[0]) *= 2;
          }
          LODWORD(expand_vec[0]) = 529;
          *(float *)&context.locational = FLOAT_30_0;
          *(float *)&context.priorityMap = FLOAT_30_0;
          *(float *)&context.collide_entity_func = FLOAT_30_0;
          (*(void (__thiscall **)(int, float *, const DObjAnimMat **, int, int *))(*(unsigned int *)(LODWORD(mn[1]) + 204) + 20))(
            LODWORD(mn[1]) + 204,
            mx,
            &mtx,
            529,
            &context.locational);
          col_context_t::col_context_t((col_context_t *)v11);
          v11[1] = LODWORD(mn[1]) + 324;
          v11[2] = *(unsigned int *)(LODWORD(mn[1]) + 316);
          LODWORD(mn[0]) = 1;
          gunnerViewTags[3] = 0;
          while ( (int)gunnerViewTags[3] < i )
          {
            if ( (LODWORD(mn[0]) & LODWORD(wheelPos[2])) != 0 )
            {
              if ( *(int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 24) > 0 )
              {
                cgameGlob = *(cg_s **)(LODWORD(mn[1]) + 12 * (int)gunnerViewTags[3] + 72);
                v183 = *(cg_s **)(LODWORD(mn[1]) + 12 * (int)gunnerViewTags[3] + 76);
                ignoreGunner = *(unsigned int *)(LODWORD(mn[1]) + 12 * (int)gunnerViewTags[3] + 80);
                trace.normal.vec.u[2] = *(unsigned int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 48);
              }
              trace_point_vs_env(
                (trace_t *)&cgameGlob,
                (const float *)&v17[24 * (int)gunnerViewTags[3]],
                trace_points[(int)gunnerViewTags[3]][0],
                (col_context_t *)v11,
                (int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 24));
              if ( *(int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 24) <= 0 )
              {
                *(float *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 144) = FLOAT_1_0;
                *(unsigned int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 48) = 0;
              }
              else
              {
                *(unsigned int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 144) = trace.normal.vec.u[1];
                v10 = (cg_s **)(LODWORD(mn[1]) + 12 * (int)gunnerViewTags[3] + 72);
                *v10 = cgameGlob;
                v10[1] = v183;
                v10[2] = (cg_s *)ignoreGunner;
                *(unsigned int *)(LODWORD(mn[1]) + 4 * (int)gunnerViewTags[3] + 48) = trace.normal.vec.u[2];
                *(float *)LODWORD(mn[1]) = cent->pose.origin[0];
                *(float *)(LODWORD(mn[1]) + 4) = cent->pose.origin[1];
                *(float *)(LODWORD(mn[1]) + 8) = cent->pose.origin[2];
                v9 = LODWORD(mn[1]) + 12;
                *(float *)(LODWORD(mn[1]) + 12) = cent->pose.angles[0];
                *(float *)(v9 + 4) = cent->pose.angles[1];
                *(float *)(v9 + 8) = cent->pose.angles[2];
                *(float *)(LODWORD(mn[1]) + 1924) = wheelPos[2];
              }
              LODWORD(cent->pose.vehicle.wheelHeight[(int)gunnerViewTags[3]]) = trace.normal.vec.u[1];
              cent->vehicle->wheelSurfType[(int)gunnerViewTags[3]] = (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20]
                                                                                           & trace.normal.vec.u[2]) >> 20);
            }
            ++gunnerViewTags[3];
            LODWORD(mn[0]) *= 2;
          }
        }
      }
    }
  }
  else
  {
    for ( gunnerViewTags[3] = 0; (int)gunnerViewTags[3] < 6; ++gunnerViewTags[3] )
      *((_BYTE *)gunnerViewTags[3] + (unsigned int)cent + 144) = -2;
  }
}

void  VEH_UpdateMaterialTime(float a1@<ebp>, centity_s *cent, int dt, float speed)
{
  int v4; // [esp-4h] [ebp-2Ch] BYREF
  float v5; // [esp+10h] [ebp-18h]
  float v6; // [esp+14h] [ebp-14h]
  const vehicle_info_t *VehicleInfo; // [esp+18h] [ebp-10h]
  float deltaTime; // [esp+1Ch] [ebp-Ch]
  float scrollMultiplier; // [esp+20h] [ebp-8h]
  float retaddr; // [esp+28h] [ebp+0h]

  deltaTime = a1;
  scrollMultiplier = retaddr;
  VehicleInfo = CG_GetVehicleInfo(cent->nextState.un2.vehicleState.vehicleInfoIndex);
  if ( VehicleInfo->texScroll )
  {
    v6 = FLOAT_1_0;
    v5 = (float)((float)dt * (float)((float)(speed * 1.0) / 176.0)) * VehicleInfo->texScrollScale;
    cent->vehicle->materialTime = (float)(v5 * 0.001) + cent->vehicle->materialTime;
    cent->vehicle->materialTime2 = (float)(v5 * 0.001) + cent->vehicle->materialTime2;
    if ( cent->destructible )
    {
      if ( cent->nitrousVeh )
      {
        v5 = (float)dt * (float)(NitrousVehicle::get_a_velocity(cent->nitrousVeh, (phys_vec3 *)&v4)->z / 176.0);
        cent->vehicle->materialTime = (float)((float)(v5 * 0.001) * aScrollSpeed) + cent->vehicle->materialTime;
        cent->vehicle->materialTime2 = cent->vehicle->materialTime2 - (float)((float)(v5 * 0.001) * aScrollSpeed);
      }
    }
  }
  else
  {
    cent->vehicle->materialTime = *(float *)&FLOAT_0_0;
    cent->vehicle->materialTime2 = *(float *)&FLOAT_0_0;
  }
}

int __cdecl GetSoundInfoForVehicle(int localClientNum, const centity_s *cent, float *slideRate)
{
  int v4; // [esp+0h] [ebp-28h]
  int surface_group_index; // [esp+4h] [ebp-24h]
  int wheelIndex; // [esp+8h] [ebp-20h]
  unsigned int surface_type_index; // [esp+Ch] [ebp-1Ch]
  rigid_body_constraint_wheel *wheel; // [esp+10h] [ebp-18h]
  int iWheel; // [esp+14h] [ebp-14h]
  int iSide; // [esp+18h] [ebp-10h]
  int hardness; // [esp+1Ch] [ebp-Ch]
  int surfType; // [esp+24h] [ebp-4h]

  *slideRate = *(float *)&FLOAT_0_0;
  surfType = -1;
  if ( cent->nitrousVeh )
  {
    hardness = -1;
    for ( iSide = 0; iSide < 2; ++iSide )
    {
      for ( iWheel = 0; iWheel < 2; ++iWheel )
      {
        wheelIndex = aaWheels[iSide][iWheel];
        surface_type_index = cent->nitrousVeh->m_wheel_surf_types[wheelIndex];
        if ( cent->nitrousVeh->m_wheel_state[wheelIndex].m_state == WHEEL_STATE_SKIDDING
          && iWheel == 1
          && cent->nitrousVeh->m_wheel_state[wheelIndex].m_rate > *slideRate )
        {
          *slideRate = cent->nitrousVeh->m_wheel_state[wheelIndex].m_rate;
        }
        wheel = cent->nitrousVeh->m_wheels[wheelIndex];
        if ( wheel && (wheel->m_wheel_flags & 1) != 0 )
        {
          if ( surface_type_index > 0x1E
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                  923,
                  0,
                  "%s",
                  "surface_type_index >= 0 && surface_type_index < ( sizeof( s_kaiSurfaceGroups ) / sizeof( s_kaiSurfaceGroups[ 0 ] ) )") )
          {
            __debugbreak();
          }
          surface_group_index = s_kaiSurfaceGroups[surface_type_index];
          if ( (unsigned int)surface_group_index >= 2
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                  926,
                  0,
                  "%s",
                  "surface_group_index >= 0 && surface_group_index < NUM_SURF_GROUPS") )
          {
            __debugbreak();
          }
          if ( hardness < surface_group_index )
            v4 = surface_group_index;
          else
            v4 = hardness;
          hardness = v4;
          if ( v4 == surface_group_index )
            surfType = surface_type_index;
        }
      }
    }
  }
  return surfType;
}

void __cdecl GetSoundOriginForVehicle(const centity_s *cent, float *origin)
{
  float v2; // [esp+4h] [ebp-8h]
  float v3; // [esp+8h] [ebp-4h]

  v2 = cent->pose.origin[1];
  v3 = cent->pose.origin[2] + 5.0;
  *origin = cent->pose.origin[0];
  origin[1] = v2;
  origin[2] = v3;
}

void __cdecl CG_CrossFadeVehicleMaterialEffects(
        int localClientNum,
        const centity_s *cent,
        const vehicle_info_t *info,
        unsigned int groundEffectIndex,
        int surfType,
        float ratio,
        int fadeTime)
{
  const char *v7; // eax
  char *v8; // eax
  unsigned int AliasId; // eax
  const char *v10; // eax
  char *v11; // eax
  const char *v12; // eax
  char *v13; // eax
  bool transition; // [esp+Bh] [ebp-19h]
  float soundOrigin[3]; // [esp+Ch] [ebp-18h] BYREF
  bool still_fading_out; // [esp+1Bh] [ebp-9h]
  const char *soundAlias; // [esp+1Ch] [ebp-8h]
  cgVehicleWheelEffect *wheelEffect; // [esp+20h] [ebp-4h]

  if ( !cent->vehicle
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp", 954, 0, "%s", "cent->vehicle") )
  {
    __debugbreak();
  }
  if ( groundEffectIndex >= 3
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
          955,
          0,
          "groundEffectIndex doesn't index NUM_VEHICLE_MATERIAL_SNDS\n\t%i not in [0, %i)",
          groundEffectIndex,
          3) )
  {
    __debugbreak();
  }
  wheelEffect = &cent->vehicle->wheelEffects[groundEffectIndex];
  if ( ratio < 0.0000152879 )
    ratio = *(float *)&FLOAT_0_0;
  soundAlias = info->sndMaterialNames[groundEffectIndex];
  if ( *soundAlias )
  {
    still_fading_out = 1;
    GetSoundOriginForVehicle(cent, soundOrigin);
    if ( wheelEffect->lastSurfaceType == -1 || wheelEffect->surfaceType == wheelEffect->lastSurfaceType )
    {
      still_fading_out = 0;
    }
    else
    {
      v7 = Com_SurfaceTypeToName(wheelEffect->lastSurfaceType);
      v8 = va("%s_%s", soundAlias, v7);
      AliasId = SND_FindAliasId(v8);
      CG_PlaySoundWithHandle(localClientNum, cent->nextState.number, soundOrigin, -fadeTime, 0, 1.0, AliasId);
      still_fading_out = 0;
    }
    transition = 0;
    if ( wheelEffect->surfaceType != surfType )
    {
      transition = 1;
      wheelEffect->lastSurfaceType = wheelEffect->surfaceType;
      wheelEffect->surfaceType = surfType;
    }
    if ( wheelEffect->surfaceType != -1 )
    {
      v10 = Com_SurfaceTypeToName(wheelEffect->surfaceType);
      v11 = va("%s_%s", soundAlias, v10);
      CG_PlaySound(localClientNum, cent->nextState.number, soundOrigin, transition ? fadeTime : 0, 0, ratio, v11);
    }
    if ( transition && wheelEffect->lastSurfaceType != -1 )
    {
      v12 = Com_SurfaceTypeToName(wheelEffect->lastSurfaceType);
      v13 = va("%s_%s", soundAlias, v12);
      CG_PlaySound(localClientNum, cent->nextState.number, soundOrigin, -fadeTime, 0, 1.0, v13);
    }
  }
}

void __cdecl CG_UpdateVehicleNotifies(int localClientNum, centity_s *cent)
{
  char *value; // eax
  int notifyFlags; // [esp+8h] [ebp-4h]

  if ( cent->nitrousVeh )
  {
    notifyFlags = cent->nitrousVeh->m_notify_flags;
    if ( notifyFlags )
    {
      if ( (notifyFlags & 1) != 0 )
      {
        if ( cent->nitrousVeh->m_collision_entnum == 1022 )
          Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
        else
          Scr_AddEntityNum(cent->nitrousVeh->m_collision_entnum, 0, SCRIPTINSTANCE_CLIENT, localClientNum);
        value = (char *)Com_SurfaceTypeToName(cent->nitrousVeh->m_collision_stype);
        Scr_AddString(value, SCRIPTINSTANCE_CLIENT);
        Scr_AddFloat(cent->nitrousVeh->m_collision_intensity, SCRIPTINSTANCE_CLIENT);
        Scr_AddVector(cent->nitrousVeh->m_collision_hitn, SCRIPTINSTANCE_CLIENT);
        Scr_AddVector(cent->nitrousVeh->m_collision_hitp, SCRIPTINSTANCE_CLIENT);
        CScr_Notify(localClientNum, cent, cscr_const.veh_collision, 5u);
      }
      if ( (notifyFlags & 2) != 0 )
        CScr_Notify(localClientNum, cent, cscr_const.veh_inair, 0);
      if ( (notifyFlags & 4) != 0 )
        CScr_Notify(localClientNum, cent, cscr_const.veh_landed, 0);
      if ( (notifyFlags & 8) != 0 )
        CScr_Notify(localClientNum, cent, cscr_const.veh_suspension_limit_activated, 0);
      if ( (notifyFlags & 0x10) != 0 )
        CScr_Notify(localClientNum, cent, cscr_const.veh_engine_stutter, 0);
    }
  }
}

void __cdecl CG_UpdateVehicleSounds(int localClientNum, centity_s *cent)
{
  unsigned __int16 i; // [esp+Ch] [ebp-14h]
  float speedIPS; // [esp+10h] [ebp-10h]
  const vehicle_info_t *info; // [esp+14h] [ebp-Ch]
  const cg_s *cgameGlob; // [esp+18h] [ebp-8h]
  bool isLocalPlayersVehicle; // [esp+1Eh] [ebp-2h]
  bool vehicleOccupied; // [esp+1Fh] [ebp-1h]
  int savedregs; // [esp+20h] [ebp+0h] BYREF

  info = CG_GetVehicleInfo(cent->nextState.un2.vehicleState.vehicleInfoIndex);
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  speedIPS = Abs(cent->currentState.pos.trDelta);
  if ( cent->nitrousVeh )
    LODWORD(speedIPS) = LODWORD(cent->nitrousVeh->m_forward_vel) & _mask__AbsFloat_;
  isLocalPlayersVehicle = cgameGlob->bgs.clientinfo[cgameGlob->clientNum].attachedVehEntNum == cent->nextState.number;
  vehicleOccupied = 0;
  for ( i = 0; !vehicleOccupied && i < com_maxclients->current.integer; ++i )
    vehicleOccupied = cgameGlob->bgs.clientinfo[i].attachedVehEntNum == cent->nextState.number;
  if ( cent->nitrousVeh && cent->vehicle && (speedIPS != 0.0 || vehicleOccupied) )
    CG_UpdateSurfaceSounds(localClientNum, cent, info);
  if ( vehicleOccupied )
    CG_UpdateVehicleEngineSounds((int)&savedregs, localClientNum, cent, info, isLocalPlayersVehicle);
  CG_UpdateBoostSounds(localClientNum, cent, info, isLocalPlayersVehicle);
  CG_UpdateTurretAndOverheatSounds(localClientNum, cent, info, isLocalPlayersVehicle);
  CG_UpdateFireSounds(localClientNum, cent, info, isLocalPlayersVehicle);
}

void __cdecl CG_UpdateSurfaceSounds(int localClientNum, centity_s *cent, const vehicle_info_t *info)
{
  bool v3; // [esp+0h] [ebp-4h]
  bool v4; // [esp+1h] [ebp-3h]
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  v4 = info && info->type == 2;
  v3 = info && (!info->type || info->type == 1);
  if ( !cent->nitrousVeh
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp", 1210, 0, "%s", "cent->nitrousVeh") )
  {
    __debugbreak();
  }
  if ( v4 )
  {
    CG_UpdateTankSurfaceSounds((unsigned int)&savedregs, localClientNum, cent, info);
  }
  else if ( v3 )
  {
    CG_UpdateWheeledSurfaceSounds((int)&savedregs, localClientNum, cent, info);
  }
}

void  CG_UpdateWheeledSurfaceSounds(
        int a1@<ebp>,
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info)
{
  double v4; // xmm0_8
  float v5; // xmm0_4
  long double v6; // [esp+8h] [ebp-4Ch]
  long double v7; // [esp+8h] [ebp-4Ch]
  float v8; // [esp+10h] [ebp-44h]
  float v9; // [esp+14h] [ebp-40h]
  float v10; // [esp+18h] [ebp-3Ch]
  float ratio; // [esp+28h] [ebp-2Ch]
  float v12; // [esp+2Ch] [ebp-28h]
  float v13; // [esp+34h] [ebp-20h]
  float v14; // [esp+38h] [ebp-1Ch]
  float v15; // [esp+40h] [ebp-14h] BYREF
  int velocity_forward; // [esp+44h] [ebp-10h]
  int v17; // [esp+48h] [ebp-Ch]
  float slideRate; // [esp+4Ch] [ebp-8h]
  float retaddr; // [esp+54h] [ebp+0h]

  v17 = a1;
  slideRate = retaddr;
  velocity_forward = 0;
  v15 = *(float *)&FLOAT_0_0;
  velocity_forward = GetSoundInfoForVehicle(localClientNum, cent, &v15);
  LODWORD(v14) = LODWORD(cent->nitrousVeh->m_forward_vel) & _mask__AbsFloat_;
  v13 = v14 / NitrousVehicle::get_max_speed(cent->nitrousVeh, 0) * 1.5707964;
  __libm_sse2_sin(v6);
  if ( (float)(v13 - 1.0) < 0.0 )
    v12 = v13;
  else
    v12 = FLOAT_1_0;
  if ( (float)(0.0 - v13) < 0.0 )
    ratio = v12;
  else
    ratio = *(float *)&FLOAT_0_0;
  CG_CrossFadeVehicleMaterialEffects(localClientNum, cent, info, 0, velocity_forward, ratio, fadetime);
  v4 = (float)(1.5707964 * v15);
  __libm_sse2_sin(v7);
  v5 = v4;
  if ( (float)(v5 - 1.0) < 0.0 )
    v10 = v5;
  else
    v10 = FLOAT_1_0;
  if ( (float)(0.0 - v5) < 0.0 )
    v9 = v10;
  else
    v9 = *(float *)&FLOAT_0_0;
  v15 = v9;
  CG_CrossFadeVehicleMaterialEffects(localClientNum, cent, info, 1u, velocity_forward, v9, fadetime);
  if ( NitrousVehicle::is_peeling_out(cent->nitrousVeh) )
    v8 = FLOAT_1_0;
  else
    v8 = *(float *)&FLOAT_0_0;
  CG_CrossFadeVehicleMaterialEffects(localClientNum, cent, info, 2u, velocity_forward, v8, fadetime);
}

// local variable allocation has failed, the output may be wrong!
void  CG_UpdateTankSurfaceSounds(
        unsigned int a1@<ebp>,
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info)
{
  float clacksCurveMaxOmitted; // xmm0_4
  const char *v5; // eax
  char *v6; // eax
  long double v7; // [esp+8h] [ebp-13Ch]
  float v8; // [esp+18h] [ebp-12Ch]
  int v9; // [esp+1Ch] [ebp-128h]
  int _rand; // [esp+20h] [ebp-124h]
  unsigned int _limit; // [esp+24h] [ebp-120h]
  int surface_group_index; // [esp+2Ch] [ebp-118h]
  int surface_type_index; // [esp+30h] [ebp-114h]
  int v14; // [esp+34h] [ebp-110h]
  int wheelIndex; // [esp+38h] [ebp-10Ch]
  int maxGroup; // [esp+44h] [ebp-100h]
  int oldNumTracks; // [esp+4Ch] [ebp-F8h]
  int newDistInches; // [esp+50h] [ebp-F4h]
  int fNewDistInches; // [esp+54h] [ebp-F0h]
  int iSide; // [esp+5Ch] [ebp-E8h]
  float iType; // [esp+60h] [ebp-E4h]
  phys_vec3 v22; // [esp+68h] [ebp-DCh] BYREF
  float v23; // [esp+84h] [ebp-C0h]
  float v24; // [esp+88h] [ebp-BCh]
  bool v25; // [esp+8Fh] [ebp-B5h]
  float frameDistInches; // [esp+90h] [ebp-B4h]
  float frametimeSeconds; // [esp+94h] [ebp-B0h]
  int *p_m_trackDistAccumRightInches; // [esp+98h] [ebp-ACh] OVERLAPPED
  int rateLengths[1]; // [esp+9Ch] [ebp-A8h]
  int *accum[1][2]; // [esp+A0h] [ebp-A4h]
  int NUM_TYPES; // [esp+A8h] [ebp-9Ch]
  const cg_s *cgameGlob; // [esp+ACh] [ebp-98h]
  const char *soundalias; // [esp+B0h] [ebp-94h]
  float v34; // [esp+B4h] [ebp-90h]
  int v35; // [esp+B8h] [ebp-8Ch]
  bool v36; // [esp+BEh] [ebp-86h]
  bool v37; // [esp+BFh] [ebp-85h]
  float HealthPercentageRightTread; // [esp+C0h] [ebp-84h]
  int sndNameIndex; // [esp+C4h] [ebp-80h]
  float v40; // [esp+C8h] [ebp-7Ch] OVERLAPPED
  float currRightTreadHealthRatio; // [esp+CCh] [ebp-78h]
  float currLeftTreadHealthRatio; // [esp+D0h] [ebp-74h]
  float attenuation; // [esp+D4h] [ebp-70h]
  float damageRatio; // [esp+D8h] [ebp-6Ch]
  float healthRatio; // [esp+DCh] [ebp-68h]
  float speedRatio; // [esp+E0h] [ebp-64h]
  float v47; // [esp+E4h] [ebp-60h]
  float *origin; // [esp+E8h] [ebp-5Ch]
  float *v49; // [esp+ECh] [ebp-58h]
  float *speedIPS; // [esp+F0h] [ebp-54h]
  float v51[3]; // [esp+F4h] [ebp-50h] BYREF
  float wheelOrigins[8]; // [esp+100h] [ebp-44h] OVERLAPPED BYREF
  float axis[3][3]; // [esp+120h] [ebp-24h] BYREF
  unsigned int retaddr; // [esp+144h] [ebp+0h]

  *(_QWORD *)&axis[2][0] = __PAIR64__(retaddr, a1);
  AnglesToAxis(cent->pose.angles, (float (*)[3])&wheelOrigins[5]);
  LODWORD(wheelOrigins[4]) = axis;
  LODWORD(wheelOrigins[3]) = cent->pose.origin;
  v51[0] = (float)(35.0 * axis[0][0]) + cent->pose.origin[0];
  v51[1] = (float)(35.0 * axis[0][1]) + cent->pose.origin[1];
  v51[2] = (float)(35.0 * axis[0][2]) + cent->pose.origin[2];
  speedIPS = wheelOrigins;
  v49 = axis[0];
  origin = cent->pose.origin;
  wheelOrigins[0] = (float)(-35.0 * axis[0][0]) + cent->pose.origin[0];
  wheelOrigins[1] = (float)(-35.0 * axis[0][1]) + cent->pose.origin[1];
  wheelOrigins[2] = (float)(-35.0 * axis[0][2]) + cent->pose.origin[2];
  v47 = Abs(cent->currentState.pos.trDelta);
  speedRatio = v47 / info->engineSndSpeed;
  if ( (float)(speedRatio - 1.0) < 0.0 )
    healthRatio = speedRatio;
  else
    healthRatio = FLOAT_1_0;
  if ( (float)(0.0 - speedRatio) < 0.0 )
    damageRatio = healthRatio;
  else
    damageRatio = *(float *)&FLOAT_0_0;
  attenuation = damageRatio;
  currLeftTreadHealthRatio = CG_VehGetHealthPercentageEntity(cent);
  currRightTreadHealthRatio = 1.0 - currLeftTreadHealthRatio;
  if ( playBodyDmgLoop )
  {
    v40 = attenuation * currRightTreadHealthRatio;
    CG_PlaySound(
      localClientNum,
      cent->nextState.number,
      v51,
      0,
      0,
      attenuation * currRightTreadHealthRatio,
      info->sndNames[11]);
    CG_PlaySound(
      localClientNum,
      cent->nextState.number,
      wheelOrigins,
      0,
      0,
      attenuation * currRightTreadHealthRatio,
      info->sndNames[12]);
  }
  if ( playTreadSqueaks )
  {
    *(float *)&sndNameIndex = CG_VehGetHealthPercentageLeftTread(cent);
    HealthPercentageRightTread = CG_VehGetHealthPercentageRightTread(cent);
    v37 = info->treadDamageLoopHealthRatio > *(float *)&sndNameIndex;
    v36 = v37;
    v35 = v37 ? 15 : 13;
    v34 = info->treadGrindAttenuation * attenuation;
    CG_PlaySound(localClientNum, cent->nextState.number, v51, 0, 0, v34, info->sndNames[v35]);
    HIBYTE(soundalias) = info->treadDamageLoopHealthRatio > HealthPercentageRightTread;
    BYTE2(soundalias) = HIBYTE(soundalias);
    cgameGlob = (const cg_s *)(HIBYTE(soundalias) != 0 ? 16 : 14);
    *(float *)&NUM_TYPES = info->treadGrindAttenuation * attenuation;
    CG_PlaySound(
      localClientNum,
      cent->nextState.number,
      wheelOrigins,
      0,
      0,
      *(float *)&NUM_TYPES,
      info->sndNames[(unsigned int)cgameGlob]);
  }
  accum[0][1] = (int *)info->sndMaterialNames[0];
  if ( info->sndMaterialNames[0][0] )
  {
    accum[0][0] = (int *)CG_GetLocalClientGlobals(localClientNum);
    rateLengths[0] = 1;
    LODWORD(frametimeSeconds) = &cent->nitrousVeh->m_trackDistAccumLeftInches;
    p_m_trackDistAccumRightInches = &cent->nitrousVeh->m_trackDistAccumRightInches;
    frameDistInches = *(float *)&info->trackLengthInches;
    v25 = playTreadClacks;
    v24 = (float)accum[0][0][97229] * 0.001;
    v23 = v24 * v47;
    if ( cent->nitrousVeh )
      iType = NitrousVehicle::get_a_velocity(cent->nitrousVeh, &v22)->z;
    else
      iType = cent->nextState.lerp.apos.trDelta[2];
    *(float *)&iSide = 48.0 * v24;
    __libm_sse2_sin(v7);
    for ( fNewDistInches = 0; !fNewDistInches; fNewDistInches = 1 )
    {
      if ( SLODWORD(frameDistInches) > 0 )
      {
        for ( newDistInches = 0; newDistInches < 2; ++newDistInches )
        {
          oldNumTracks = **((unsigned int **)&frametimeSeconds + newDistInches);
          maxGroup = (int)(float)((float)((float)oldNumTracks + v23)
                                + (float)(factor[newDistInches] * (float)(*(float *)&iSide * iType)));
          **((unsigned int **)&frametimeSeconds + newDistInches) = maxGroup;
          if ( maxGroup / SLODWORD(frameDistInches) != oldNumTracks / SLODWORD(frameDistInches) )
          {
            **((int **)&frametimeSeconds + newDistInches) %= SLODWORD(frameDistInches);
            if ( v25 )
            {
              wheelIndex = -1;
              v14 = 0;
              for ( surface_type_index = 0; surface_type_index < 3; ++surface_type_index )
              {
                surface_group_index = aaWheels_0[newDistInches][surface_type_index];
                if ( (cent->nitrousVeh->m_wheels[surface_group_index]->m_wheel_flags & 1) != 0 )
                {
                  _limit = cent->nitrousVeh->m_wheel_surf_types[surface_group_index];
                  if ( _limit > 0x1E
                    && !Assert_MyHandler(
                          "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                          1159,
                          0,
                          "%s",
                          "surface_type_index >= 0 && surface_type_index < ( sizeof( s_kaiSurfaceGroups ) / sizeof( s_kai"
                          "SurfaceGroups[ 0 ] ) )") )
                  {
                    __debugbreak();
                  }
                  _rand = s_kaiSurfaceGroups[_limit];
                  if ( (unsigned int)_rand >= 2
                    && !Assert_MyHandler(
                          "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                          1162,
                          0,
                          "%s",
                          "surface_group_index >= 0 && surface_group_index < NUM_SURF_GROUPS") )
                  {
                    __debugbreak();
                  }
                  if ( wheelIndex < _rand )
                    v9 = _rand;
                  else
                    v9 = wheelIndex;
                  wheelIndex = v9;
                  if ( v9 == _rand )
                    v14 = _limit;
                }
              }
              if ( wheelIndex >= 0 )
              {
                if ( (_S2_2 & 1) == 0 )
                {
                  _S2_2 |= 1u;
                  clacksCurveSlope = info->clacksCurveMaxOmitted
                                   / (float)(info->clacksCurveDamageEnd - info->clacksCurveDamageStart);
                }
                if ( info->clacksCurveDamageStart < currRightTreadHealthRatio )
                {
                  if ( currRightTreadHealthRatio < info->clacksCurveDamageStart )
                    clacksCurveMaxOmitted = (float)(currRightTreadHealthRatio - info->clacksCurveDamageStart)
                                          * clacksCurveSlope;
                  else
                    clacksCurveMaxOmitted = info->clacksCurveMaxOmitted;
                  v8 = clacksCurveMaxOmitted;
                }
                else
                {
                  v8 = *(float *)&FLOAT_0_0;
                }
                if ( (float)((float)(rand() % 101) / 100.0) >= v8 )
                {
                  v5 = Com_SurfaceTypeToName(v14);
                  v6 = va("%s_%s", (const char *)accum[0][1], v5);
                  CG_PlaySound(localClientNum, cent->nextState.number, &v51[3 * newDistInches], 0, 0, 1.0, v6);
                }
              }
            }
          }
        }
      }
    }
  }
}

void __cdecl CG_UpdateFireSounds(
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle)
{
  char *v4; // [esp+8h] [ebp-Ch]
  float healthRatio; // [esp+10h] [ebp-4h]

  if ( playFireSounds )
  {
    healthRatio = CG_VehGetHealthPercentageEntity(cent);
    if ( healthRatio < 0.5 )
    {
      if ( isLocalPlayersVehicle )
        v4 = "tank_burning_1p_loop";
      else
        v4 = "tank_burning_3p_loop";
      CG_PlaySound(localClientNum, cent->nextState.number, cent->pose.origin, 0, 0, 1.0, v4);
    }
  }
}

void __cdecl CG_UpdateBoostSounds(
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle)
{
  float soundOrigin[3]; // [esp+8h] [ebp-14h] BYREF
  int sndNameIndex; // [esp+14h] [ebp-8h]
  bool wasBoosting; // [esp+1Bh] [ebp-1h]

  if ( cent->nitrousVeh )
  {
    wasBoosting = (cent->nitrousVeh->m_sfx_state_flags & 2) != 0;
    if ( NitrousVehicle::is_boosting(cent->nitrousVeh) && !wasBoosting )
    {
      GetSoundOriginForVehicle(cent, soundOrigin);
      sndNameIndex = isLocalPlayersVehicle + 17;
      CG_PlaySound(localClientNum, cent->nextState.number, soundOrigin, 0, 0, 1.0, info->sndNames[sndNameIndex]);
    }
    if ( NitrousVehicle::is_boosting(cent->nitrousVeh) )
      cent->nitrousVeh->m_sfx_state_flags |= 2u;
    else
      cent->nitrousVeh->m_sfx_state_flags &= ~2u;
  }
}

void  CG_UpdateVehicleEngineSounds(
        int a1@<ebp>,
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle)
{
  float v5; // xmm0_4
  long double v6; // [esp+Ch] [ebp-9Ch]
  float v7; // [esp+14h] [ebp-94h] BYREF
  VehicleSound sndLoadNameIndex; // [esp+18h] [ebp-90h] BYREF
  VehicleSound sndNoLoadNameIndex; // [esp+1Ch] [ebp-8Ch]
  float noLoadLevel; // [esp+20h] [ebp-88h]
  float loadLevel; // [esp+24h] [ebp-84h]
  float pitch; // [esp+28h] [ebp-80h]
  unsigned int v13; // [esp+2Ch] [ebp-7Ch]
  float engineLoopLoadFactor; // [esp+30h] [ebp-78h]
  float engineLoopModulation; // [esp+34h] [ebp-74h]
  int sid; // [esp+38h] [ebp-70h]
  int attenuation; // [esp+3Ch] [ebp-6Ch]
  float v18; // [esp+40h] [ebp-68h]
  float v19; // [esp+44h] [ebp-64h]
  float v20; // [esp+48h] [ebp-60h]
  float idle_attenuation; // [esp+4Ch] [ebp-5Ch]
  bool v22; // [esp+50h] [ebp-58h]
  float v23[2]; // [esp+54h] [ebp-54h] BYREF
  float soundOrigin[3]; // [esp+60h] [ebp-48h]
  float idleRPMDelta; // [esp+6Ch] [ebp-3Ch]
  float idleLoopModulation; // [esp+70h] [ebp-38h]
  cgVehicleWheelEffect *wheelEffects; // [esp+74h] [ebp-34h]
  float dydx; // [esp+78h] [ebp-30h]
  float engineRPMs; // [esp+7Ch] [ebp-2Ch]
  cgVehicleWheelEffect *wheelEffect; // [esp+80h] [ebp-28h]
  float speedMPH; // [esp+84h] [ebp-24h]
  float damageRatio; // [esp+88h] [ebp-20h]
  float healthRatio; // [esp+8Ch] [ebp-1Ch]
  float speedRatio; // [esp+90h] [ebp-18h]
  float v35; // [esp+94h] [ebp-14h]
  float m_throttle; // [esp+98h] [ebp-10h]
  int v37; // [esp+9Ch] [ebp-Ch]
  float speedIPS; // [esp+A0h] [ebp-8h]
  float retaddr; // [esp+A8h] [ebp+0h]

  v37 = a1;
  speedIPS = retaddr;
  m_throttle = (float)cent->nextState.lerp.u.vehicle.throttle * 0.000030517578;
  if ( cent->nitrousVeh )
    m_throttle = cent->nitrousVeh->m_throttle;
  v35 = Abs(cent->currentState.pos.trDelta);
  speedRatio = v35 / info->engineSndSpeed;
  if ( (float)(speedRatio - 1.0) < 0.0 )
    healthRatio = speedRatio;
  else
    healthRatio = FLOAT_1_0;
  if ( (float)(0.0 - speedRatio) < 0.0 )
    damageRatio = healthRatio;
  else
    damageRatio = *(float *)&FLOAT_0_0;
  speedMPH = damageRatio;
  *(float *)&wheelEffect = CG_VehGetHealthPercentageEntity(cent);
  engineRPMs = 1.0 - *(float *)&wheelEffect;
  dydx = v35 / 17.6;
  wheelEffects = cent->vehicle->wheelEffects;
  if ( wheelEffects->surfaceType == -1 )
  {
    idleLoopModulation = (float)((float)(info->gear2MaxRPMs - info->idleRPMs) * m_throttle) + info->idleRPMs;
  }
  else if ( cent->nitrousVeh && m_throttle == 0.0 )
  {
    idleLoopModulation = info->idleRPMs;
  }
  else
  {
    if ( info->gearChangeMPH <= dydx )
    {
      soundOrigin[2] = (float)(info->gear2MaxRPMs - info->gear2MinRPMs) / (float)(info->maxSpeed - info->gearChangeMPH);
      v5 = (float)((float)(dydx - info->gearChangeMPH) * soundOrigin[2]) + info->gear2MinRPMs;
    }
    else
    {
      idleRPMDelta = (float)(info->gear1MaxRPMs - info->gear1MinRPMs) / (float)(info->gearChangeMPH - 0.0);
      v5 = (float)((float)(dydx - 0.0) * idleRPMDelta) + info->gear1MinRPMs;
    }
    idleLoopModulation = v5;
  }
  soundOrigin[1] = idleLoopModulation / info->idleRPMs;
  soundOrigin[0] = info->idleRPMsMax - info->idleRPMs;
  if ( soundOrigin[0] <= 0.0 )
    soundOrigin[0] = FLOAT_1_0;
  GetSoundOriginForVehicle(cent, v23);
  v22 = isLocalPlayersVehicle;
  idle_attenuation = (float)(idleLoopModulation - info->idleRPMs) / soundOrigin[0];
  if ( (float)(idle_attenuation - 1.0) < 0.0 )
    v20 = idle_attenuation;
  else
    v20 = FLOAT_1_0;
  if ( (float)(0.0 - idle_attenuation) < 0.0 )
    v19 = v20;
  else
    v19 = *(float *)&FLOAT_0_0;
  v18 = v19;
  __libm_sse2_cos(v6);
  *(float *)&attenuation = 1.5707964 * v19;
  if ( (float)((float)(1.5707964 * v19) - 1.0) < 0.0 )
    sid = attenuation;
  else
    sid = LODWORD(FLOAT_1_0);
  if ( (float)(0.0 - *(float *)&attenuation) < 0.0 )
    engineLoopModulation = *(float *)&sid;
  else
    engineLoopModulation = *(float *)&FLOAT_0_0;
  engineLoopLoadFactor = engineLoopModulation;
  v13 = CG_PlaySoundWithHandle(
          localClientNum,
          cent->nextState.number,
          v23,
          0,
          0,
          engineLoopModulation,
          info->sndNames[v22]);
  SND_SetPlaybackPitch(v13, soundOrigin[1]);
  pitch = idleLoopModulation / info->engineModLoopNaturalRPMs;
  loadLevel = (float)(COERCE_FLOAT(LODWORD(m_throttle) & _mask__AbsFloat_) - speedMPH) + engineRPMs;
  if ( (float)(loadLevel - 1.0) < 0.0 )
    noLoadLevel = loadLevel;
  else
    noLoadLevel = FLOAT_1_0;
  if ( (float)(0.0 - loadLevel) < 0.0 )
    sndNoLoadNameIndex = LODWORD(noLoadLevel);
  else
    sndNoLoadNameIndex = *(unsigned int *)&FLOAT_0_0;
  SND_EqualPowerFadeCoefs(*(float *)&sndNoLoadNameIndex, &v7, (float *)&sndLoadNameIndex);
  engineLoopLoadFactor = v7 * v18;
  v13 = CG_PlaySoundWithHandle(
          localClientNum,
          cent->nextState.number,
          v23,
          0,
          0,
          v7 * v18,
          info->sndNames[isLocalPlayersVehicle + 7]);
  SND_SetPlaybackPitch(v13, pitch);
  engineLoopLoadFactor = *(float *)&sndLoadNameIndex * v18;
  v13 = CG_PlaySoundWithHandle(
          localClientNum,
          cent->nextState.number,
          v23,
          0,
          0,
          *(float *)&sndLoadNameIndex * v18,
          info->sndNames[isLocalPlayersVehicle + 9]);
  SND_SetPlaybackPitch(v13, pitch);
}

void __cdecl CG_UpdateTurretAndOverheatSounds(
        int localClientNum,
        centity_s *cent,
        const vehicle_info_t *info,
        bool isLocalPlayersVehicle)
{
  const FxEffectDef *viewLastShotEjectEffect; // [esp+8h] [ebp-90h]
  char *overheatSoundPlayer; // [esp+Ch] [ebp-8Ch]
  char *rotateLoopSoundPlayer; // [esp+10h] [ebp-88h]
  float v7; // [esp+14h] [ebp-84h]
  float v8; // [esp+18h] [ebp-80h]
  float v9; // [esp+1Ch] [ebp-7Ch]
  bool v10; // [esp+23h] [ebp-75h]
  float v11; // [esp+24h] [ebp-74h]
  float v12; // [esp+28h] [ebp-70h]
  unsigned __int16 flash; // [esp+30h] [ebp-68h]
  float gunPitch; // [esp+3Ch] [ebp-5Ch]
  float gunYaw; // [esp+44h] [ebp-54h]
  float yawMax; // [esp+48h] [ebp-50h]
  float pitchMax; // [esp+4Ch] [ebp-4Ch]
  float yawMin; // [esp+54h] [ebp-44h]
  float pitchLockFade; // [esp+58h] [ebp-40h]
  float pitchMin; // [esp+64h] [ebp-34h]
  float yawLockFade; // [esp+6Ch] [ebp-2Ch]
  int overheatFlag; // [esp+7Ch] [ebp-1Ch]
  int vehSeat; // [esp+80h] [ebp-18h]
  char isOverheating; // [esp+87h] [ebp-11h]
  const WeaponDef *pWeapon; // [esp+88h] [ebp-10h]
  bool gun_turning_pitch; // [esp+8Ch] [ebp-Ch]
  bool gun_turning_yaw; // [esp+8Dh] [ebp-Bh]
  char wasOverheating; // [esp+8Fh] [ebp-9h]
  unsigned int i; // [esp+90h] [ebp-8h]
  const cg_s *cgameGlob; // [esp+94h] [ebp-4h]

  if ( (playTurretRotationSounds || playOverheatSounds) && cent->nitrousVeh )
  {
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    for ( i = 0; i < 2; ++i )
    {
      vehSeat = seat_positions[i];
      if ( vehSeat
        && (vehSeat < 1 || vehSeat > 4)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
              1402,
              0,
              "%s",
              "( vehSeat == VEH_POS_DRIVER ) || VEH_GUNNER_POSITION( vehSeat )") )
      {
        __debugbreak();
      }
      v10 = cgameGlob->bgs.clientinfo[cgameGlob->clientNum].attachedVehSeat == vehSeat && isLocalPlayersVehicle;
      gun_turning_yaw = CG_VehGetSeatGunTurningYaw(cent, vehSeat);
      gun_turning_pitch = CG_VehGetSeatGunTurningPitch(cent, vehSeat);
      overheatFlag = overheat_flags[i];
      wasOverheating = (overheatFlag & cent->nitrousVeh->m_sfx_state_flags) != 0;
      isOverheating = 0;
      if ( v10 )
      {
        if ( vehSeat )
        {
          if ( vehSeat >= 1 && vehSeat <= 4 )
            isOverheating = BG_PlayerWeaponOverheating(
                              &cgameGlob->predictedPlayerState,
                              *(unsigned __int16 *)&info->gunnerWeapon[3][2 * vehSeat + 62]);
        }
        else
        {
          isOverheating = cent->nextState.lerp.u.turret.overheating != 0;
        }
      }
      else
      {
        isOverheating = CG_VehGetSeatGunOverheating(cent, vehSeat);
      }
      if ( isOverheating != wasOverheating || gun_turning_yaw || gun_turning_pitch )
      {
        if ( vehSeat )
        {
          pWeapon = BG_GetWeaponDef(*(unsigned __int16 *)&info->gunnerWeapon[3][2 * vehSeat + 62]);
          if ( !pWeapon
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                  1458,
                  0,
                  "%s",
                  "pWeapon != NULL") )
          {
            __debugbreak();
          }
        }
        else
        {
          pWeapon = BG_GetWeaponDef(cent->nextState.weapon);
          if ( !pWeapon
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                  1452,
                  0,
                  "%s",
                  "pWeapon != NULL") )
          {
            __debugbreak();
          }
        }
        if ( (gun_turning_yaw || gun_turning_pitch) && playTurretRotationSounds )
        {
          if ( vehSeat )
          {
            gunYaw = (float)cent->currentState.u.vehicle.gunnerAngles[vehSeat - 1].yaw * 0.0054931641;
            LODWORD(yawMin) = LODWORD(pWeapon->rightArc) ^ _mask__NegFloat_;
            yawMax = pWeapon->leftArc;
            gunPitch = (float)*(__int16 *)&cent->currentState.u.primaryLight.colorAndExp[4 * vehSeat + 4] * 0.0054931641;
            LODWORD(pitchMin) = LODWORD(pWeapon->topArc) ^ _mask__NegFloat_;
            pitchMax = pWeapon->bottomArc;
          }
          else
          {
            gunYaw = (float)cent->currentState.u.vehicle.gunYaw * 0.0054931641;
            LODWORD(yawMin) = LODWORD(info->turretViewLimits.horizSpanRight) ^ _mask__NegFloat_;
            yawMax = info->turretViewLimits.horizSpanLeft;
            gunPitch = (float)cent->currentState.u.vehicle.gunPitch * 0.0054931641;
            LODWORD(pitchMin) = LODWORD(info->turretViewLimits.vertSpanUp) ^ _mask__NegFloat_;
            pitchMax = info->turretViewLimits.vertSpanDown;
          }
          yawLockFade = FLOAT_1_0;
          pitchLockFade = FLOAT_1_0;
          if ( (float)(yawMin + 3.0) <= gunYaw )
          {
            if ( gunYaw > (float)(yawMax - 3.0) )
              yawLockFade = (float)(yawMax - gunYaw) / 3.0;
          }
          else
          {
            yawLockFade = (float)(gunYaw - yawMin) / 3.0;
          }
          if ( (float)(pitchMin + 3.0) <= gunPitch )
          {
            if ( gunPitch > (float)(pitchMax - 3.0) )
              pitchLockFade = (float)(pitchMax - gunPitch) / 3.0;
          }
          else
          {
            pitchLockFade = (float)(gunPitch - pitchMin) / 3.0;
          }
          if ( gun_turning_yaw )
            v12 = FLOAT_1_0;
          else
            v12 = *(float *)&FLOAT_0_0;
          if ( (float)(yawLockFade - v12) < 0.0 )
            v9 = yawLockFade;
          else
            v9 = v12;
          if ( gun_turning_pitch )
            v11 = FLOAT_1_0;
          else
            v11 = *(float *)&FLOAT_0_0;
          if ( (float)(pitchLockFade - v11) < 0.0 )
            v8 = pitchLockFade;
          else
            v8 = v11;
          if ( (float)(v9 - v8) < 0.0 )
            v7 = v8;
          else
            v7 = v9;
          if ( v10 )
            rotateLoopSoundPlayer = (char *)pWeapon->rotateLoopSoundPlayer;
          else
            rotateLoopSoundPlayer = (char *)pWeapon->rotateLoopSound;
          if ( rotateLoopSoundPlayer )
            CG_PlaySound(localClientNum, cent->nextState.number, cent->pose.origin, 0, 0, v7, rotateLoopSoundPlayer);
        }
        if ( playOverheatSounds )
        {
          if ( isOverheating && !wasOverheating )
          {
            if ( v10 )
              overheatSoundPlayer = (char *)pWeapon->overheatSoundPlayer;
            else
              overheatSoundPlayer = (char *)pWeapon->overheatSound;
            CG_PlaySound(localClientNum, cent->nextState.number, cent->pose.origin, 0, 0, 1.0, overheatSoundPlayer);
            flash = scr_const.tag_flash;
            if ( vehSeat >= 1 && vehSeat <= 4 )
              flash = *(&scr_const.tag_gunner_turret4 + vehSeat);
            if ( v10 )
              viewLastShotEjectEffect = pWeapon->viewLastShotEjectEffect;
            else
              viewLastShotEjectEffect = pWeapon->worldLastShotEjectEffect;
            if ( viewLastShotEjectEffect )
              CG_PlayBoltedEffect(localClientNum, viewLastShotEjectEffect, cent->nextState.number, flash);
          }
          if ( isOverheating )
            cent->nitrousVeh->m_sfx_state_flags |= overheatFlag;
          else
            cent->nitrousVeh->m_sfx_state_flags &= ~overheatFlag;
        }
      }
    }
  }
}

