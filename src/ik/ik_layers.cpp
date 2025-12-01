#include "ik_layers.h"

// local variable allocation has failed, the output may be wrong!
void  IK_Layer_TerrainMapping(float a1@<ebp>, IKState *ikState)
{
  float *v2; // edx
  float *v3; // ecx
  float *v4; // eax
  float *v5; // edx
  double v6; // st7
  float v7; // xmm0_4
  float v8; // xmm0_4
  double v9; // st7
  double v10; // st7
  float v11; // xmm0_4
  float v12; // xmm0_4
  float v13; // xmm0_4
  float v14; // xmm0_4
  double v15; // st7
  float v16; // xmm0_4
  float v17; // xmm0_4
  float v18; // xmm0_4
  float *v19; // [esp+24h] [ebp-790h]
  float v20; // [esp+38h] [ebp-77Ch]
  float v21; // [esp+3Ch] [ebp-778h]
  float v22; // [esp+40h] [ebp-774h]
  __int64 curFoot_8; // [esp+4Ch] [ebp-768h]
  __int64 curFoot_24; // [esp+5Ch] [ebp-758h]
  float v25; // [esp+A0h] [ebp-714h]
  float v26; // [esp+B0h] [ebp-704h]
  _BYTE v27[64]; // [esp+D8h] [ebp-6DCh] OVERLAPPED BYREF
  float *v28; // [esp+124h] [ebp-690h]
  _BYTE v29[76]; // [esp+128h] [ebp-68Ch] OVERLAPPED BYREF
  float v30; // [esp+174h] [ebp-640h]
  float v31; // [esp+178h] [ebp-63Ch]
  float v32[3]; // [esp+17Ch] [ebp-638h] BYREF
  float *v33; // [esp+188h] [ebp-62Ch]
  float v34; // [esp+18Ch] [ebp-628h]
  float *v35; // [esp+190h] [ebp-624h]
  float v36; // [esp+194h] [ebp-620h]
  float v37; // [esp+198h] [ebp-61Ch]
  float v38; // [esp+19Ch] [ebp-618h]
  float XYdist; // [esp+1A0h] [ebp-614h]
  float v40; // [esp+1A4h] [ebp-610h]
  int v41; // [esp+1A8h] [ebp-60Ch]
  float *v42; // [esp+1ACh] [ebp-608h]
  unsigned int *v43; // [esp+1B0h] [ebp-604h]
  float v44; // [esp+1B4h] [ebp-600h]
  float fraction; // [esp+1B8h] [ebp-5FCh]
  float v46; // [esp+1BCh] [ebp-5F8h]
  float angularWeight; // [esp+1C0h] [ebp-5F4h]
  float v48; // [esp+1C4h] [ebp-5F0h]
  float footOffsetAngularScale; // [esp+1C8h] [ebp-5ECh]
  float v50; // [esp+1CCh] [ebp-5E8h]
  float v51; // [esp+1D0h] [ebp-5E4h] BYREF
  int v52; // [esp+1D4h] [ebp-5E0h]
  float angularMappingHeightOffsetMax; // [esp+1D8h] [ebp-5DCh]
  float v_up[3]; // [esp+1DCh] [ebp-5D8h]
  float terrainNormalMinZ; // [esp+1E8h] [ebp-5CCh]
  float terrainNormalRangeCap; // [esp+1ECh] [ebp-5C8h]
  float terrainNormalLerpTime; // [esp+1F0h] [ebp-5C4h]
  float v58; // [esp+1F4h] [ebp-5C0h]
  float bSq; // [esp+1F8h] [ebp-5BCh]
  float cMaxSq; // [esp+1FCh] [ebp-5B8h]
  float v61; // [esp+200h] [ebp-5B4h]
  float v62; // [esp+204h] [ebp-5B0h]
  float v63; // [esp+208h] [ebp-5ACh]
  float cSq; // [esp+20Ch] [ebp-5A8h]
  _BYTE *aSq; // [esp+210h] [ebp-5A4h]
  const float *v66; // [esp+214h] [ebp-5A0h]
  _BYTE v67[76]; // [esp+218h] [ebp-59Ch] OVERLAPPED BYREF
  float v68; // [esp+264h] [ebp-550h]
  float v69; // [esp+268h] [ebp-54Ch]
  float terrainLegExtendAdjust; // [esp+26Ch] [ebp-548h]
  float v71; // [esp+270h] [ebp-544h]
  float v72; // [esp+274h] [ebp-540h]
  float changeZ; // [esp+278h] [ebp-53Ch]
  float realPelvisZ; // [esp+27Ch] [ebp-538h]
  float PELVIS_ABS_SCALAR; // [esp+280h] [ebp-534h]
  float zOfs; // [esp+284h] [ebp-530h]
  float pelvisMovingZAdjust; // [esp+288h] [ebp-52Ch]
  float v78; // [esp+28Ch] [ebp-528h]
  float v79; // [esp+290h] [ebp-524h]
  float v80; // [esp+294h] [ebp-520h]
  float v81; // [esp+298h] [ebp-51Ch]
  bool pelvisForceZAdjust; // [esp+29Ch] [ebp-518h]
  float pelvisMoveScale; // [esp+2A0h] [ebp-514h]
  float lowerAdjust; // [esp+2A4h] [ebp-510h]
  int lowerFoot; // [esp+2A8h] [ebp-50Ch]
  int v86; // [esp+2ACh] [ebp-508h]
  float v87; // [esp+2B0h] [ebp-504h]
  float v88; // [esp+2B4h] [ebp-500h]
  float v89; // [esp+2B8h] [ebp-4FCh]
  float minScale; // [esp+2BCh] [ebp-4F8h]
  float footOffsetAdjusted; // [esp+2C0h] [ebp-4F4h]
  float footOffsetFromStandard; // [esp+2C4h] [ebp-4F0h]
  float v93; // [esp+2C8h] [ebp-4ECh]
  float v94; // [esp+2CCh] [ebp-4E8h]
  float v95; // [esp+2D0h] [ebp-4E4h]
  unsigned int v96[3]; // [esp+2D4h] [ebp-4E0h]
  float fastLerpScalar[2]; // [esp+2E0h] [ebp-4D4h]
  float terrainDiffNoLerpingThreshold; // [esp+2E8h] [ebp-4CCh] BYREF
  float terrainDiff; // [esp+2ECh] [ebp-4C8h]
  float *realZ; // [esp+2F0h] [ebp-4C4h]
  float *v101; // [esp+2F4h] [ebp-4C0h]
  float *v102; // [esp+2F8h] [ebp-4BCh]
  float *v103; // [esp+2FCh] [ebp-4B8h]
  unsigned int *v104; // [esp+300h] [ebp-4B4h]
  float *v105; // [esp+304h] [ebp-4B0h]
  float *v106; // [esp+308h] [ebp-4ACh]
  float v107; // [esp+30Ch] [ebp-4A8h] BYREF
  float v108; // [esp+310h] [ebp-4A4h]
  float v109; // [esp+314h] [ebp-4A0h]
  float end[3]; // [esp+318h] [ebp-49Ch] BYREF
  float start[3]; // [esp+324h] [ebp-490h]
  float v112; // [esp+330h] [ebp-484h]
  float v113; // [esp+334h] [ebp-480h]
  float *v114; // [esp+338h] [ebp-47Ch]
  float *v115; // [esp+33Ch] [ebp-478h]
  float footDirectionToNormalDot; // [esp+340h] [ebp-474h]
  float v117; // [esp+344h] [ebp-470h]
  float v118; // [esp+348h] [ebp-46Ch]
  float lowerFootTerrainDotClampMax; // [esp+34Ch] [ebp-468h]
  float lowerFootTerrainDotClampMin; // [esp+350h] [ebp-464h]
  float lowerFootTerrainDotScale; // [esp+354h] [ebp-460h]
  float lowerFootTerrainSteepnessScale; // [esp+358h] [ebp-45Ch]
  float traceFootAdjust; // [esp+35Ch] [ebp-458h]
  float v124; // [esp+360h] [ebp-454h]
  float v125; // [esp+364h] [ebp-450h]
  float v126; // [esp+368h] [ebp-44Ch]
  float tracePos[3]; // [esp+36Ch] [ebp-448h] BYREF
  float thisTracePos[3]; // [esp+378h] [ebp-43Ch] BYREF
  float thisNormal[3]; // [esp+384h] [ebp-430h]
  int k; // [esp+390h] [ebp-424h]
  float v131; // [esp+394h] [ebp-420h]
  char v132; // [esp+39Bh] [ebp-419h]
  int n; // [esp+39Ch] [ebp-418h] BYREF
  float maxTraceZ; // [esp+3A0h] [ebp-414h]
  float *v135; // [esp+3A4h] [ebp-410h] OVERLAPPED
  float *v136; // [esp+3A8h] [ebp-40Ch]
  float v137; // [esp+3ACh] [ebp-408h]
  float *v138; // [esp+3B0h] [ebp-404h]
  float *v139; // [esp+3B4h] [ebp-400h]
  float footTraceCacheDistThreshold; // [esp+3B8h] [ebp-3FCh]
  float *v141; // [esp+3BCh] [ebp-3F8h]
  float *v142; // [esp+3C0h] [ebp-3F4h]
  unsigned int v143[3]; // [esp+3C4h] [ebp-3F0h] BYREF
  float footTraceNormals[2][3]; // [esp+3D0h] [ebp-3E4h] BYREF
  float v145; // [esp+3E8h] [ebp-3CCh]
  float startPos[3]; // [esp+3ECh] [ebp-3C8h] BYREF
  float endPos[3]; // [esp+3F8h] [ebp-3BCh]
  float modifiedFeet; // [esp+408h] [ebp-3ACh] OVERLAPPED
  float groundOffsets[2]; // [esp+40Ch] [ebp-3A8h]
  float terrainZ; // [esp+414h] [ebp-3A0h]
  float *maxPelvisReach; // [esp+418h] [ebp-39Ch]
  float *highestGroundZ; // [esp+41Ch] [ebp-398h]
  float *lowestGroundZ; // [esp+420h] [ebp-394h]
  float *v154; // [esp+424h] [ebp-390h]
  float v155; // [esp+428h] [ebp-38Ch]
  float v156; // [esp+42Ch] [ebp-388h]
  float v157; // [esp+430h] [ebp-384h]
  float v158; // [esp+434h] [ebp-380h]
  float v159; // [esp+438h] [ebp-37Ch]
  float v160; // [esp+43Ch] [ebp-378h]
  float v161; // [esp+440h] [ebp-374h]
  float v162; // [esp+444h] [ebp-370h]
  float v163; // [esp+448h] [ebp-36Ch]
  float v164; // [esp+44Ch] [ebp-368h]
  float v165; // [esp+450h] [ebp-364h]
  float v166; // [esp+454h] [ebp-360h]
  float v167; // [esp+458h] [ebp-35Ch]
  float v168; // [esp+45Ch] [ebp-358h]
  float v169; // [esp+460h] [ebp-354h]
  float v170; // [esp+464h] [ebp-350h]
  float *v171; // [esp+46Ch] [ebp-348h]
  float *v172; // [esp+470h] [ebp-344h]
  float *v173; // [esp+474h] [ebp-340h]
  __int64 v174; // [esp+478h] [ebp-33Ch]
  __int64 v175; // [esp+480h] [ebp-334h]
  __int64 v176; // [esp+488h] [ebp-32Ch]
  __int64 v177; // [esp+490h] [ebp-324h]
  __int64 v178; // [esp+498h] [ebp-31Ch]
  __int64 v179; // [esp+4A0h] [ebp-314h]
  __int64 v180; // [esp+4A8h] [ebp-30Ch]
  __int64 v181; // [esp+4B0h] [ebp-304h]
  float *v182; // [esp+4B8h] [ebp-2FCh]
  float *v183; // [esp+4BCh] [ebp-2F8h]
  float *v184; // [esp+4C0h] [ebp-2F4h]
  float *v185; // [esp+4C4h] [ebp-2F0h]
  float v186; // [esp+4C8h] [ebp-2ECh]
  float v187; // [esp+4CCh] [ebp-2E8h]
  float v188; // [esp+4D0h] [ebp-2E4h]
  __int64 v189; // [esp+4D4h] [ebp-2E0h]
  __int64 v190; // [esp+4DCh] [ebp-2D8h]
  __int64 v191; // [esp+4E4h] [ebp-2D0h]
  __int64 v192; // [esp+4ECh] [ebp-2C8h]
  __int64 v193; // [esp+4F4h] [ebp-2C0h]
  __int64 v194; // [esp+4FCh] [ebp-2B8h]
  float v195; // [esp+504h] [ebp-2B0h]
  unsigned int *v196; // [esp+50Ch] [ebp-2A8h]
  float *v197; // [esp+510h] [ebp-2A4h]
  float *v198; // [esp+514h] [ebp-2A0h]
  _BYTE v199[12]; // [esp+518h] [ebp-29Ch] BYREF
  float worldFootMats[2][4][4]; // [esp+524h] [ebp-290h] BYREF
  unsigned int *v201; // [esp+5A4h] [ebp-210h]
  unsigned int v202[3]; // [esp+5A8h] [ebp-20Ch] BYREF
  float tmpMat[4][4]; // [esp+5B4h] [ebp-200h] BYREF
  float *v204; // [esp+5F4h] [ebp-1C0h]
  float *v205; // [esp+5F8h] [ebp-1BCh]
  float *v206; // [esp+5FCh] [ebp-1B8h]
  float v207; // [esp+600h] [ebp-1B4h]
  float v208; // [esp+604h] [ebp-1B0h]
  float v209; // [esp+608h] [ebp-1ACh]
  __int64 v210; // [esp+60Ch] [ebp-1A8h]
  __int64 v211; // [esp+614h] [ebp-1A0h]
  __int64 v212; // [esp+61Ch] [ebp-198h]
  __int64 v213; // [esp+624h] [ebp-190h]
  __int64 v214; // [esp+62Ch] [ebp-188h]
  __int64 v215; // [esp+634h] [ebp-180h]
  float v216; // [esp+63Ch] [ebp-178h]
  float *v217; // [esp+644h] [ebp-170h]
  float *v218; // [esp+648h] [ebp-16Ch]
  int j; // [esp+64Ch] [ebp-168h]
  float *v220; // [esp+650h] [ebp-164h]
  float *v221; // [esp+654h] [ebp-160h]
  int i; // [esp+658h] [ebp-15Ch]
  float v223; // [esp+65Ch] [ebp-158h]
  float v224; // [esp+660h] [ebp-154h]
  float footTraceLift; // [esp+664h] [ebp-150h]
  float v226; // [esp+668h] [ebp-14Ch]
  float minPelvisHeight; // [esp+66Ch] [ebp-148h]
  float v228; // [esp+670h] [ebp-144h]
  float v229; // [esp+674h] [ebp-140h]
  float v230; // [esp+678h] [ebp-13Ch]
  float maxFootRaise; // [esp+67Ch] [ebp-138h]
  float v232; // [esp+680h] [ebp-134h]
  float maxFootDrop; // [esp+684h] [ebp-130h]
  float v234; // [esp+688h] [ebp-12Ch]
  float velTimeScalar; // [esp+68Ch] [ebp-128h]
  float velSpeedScale; // [esp+690h] [ebp-124h]
  float *v237; // [esp+694h] [ebp-120h]
  float *lastTerrainMappingOrigin; // [esp+698h] [ebp-11Ch]
  float velSpeed; // [esp+69Ch] [ebp-118h]
  bool v240; // [esp+6A0h] [ebp-114h]
  float v241; // [esp+6A4h] [ebp-110h]
  float v242; // [esp+6A8h] [ebp-10Ch] OVERLAPPED
  float v243; // [esp+6ACh] [ebp-108h]
  float originChange[3]; // [esp+6B0h] [ebp-104h]
  char v245; // [esp+6BFh] [ebp-F5h]
  float v246; // [esp+6C0h] [ebp-F4h]
  float footTraceEndOffsetZ; // [esp+6C4h] [ebp-F0h]
  float v248; // [esp+6C8h] [ebp-ECh] OVERLAPPED
  float frametimeScalar; // [esp+6CCh] [ebp-E8h]
  float maxFootRaiseTurret; // [esp+6D0h] [ebp-E4h]
  float maxFootRaiseCrouching; // [esp+6D4h] [ebp-E0h]
  float maxFootRaiseStanding; // [esp+6D8h] [ebp-DCh]
  float maxFootDropMoving; // [esp+6DCh] [ebp-D8h]
  float maxFootDropIdleCr; // [esp+6E0h] [ebp-D4h]
  float maxFootDropIdle; // [esp+6E4h] [ebp-D0h]
  float maxFootSpan; // [esp+6E8h] [ebp-CCh]
  float minPelvisHeightTurret; // [esp+6ECh] [ebp-C8h]
  float minPelvisHeightCrouching; // [esp+6F0h] [ebp-C4h]
  float minPelvisHeightNormal; // [esp+6F4h] [ebp-C0h]
  float footInAirLiftReductionMaxInAir; // [esp+6F8h] [ebp-BCh]
  float maxPelvisReachAdjust; // [esp+6FCh] [ebp-B8h]
  float footTraceEndOffsetZIdle[2]; // [esp+700h] [ebp-B4h]
  float footTraceEndOffsetZMoving; // [esp+708h] [ebp-ACh]
  float entityBoundsMinZ; // [esp+70Ch] [ebp-A8h]
  float footTraceLiftTurret; // [esp+710h] [ebp-A4h]
  float footTraceLiftNormal; // [esp+714h] [ebp-A0h]
  float footTraceOffsetFromAnkle[2]; // [esp+718h] [ebp-9Ch]
  float IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MIN; // [esp+720h] [ebp-94h]
  float IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX; // [esp+724h] [ebp-90h]
  float IK_TERRAIN_MAPPING_VELOCITY_SCALAR[2]; // [esp+728h] [ebp-8Ch]
  float IK_TERRAIN_MAPPING_VELOCITY_MAX; // [esp+730h] [ebp-84h]
  float IK_TERRAIN_MAPPING_CLAMP; // [esp+734h] [ebp-80h]
  float IK_TERRAIN_MAPPING_LERP_SCALE; // [esp+738h] [ebp-7Ch]
  float IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE; // [esp+73Ch] [ebp-78h]
  float IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE; // [esp+740h] [ebp-74h]
  int v276; // [esp+74Ch] [ebp-68h]
  float v277; // [esp+750h] [ebp-64h]
  bool v280; // [esp+757h] [ebp-5Dh]
  _BYTE v281[76]; // [esp+758h] [ebp-5Ch] OVERLAPPED BYREF
  float layerLerp; // [esp+7A8h] [ebp-Ch]
  IKLayerNames layerName; // [esp+7ACh] [ebp-8h]
  IKLayerNames retaddr; // [esp+7B4h] [ebp+0h]

  layerLerp = a1;
  layerName = retaddr;
  *(unsigned int *)&v281[72] = 9;
  *(float *)&v281[68] = IKImport_GetLayerLerp(ikState, IKLAYER_TERRAIN_MAPPING, flt_E12704);
  if ( *(float *)&v281[68] > 0.0 )
  {
    v280 = ikState->entityNum < 32;
    if ( ikState->bJointVarsValid )
    {
      if ( IKImport_GetVar_IK_Foot_Test() )
      {
        if ( ikState->ikBoneToObjBone[18] != 161
          && ikState->ikBoneToObjBone[21] != 161
          && ikState->lastFrameMS[9] != ikState->timeMS )
        {
          ikState->lastFrameMS[9] = ikState->timeMS;
          if ( IK_IsCalcBone(ikState, 18) )
          {
            v277 = (float)(ikState->lastFrameMS[9] % 2000) - 1000.0;
            v276 = LODWORD(v277) & _mask__AbsFloat_;
            ikState->footMats[0][3][2] = (float)((float)(6.0 * fabs(v277)) / 1000.0)
                                       + ikState->footMats[0][3][2];
          }
          if ( IK_IsCalcBone(ikState, 21) )
            ikState->footMats[1][3][2] = (float)((float)(1.0
                                                       - (float)(COERCE_FLOAT(
                                                                   COERCE_UNSIGNED_INT((float)(ikState->lastFrameMS[9]
                                                                                             % 2000) - 1000.0)
                                                                 & _mask__AbsFloat_)
                                                               / 1000.0))
                                               * 6.0)
                                       + ikState->footMats[1][3][2];
        }
      }
      else
      {
        HIBYTE(IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE) = ikState->ikBoneToObjBone[18] != 161;
        if ( HIBYTE(IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE) )
        {
          BYTE2(IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE) = ikState->ikBoneToObjBone[21] != 161;
          if ( BYTE2(IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE) )
          {
            if ( IK_IsCalcBone(ikState, 18) || IK_IsCalcBone(ikState, 21) )
            {
              IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE = (float)(ikState->timeMS - ikState->lastFrameMS[9]) * 0.001;
              if ( IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE <= 0.0 )
              {
                if ( Demo_IsPlaying() )
                  IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE = 0.1f;
                else
                  IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE = 0.0f;
              }
              ikState->lastFrameMS[9] = ikState->timeMS;
              IK_TERRAIN_MAPPING_LERP_SCALE = IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE;
              IK_TERRAIN_MAPPING_CLAMP = 30.0f;
              IK_TERRAIN_MAPPING_VELOCITY_MAX = 30.0f;
              IK_TERRAIN_MAPPING_VELOCITY_SCALAR[1] = 6.0f;
              IK_TERRAIN_MAPPING_VELOCITY_SCALAR[0] = 32.0f;
              IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX = 300.0f;
              footTraceOffsetFromAnkle[1] = 7.0f;
              IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MIN = 7.0f;
              footTraceOffsetFromAnkle[0] = 8.0f;
              footTraceLiftNormal = 1.0f;
              entityBoundsMinZ = 0.0f;
              footTraceLiftTurret = 8.0f;
              footTraceEndOffsetZMoving = 24.0f;
              footTraceEndOffsetZIdle[1] = 30.0f;
              footTraceEndOffsetZIdle[0] = 0.0f;
              maxPelvisReachAdjust = FLOAT_40_0;
              minPelvisHeightNormal = 20.0f;
              footInAirLiftReductionMaxInAir = 20.0f;
              minPelvisHeightCrouching = -0.5f;
              minPelvisHeightTurret = 16.0f;
              maxFootSpan = 6.0f;
              maxFootDropIdle = FLOAT_13_0;
              maxFootDropIdleCr = 16.0f;
              maxFootDropMoving = 18.0f;
              maxFootRaiseStanding = 12.0f;
              maxFootRaiseCrouching = 15.0f;
              maxFootRaiseTurret = 30.0f;
              frametimeScalar = FLOAT_17_0;
              v248 = FLOAT_17_0;
              footTraceEndOffsetZ = FLOAT_27_0;
              v246 = 1.0f;
              v245 = 0;
              originChange[2] = *(&minPelvisHeightNormal + v280);
              ikCalcBoneModelMatrix(ikState, 1, (*ikState->matArrayPostIK)[0], (float (*)[4])v281);
              LODWORD(originChange[1]) = ikState->lastTerrainMappingOrigin;
              LODWORD(originChange[0]) = ikState->entityXform[3];
              v241 = ikState->entityXform[3][0] - ikState->lastTerrainMappingOrigin[0];
              v242 = ikState->entityXform[3][1] - ikState->lastTerrainMappingOrigin[1];
              v243 = ikState->entityXform[3][2] - ikState->lastTerrainMappingOrigin[2];
              v240 = (float)((float)((float)(v241 * v241) + (float)(v242 * v242)) + (float)(v243 * v243)) < 4096.0;
              HIBYTE(velSpeed) = (float)((float)((float)(v241 * v241) + (float)(v242 * v242)) + (float)(v243 * v243)) < 4096.0;
              if ( IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE > frametimeCap )
              {
                if ( IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE > frametimeResetCap )
                {
                  lastTerrainMappingOrigin = ikState->lastTerrainMappingOrigin;
                  v237 = ikState->entityXform[3];
                  ikState->lastTerrainMappingOrigin[0] = ikState->entityXform[3][0];
                  lastTerrainMappingOrigin[1] = v237[1];
                  lastTerrainMappingOrigin[2] = v237[2];
                  ikState->lastTerrainMappingFootOffsetZ[0] = 0.0f;
                  ikState->lastTerrainMappingFootOffsetZ[1] = 0.0f;
                  ikState->lastTerrainMappingPelvisZ = *(float *)&v281[56] + ikState->entityXform[3][2];
                }
                IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE = frametimeCapTime;
                IK_TERRAIN_MAPPING_LERP_SCALE = frametimeCapTime;
                HIBYTE(velSpeed) = 0;
              }
              velSpeedScale = Abs(ikState->velocity);
              velTimeScalar = velSpeedScale / IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX;
              if ( (float)(1.0 - (float)(velSpeedScale / IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX)) < 0.0 )
                v234 = 1.0f;
              else
                v234 = velTimeScalar;
              maxFootDrop = v234;
              v232 = 1.0f;
              v232 = (float)((float)(footTraceOffsetFromAnkle[v280 + 1] - footTraceLiftNormal) * v234)
                   + footTraceLiftNormal;
              originChange[2] = (float)((float)(1.0 - v234) * *(&minPelvisHeightNormal + v280))
                              + (float)(maxPelvisReachAdjust * v234);
              maxFootRaise = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
              v230 = (float)((float)(1.0 - maxFootDrop)
                           * (float)((float)((float)(1.0 - maxFootRaise) * maxFootRaiseStanding)
                                   + (float)(maxFootRaiseCrouching * maxFootRaise)))
                   + (float)(maxFootRaiseTurret * maxFootDrop);
              v229 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
              minPelvisHeight = IKImport_GetLayerLerp(ikState, IKLAYER_TURRET_OFFSET);
              v228 = (float)((float)(1.0 - minPelvisHeight)
                           * (float)((float)((float)(1.0 - v229) * frametimeScalar) + (float)(v248 * v229)))
                   + (float)(footTraceEndOffsetZ * minPelvisHeight);
              v226 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
              footTraceLift = IKImport_GetLayerLerp(ikState, IKLAYER_TURRET_OFFSET);
              v224 = (float)((float)((float)((float)(1.0 - v226) * maxFootSpan) + (float)(maxFootDropIdle * v226))
                           * (float)(1.0 - footTraceLift))
                   + (float)(maxFootDropIdleCr * footTraceLift);
              v223 = IKImport_GetLayerLerp(ikState, IKLAYER_TURRET_OFFSET);
              *(float *)&i = (float)((float)(1.0 - v223) * footTraceEndOffsetZMoving)
                           + (float)(footTraceEndOffsetZIdle[1] * v223);
              v221 = ikState->lastTerrainMappingOrigin;
              v220 = ikState->entityXform[3];
              ikState->lastTerrainMappingOrigin[0] = ikState->entityXform[3][0];
              v221[1] = v220[1];
              v221[2] = v220[2];
              for ( j = 0; j < 2; ++j )
              {
                v218 = ikState->footMats[j][0];
                v217 = v218;
                v207 = *v218;
                v208 = ikState->footMats[j][0][1];
                v209 = ikState->footMats[j][0][2];
                *(float *)&v210 = ikState->footMats[j][0][3];
                v206 = ikState->footMats[j][1];
                *((float *)&v210 + 1) = *v206;
                v211 = *(_QWORD *)&ikState->footMats[j][1][1];
                *(float *)&v212 = ikState->footMats[j][1][3];
                v205 = ikState->footMats[j][2];
                *((float *)&v212 + 1) = *v205;
                v213 = *(_QWORD *)&ikState->footMats[j][2][1];
                *(float *)&v214 = ikState->footMats[j][2][3];
                v204 = ikState->footMats[j][3];
                *((float *)&v214 + 1) = *v204;
                v215 = *(_QWORD *)&ikState->footMats[j][3][1];
                v216 = ikState->footMats[j][3][3];
                v201 = v202;
                *(float *)v202 = v207;
                *(float *)&v202[1] = v208;
                *(float *)&v202[2] = v209;
                *(_QWORD *)&tmpMat[0][0] = v210;
                LODWORD(worldFootMats[1][3][3]) = &tmpMat[0][1];
                *(_QWORD *)&tmpMat[0][2] = v211;
                *(_QWORD *)&tmpMat[1][0] = v212;
                LODWORD(worldFootMats[1][3][2]) = &tmpMat[1][1];
                *(_QWORD *)&tmpMat[1][2] = v213;
                *(_QWORD *)&tmpMat[2][0] = v214;
                LODWORD(worldFootMats[1][3][1]) = &tmpMat[2][1];
                *(_QWORD *)&tmpMat[2][2] = v215;
                tmpMat[3][0] = v216;
                v198 = (float *)&v199[64 * j];
                v197 = ikState->entityLegsXform[0];
                v196 = v202;
                v186 = v207;
                v187 = v208;
                v188 = v209;
                v189 = v210;
                v185 = &tmpMat[0][1];
                v190 = v211;
                v191 = v212;
                v184 = &tmpMat[1][1];
                v192 = v213;
                v193 = v214;
                v183 = &tmpMat[2][1];
                v194 = v215;
                v195 = v216;
                v182 = ikState->entityLegsXform[0];
                v174 = *(_QWORD *)&ikState->entityLegsXform[0][0];
                v175 = *(_QWORD *)&ikState->entityLegsXform[0][2];
                v173 = ikState->entityLegsXform[1];
                v176 = *(_QWORD *)&ikState->entityLegsXform[1][0];
                v177 = *(_QWORD *)&ikState->entityLegsXform[1][2];
                v172 = ikState->entityLegsXform[2];
                v178 = *(_QWORD *)&ikState->entityLegsXform[2][0];
                v179 = *(_QWORD *)&ikState->entityLegsXform[2][2];
                v171 = ikState->entityLegsXform[3];
                v180 = *(_QWORD *)&ikState->entityLegsXform[3][0];
                v181 = *(_QWORD *)&ikState->entityLegsXform[3][2];
                v155 = (float)((float)((float)(v207 * *(float *)&v174) + (float)(v208 * *(float *)&v176))
                             + (float)(v209 * *(float *)&v178))
                     + (float)(*(float *)&v210 * *(float *)&v180);
                v156 = (float)((float)((float)(v207 * *((float *)&v174 + 1)) + (float)(v208 * *((float *)&v176 + 1)))
                             + (float)(v209 * *((float *)&v178 + 1)))
                     + (float)(*(float *)&v210 * *((float *)&v180 + 1));
                v157 = (float)((float)((float)(v207 * *(float *)&v175) + (float)(v208 * *(float *)&v177))
                             + (float)(v209 * *(float *)&v179))
                     + (float)(*(float *)&v210 * *(float *)&v181);
                v158 = (float)((float)((float)(v207 * *((float *)&v175 + 1)) + (float)(v208 * *((float *)&v177 + 1)))
                             + (float)(v209 * *((float *)&v179 + 1)))
                     + (float)(*(float *)&v210 * *((float *)&v181 + 1));
                v159 = (float)((float)((float)(*((float *)&v210 + 1) * *(float *)&v174)
                                     + (float)(*(float *)&v211 * *(float *)&v176))
                             + (float)(*((float *)&v211 + 1) * *(float *)&v178))
                     + (float)(*(float *)&v212 * *(float *)&v180);
                v160 = (float)((float)((float)(*((float *)&v210 + 1) * *((float *)&v174 + 1))
                                     + (float)(*(float *)&v211 * *((float *)&v176 + 1)))
                             + (float)(*((float *)&v211 + 1) * *((float *)&v178 + 1)))
                     + (float)(*(float *)&v212 * *((float *)&v180 + 1));
                v161 = (float)((float)((float)(*((float *)&v210 + 1) * *(float *)&v175)
                                     + (float)(*(float *)&v211 * *(float *)&v177))
                             + (float)(*((float *)&v211 + 1) * *(float *)&v179))
                     + (float)(*(float *)&v212 * *(float *)&v181);
                v162 = (float)((float)((float)(*((float *)&v210 + 1) * *((float *)&v175 + 1))
                                     + (float)(*(float *)&v211 * *((float *)&v177 + 1)))
                             + (float)(*((float *)&v211 + 1) * *((float *)&v179 + 1)))
                     + (float)(*(float *)&v212 * *((float *)&v181 + 1));
                v163 = (float)((float)((float)(*((float *)&v212 + 1) * *(float *)&v174)
                                     + (float)(*(float *)&v213 * *(float *)&v176))
                             + (float)(*((float *)&v213 + 1) * *(float *)&v178))
                     + (float)(*(float *)&v214 * *(float *)&v180);
                v164 = (float)((float)((float)(*((float *)&v212 + 1) * *((float *)&v174 + 1))
                                     + (float)(*(float *)&v213 * *((float *)&v176 + 1)))
                             + (float)(*((float *)&v213 + 1) * *((float *)&v178 + 1)))
                     + (float)(*(float *)&v214 * *((float *)&v180 + 1));
                v165 = (float)((float)((float)(*((float *)&v212 + 1) * *(float *)&v175)
                                     + (float)(*(float *)&v213 * *(float *)&v177))
                             + (float)(*((float *)&v213 + 1) * *(float *)&v179))
                     + (float)(*(float *)&v214 * *(float *)&v181);
                v166 = (float)((float)((float)(*((float *)&v212 + 1) * *((float *)&v175 + 1))
                                     + (float)(*(float *)&v213 * *((float *)&v177 + 1)))
                             + (float)(*((float *)&v213 + 1) * *((float *)&v179 + 1)))
                     + (float)(*(float *)&v214 * *((float *)&v181 + 1));
                v167 = (float)((float)((float)(*((float *)&v214 + 1) * *(float *)&v174)
                                     + (float)(*(float *)&v215 * *(float *)&v176))
                             + (float)(*((float *)&v215 + 1) * *(float *)&v178))
                     + (float)(v216 * *(float *)&v180);
                v168 = (float)((float)((float)(*((float *)&v214 + 1) * *((float *)&v174 + 1))
                                     + (float)(*(float *)&v215 * *((float *)&v176 + 1)))
                             + (float)(*((float *)&v215 + 1) * *((float *)&v178 + 1)))
                     + (float)(v216 * *((float *)&v180 + 1));
                v169 = (float)((float)((float)(*((float *)&v214 + 1) * *(float *)&v175)
                                     + (float)(*(float *)&v215 * *(float *)&v177))
                             + (float)(*((float *)&v215 + 1) * *(float *)&v179))
                     + (float)(v216 * *(float *)&v181);
                v170 = (float)((float)((float)(*((float *)&v214 + 1) * *((float *)&v175 + 1))
                                     + (float)(*(float *)&v215 * *((float *)&v177 + 1)))
                             + (float)(*((float *)&v215 + 1) * *((float *)&v179 + 1)))
                     + (float)(v216 * *((float *)&v181 + 1));
                v154 = v198;
                v2 = v198;
                *v198 = v155;
                v2[1] = v156;
                v2[2] = v157;
                v2[3] = v158;
                lowestGroundZ = v198 + 4;
                v3 = v198 + 4;
                v198[4] = v159;
                v3[1] = v160;
                v3[2] = v161;
                v3[3] = v162;
                highestGroundZ = v198 + 8;
                v4 = v198 + 8;
                v198[8] = v163;
                v4[1] = v164;
                v4[2] = v165;
                v4[3] = v166;
                maxPelvisReach = v198 + 12;
                v5 = v198 + 12;
                v198[12] = v167;
                v5[1] = v168;
                v5[2] = v169;
                v5[3] = v170;
              }
              terrainZ = FLOAT_9999999_0;
              groundOffsets[1] = FLOAT_N9999999_0;
              groundOffsets[0] = (float)((float)(ikState->jointVars[2].LowerLength + ikState->jointVars[2].UpperLength)
                                       + ikState->footHeightFromGround)
                               + minPelvisHeightCrouching;
              for ( j = 0; j < 2; ++j )
              {
                IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE = IK_TERRAIN_MAPPING_LERP_SCALE;
                modifiedFeet = 0.0f;
                endPos[j + 2] = 0.0f;
                v246 = 1.0f;
                *((_BYTE *)&endPos[1] + j) = 0;
                endPos[0] = 3.0f;
                if ( IKImport_GetVar_IK_Debug() == 1 )
                  IKImport_DrawLine(&footTraceNormals[1][1], startPos, j);
                footTraceNormals[1][0] = Abs(ikState->lastFootTraceGoalNormals[j]) - 1.0;
                if ( fabs(footTraceNormals[1][0]) >= 0.1 )
                {
                  LODWORD(footTraceCacheDistThreshold) = &v143[3 * j];
                  *(unsigned int *)LODWORD(footTraceCacheDistThreshold) = 0;
                  *(unsigned int *)(LODWORD(footTraceCacheDistThreshold) + 4) = 0;
                  *(float *)(LODWORD(footTraceCacheDistThreshold) + 8) = 1.0f;
                  v139 = ikState->lastFootTraceGoalNormals[j];
                  v138 = (float *)&v143[3 * j];
                  *v139 = *v138;
                  v139[1] = v138[1];
                  v139[2] = v138[2];
                }
                else
                {
                  v142 = (float *)&v143[3 * j];
                  v141 = ikState->lastFootTraceGoalNormals[j];
                  *v142 = *v141;
                  v142[1] = v141[1];
                  v142[2] = v141[2];
                }
                v137 = 0.2f;
                v136 = &worldFootMats[j][2][1];
                v135 = ikState->lastTerrainMappingTraceFootPos[j];
                *(float *)&n = *v136 - *v135;
                maxTraceZ = v136[1] - ikState->lastTerrainMappingTraceFootPos[j][1];
                v6 = Vec2Length((const float *)&n);
                if ( v6 <= v137 )
                {
                  v101 = &worldFootMats[j][2][1];
                  realZ = ikState->lastTerrainMappingTraceFootPos[j];
                  terrainDiffNoLerpingThreshold = *v101 - *realZ;
                  terrainDiff = v101[1] - ikState->lastTerrainMappingTraceFootPos[j][1];
                  v10 = Vec2Length(&terrainDiffNoLerpingThreshold);
                  if ( v137 < v10 )
                    v11 = ikState->lastTerrainMappingFootOffsetZ[j];
                  else
                    v11 = ikState->lastTerrainMappingTraceZ[j];
                  modifiedFeet = v11;
                }
                else
                {
                  v132 = 0;
                  v131 = FLOAT_N99999_0;
                  for ( k = 0; k < 2; ++k )
                  {
                    LODWORD(thisNormal[2]) = &worldFootMats[j][0][1];
                    thisNormal[1] = (float)(2 * (j == 0) - 1) * *(&entityBoundsMinZ + k);
                    LODWORD(thisNormal[0]) = &worldFootMats[j][2][1];
                    footTraceNormals[1][1] = (float)(thisNormal[1] * *(float *)LODWORD(thisNormal[2]))
                                           + *(float *)LODWORD(thisNormal[0]);
                    footTraceNormals[1][2] = (float)(thisNormal[1] * *(float *)(LODWORD(thisNormal[2]) + 4))
                                           + *(float *)(LODWORD(thisNormal[0]) + 4);
                    v145 = (float)(thisNormal[1] * *(float *)(LODWORD(thisNormal[2]) + 8))
                         + *(float *)(LODWORD(thisNormal[0]) + 8);
                    v145 = ikState->entityXform[3][2] + *(float *)&i;
                    *(_QWORD *)startPos = *(_QWORD *)&footTraceNormals[1][1];
                    startPos[2] = v145;
                    startPos[2] = (float)((float)(ikState->entityXform[3][2] - footTraceEndOffsetZIdle[0])
                                        - ikState->footHeightFromGround)
                                - originChange[2];
                    if ( IKImport_TraceBox(
                           ikState,
                           &footTraceNormals[1][1],
                           startPos,
                           endPos[0],
                           tracePos,
                           thisTracePos,
                           k + 2 * j,
                           0,
                           1) )
                    {
                      if ( tracePos[2] > v131 )
                      {
                        v124 = tracePos[0];
                        v125 = tracePos[1];
                        v126 = tracePos[2];
                        traceFootAdjust = Abs(thisTracePos) - 1.0;
                        if ( fabs(traceFootAdjust) < 0.1 )
                        {
                          LODWORD(lowerFootTerrainSteepnessScale) = &v143[3 * j];
                          *(unsigned int *)LODWORD(lowerFootTerrainSteepnessScale) = LODWORD(thisTracePos[0]);
                          *(float *)(LODWORD(lowerFootTerrainSteepnessScale) + 4) = thisTracePos[1];
                          *(float *)(LODWORD(lowerFootTerrainSteepnessScale) + 8) = thisTracePos[2];
                          LODWORD(lowerFootTerrainDotScale) = ikState->lastFootTraceGoalNormals[j];
                          *(unsigned int *)LODWORD(lowerFootTerrainDotScale) = LODWORD(thisTracePos[0]);
                          *(float *)(LODWORD(lowerFootTerrainDotScale) + 4) = thisTracePos[1];
                          *(float *)(LODWORD(lowerFootTerrainDotScale) + 8) = thisTracePos[2];
                        }
                        v131 = tracePos[2];
                      }
                      v132 = 1;
                    }
                  }
                  if ( v132 )
                  {
                    lowerFootTerrainDotClampMin = 0.0f;
                    modifiedFeet = (float)(v126 - endPos[0]) + 0.0;
                    lowerFootTerrainDotClampMax = -1.0f;
                    v118 = 5.0f;
                    v117 = 0.0f;
                    footDirectionToNormalDot = 1.0f;
                    v115 = &worldFootMats[j][0][1];
                    v114 = ikState->lastFootTraceNormals[j];
                    LODWORD(v113) = COERCE_UNSIGNED_INT((float)(*v114 * *v115) + (float)(ikState->lastFootTraceNormals[j][1]
                                                                                       * v115[1]))
                                  ^ _mask__NegFloat_;
                    if ( j == 1 )
                      LODWORD(v113) ^= _mask__NegFloat_;
                    if ( (float)(v113 - footDirectionToNormalDot) < 0.0 )
                      v7 = v113;
                    else
                      v7 = footDirectionToNormalDot;
                    v112 = v7;
                    if ( (float)(v117 - v113) < 0.0 )
                      v8 = v112;
                    else
                      v8 = v117;
                    start[2] = v8;
                    v113 = v8;
                    v9 = Vec2Length(ikState->lastFootTraceNormals[j]);
                    modifiedFeet = modifiedFeet - v9 * lowerFootTerrainDotClampMax;
                    modifiedFeet = modifiedFeet - (float)(v113 * v118);
                    if ( ikState->footMats[j][3][2] > ikState->footHeightFromGround
                      && modifiedFeet > ikState->entityXform[3][2] )
                    {
                      start[1] = (float)(modifiedFeet - ikState->entityXform[3][2]) / minPelvisHeightTurret;
                      if ( start[1] <= 1.0 )
                        start[0] = start[1];
                      else
                        start[0] = 1.0f;
                      modifiedFeet = modifiedFeet
                                   - (float)((float)(ikState->footMats[j][3][2] - ikState->footHeightFromGround)
                                           * start[0]);
                    }
                    ikState->lastTerrainMappingTraceZ[j] = modifiedFeet;
                    if ( IKImport_GetVar_IK_Debug() == 1 )
                    {
                      end[2] = v126;
                      v109 = v126;
                      end[0] = v124 + 4.0;
                      end[1] = v125 + 4.0;
                      v107 = v124 - 4.0;
                      v108 = v125 - 4.0;
                      IKImport_DrawLine(end, &v107, 3);
                      end[2] = v126;
                      v109 = v126;
                      end[0] = v124 - 4.0;
                      end[1] = v125 + 4.0;
                      v107 = v124 + 4.0;
                      v108 = v125 - 4.0;
                      IKImport_DrawLine(end, &v107, 3);
                    }
                    if ( ikState->lerpInfo[3].goalState )
                    {
                      v106 = ikState->lastTerrainMappingTraceFootPos[j];
                      v105 = &worldFootMats[j][2][1];
                      *v106 = *v105;
                      v106[1] = v105[1];
                    }
                  }
                  if ( !v132 )
                  {
                    modifiedFeet = ikState->entityXform[3][2];
                    ikState->lastTerrainMappingTraceZ[j] = modifiedFeet;
                    v104 = &v143[3 * j];
                    *v104 = 0;
                    v104[1] = 0;
                    v104[2] = LODWORD(1.0f);
                    v103 = ikState->lastFootTraceGoalNormals[j];
                    v102 = (float *)&v143[3 * j];
                    *v103 = *v102;
                    v103[1] = v102[1];
                    v103[2] = v102[2];
                  }
                }
                if ( (float)(ikState->entityXform[3][2] - v230) > modifiedFeet )
                  modifiedFeet = ikState->entityXform[3][2] - v230;
                if ( modifiedFeet > (float)(ikState->entityXform[3][2] + v228) )
                  modifiedFeet = ikState->entityXform[3][2] + v228;
                if ( HIBYTE(velSpeed) )
                {
                  fastLerpScalar[1] = modifiedFeet;
                  fastLerpScalar[0] = modifiedFeet - ikState->lastTerrainMappingFootOffsetZ[j];
                  *(float *)&v96[2] = 128.0f;
                  if ( fabs(fastLerpScalar[0]) < 128.0 )
                  {
                    v246 = v232;
                    if ( v245 )
                    {
                      *(float *)v96 = 5.0f;
                      *(float *)&v96[1] = 10.0f;
                      v246 = v246 + *(float *)&v96[v280];
                    }
                    else if ( fabs(fastLerpScalar[1] - modifiedFeet) > IK_TERRAIN_MAPPING_VELOCITY_SCALAR[0] )
                    {
                      v95 = (float)((float)(fabs(fastLerpScalar[1] - modifiedFeet)
                                          / IK_TERRAIN_MAPPING_VELOCITY_SCALAR[0])
                                  * 3.0)
                          + 1.0;
                      if ( v246 <= v95 )
                        v12 = v95;
                      else
                        v12 = v246;
                      v94 = v12;
                      v246 = v246 + v12;
                    }
                    if ( !v245 && v246 > footTraceOffsetFromAnkle[0] )
                      v246 = footTraceOffsetFromAnkle[0];
                    IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE = IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE * v246;
                    fastLerpScalar[0] = (float)(IK_TERRAIN_MAPPING_VELOCITY_SCALAR[1]
                                              * IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE)
                                      * fastLerpScalar[0];
                    if ( fastLerpScalar[0] <= 0.0 )
                    {
                      if ( fastLerpScalar[0] > (float)(COERCE_FLOAT(LODWORD(IK_TERRAIN_MAPPING_VELOCITY_MAX) ^ _mask__NegFloat_)
                                                     * IK_TERRAIN_MAPPING_LERP_SCALE) )
                        fastLerpScalar[0] = COERCE_FLOAT(LODWORD(IK_TERRAIN_MAPPING_VELOCITY_MAX) ^ _mask__NegFloat_)
                                          * IK_TERRAIN_MAPPING_LERP_SCALE;
                      footOffsetAdjusted = ikState->lastTerrainMappingFootOffsetZ[j] + fastLerpScalar[0];
                      if ( fastLerpScalar[1] <= footOffsetAdjusted )
                        v14 = footOffsetAdjusted;
                      else
                        v14 = fastLerpScalar[1];
                      minScale = v14;
                      modifiedFeet = v14;
                    }
                    else
                    {
                      if ( (float)(IK_TERRAIN_MAPPING_CLAMP * IK_TERRAIN_MAPPING_LERP_SCALE) > fastLerpScalar[0] )
                        fastLerpScalar[0] = IK_TERRAIN_MAPPING_CLAMP * IK_TERRAIN_MAPPING_LERP_SCALE;
                      v93 = ikState->lastTerrainMappingFootOffsetZ[j] + fastLerpScalar[0];
                      if ( v93 <= fastLerpScalar[1] )
                        v13 = v93;
                      else
                        v13 = fastLerpScalar[1];
                      footOffsetFromStandard = v13;
                      modifiedFeet = v13;
                    }
                  }
                }
                v89 = modifiedFeet - ikState->entityXform[3][2];
                v88 = v89;
                if ( v89 < 0.0 )
                {
                  v87 = 1.0f;
                  v15 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
                  *(float *)&v86 = v15 + 1.0 - maxFootDrop;
                  if ( (float)(*(float *)&v86 - 1.0) < 0.0 )
                    lowerFoot = v86;
                  else
                    *(float *)&lowerFoot = 1.0f;
                  if ( (float)(v87 - *(float *)&v86) < 0.0 )
                    v16 = *(float *)&lowerFoot;
                  else
                    v16 = v87;
                  lowerAdjust = v16;
                  v88 = v88 * v16;
                }
                ikState->footMats[j][3][2] = (float)(*(float *)&v281[68] * v88) + ikState->footMats[j][3][2];
                if ( terrainZ > modifiedFeet )
                  terrainZ = modifiedFeet;
                if ( modifiedFeet > groundOffsets[1] )
                  groundOffsets[1] = modifiedFeet;
                ikState->lastTerrainMappingFootOffsetZ[j] = modifiedFeet;
              }
              pelvisMoveScale = ikState->lastTerrainMappingFootOffsetZ[0] - ikState->lastTerrainMappingFootOffsetZ[1];
              if ( fabs(pelvisMoveScale) > maxFootDropMoving )
              {
                pelvisForceZAdjust = ikState->lastTerrainMappingFootOffsetZ[0] > ikState->lastTerrainMappingFootOffsetZ[1];
                v81 = (float)((float)(ikState->lastTerrainMappingFootOffsetZ[!pelvisForceZAdjust]
                                    - ikState->lastTerrainMappingFootOffsetZ[pelvisForceZAdjust])
                            - maxFootDropMoving)
                    * *(float *)&v281[68];
                ikState->footMats[pelvisForceZAdjust][3][2] = ikState->footMats[pelvisForceZAdjust][3][2] + v81;
                ikState->lastTerrainMappingFootOffsetZ[pelvisForceZAdjust] = ikState->lastTerrainMappingFootOffsetZ[pelvisForceZAdjust]
                                                                           + v81;
                terrainZ = terrainZ + v81;
              }
              v80 = 0.0f;
              v79 = 0.0f;
              v78 = 0.75f;
              pelvisMovingZAdjust = IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING);
              if ( (float)(pelvisMovingZAdjust - 1.0) < 0.0 )
                zOfs = pelvisMovingZAdjust;
              else
                zOfs = 1.0f;
              if ( (float)(v78 - pelvisMovingZAdjust) < 0.0 )
                v17 = zOfs;
              else
                v17 = v78;
              PELVIS_ABS_SCALAR = v17;
              v80 = v17;
              realPelvisZ = 0.0f;
              v79 = (1.0 - IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING)) * 0.0;
              if ( IK_IsCalcBone(ikState, 1) )
              {
                changeZ = 0.0f;
                v72 = 8.0f;
                if ( *(float *)&v281[56] <= (float)((float)(terrainZ + groundOffsets[0]) - ikState->entityXform[3][2]) )
                {
                  if ( (float)((float)(groundOffsets[1] + v224) - ikState->entityXform[3][2]) <= *(float *)&v281[56] )
                    v18 = *(float *)&v281[56] + ikState->entityXform[3][2];
                  else
                    v18 = groundOffsets[1] + v224;
                  v71 = v18;
                }
                else
                {
                  v71 = terrainZ + groundOffsets[0];
                }
                terrainLegExtendAdjust = v71 - ikState->lastTerrainMappingPelvisZ;
                if ( HIBYTE(velSpeed) )
                {
                  v69 = (float)((float)(IK_TERRAIN_MAPPING_LERP_SCALE * v232) * v80) * v72;
                  if ( (float)(1.0 - v69) < 0.0 )
                    v68 = 1.0f;
                  else
                    v68 = v69;
                  terrainLegExtendAdjust = terrainLegExtendAdjust * v68;
                }
                changeZ = (float)(terrainLegExtendAdjust + ikState->lastTerrainMappingPelvisZ)
                        - (float)(*(float *)&v281[56] + ikState->entityXform[3][2]);
                ikState->lastTerrainMappingPelvisZ = ikState->lastTerrainMappingPelvisZ + terrainLegExtendAdjust;
                changeZ = changeZ * *(float *)&v281[68];
                (*ikState->matArrayPostIK)[7][2] = (*ikState->matArrayPostIK)[7][2] + changeZ;
                if ( !IK_GetStatus(ikState, IKSTATUS_MOUNTED_TURRET) )
                {
                  ikState->handMats[1][3][2] = ikState->handMats[1][3][2] + changeZ;
                  ikState->handMats[0][3][2] = ikState->handMats[0][3][2] + changeZ;
                }
                ikState->bHasActiveLayers = 1;
                if ( ikState->cacheActive )
                {
                  memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                  ikState->cacheActive = 0;
                }
                ikState->modifiedIKBones |= 2u;
              }
              *(float *)&v67[72] = 0.5f;
              if ( !ikState->lerpInfo[1].goalState )
              {
                for ( j = 0; j < 2; ++j )
                {
                  ikCalcBoneModelMatrix(
                    ikState,
                    ikState->localIkSystem->joints[j + 2].parentBone,
                    (*ikState->matArrayPostIK)[0],
                    (float (*)[4])v67);
                  v66 = ikState->footMats[j][3];
                  aSq = &v67[48];
                  v63 = *v66 - *(float *)&v67[48];
                  cSq = ikState->footMats[j][3][1] - *(float *)&v67[52];
                  v62 = (float)(v63 * v63) + (float)(cSq * cSq);
                  v61 = Vec3DistanceSq((const float *)&v67[48], v66);
                  cMaxSq = *(float *)&v67[56] - ikState->footMats[j][3][2];
                  LODWORD(bSq) = LODWORD(cMaxSq) & _mask__AbsFloat_;
                  if ( (float)(fabs(cMaxSq)
                             * fabs(cMaxSq)) > v62 )
                  {
                    v58 = (float)(ikState->jointVars[j + 2].LowerLength + ikState->jointVars[j + 2].UpperLength)
                        - *(float *)&v67[72];
                    terrainNormalLerpTime = v58 * v58;
                    if ( v61 > (float)(v58 * v58) )
                    {
                      terrainNormalRangeCap = terrainNormalLerpTime - v62;
                      if ( (float)(terrainNormalLerpTime - v62) > 0.1 )
                      {
                        terrainNormalMinZ = sqrtf(terrainNormalRangeCap);
                        ikState->footMats[j][3][2] = *(float *)&v67[56] - terrainNormalMinZ;
                      }
                    }
                  }
                }
              }
              v_up[2] = 0.2f;
              v_up[1] = 0.4f;
              v_up[0] = 0.2f;
              for ( j = 0; j < 2; ++j )
              {
                v51 = 0.0f;
                v52 = 0;
                angularMappingHeightOffsetMax = 1.0f;
                v50 = 4.0f;
                footOffsetAngularScale = (float)(ikState->footMats[j][3][2] - ikState->footHeightFromGround) / 4.0;
                if ( (float)(footOffsetAngularScale - 1.0) < 0.0 )
                  v48 = footOffsetAngularScale;
                else
                  v48 = 1.0f;
                if ( (float)(0.0 - footOffsetAngularScale) < 0.0 )
                  angularWeight = v48;
                else
                  angularWeight = 0.0f;
                v46 = 1.0 - angularWeight;
                if ( ikState->lerpInfo[3].goalState )
                  fraction = 1.0f;
                else
                  fraction = v46;
                v44 = fraction;
                Vec3Lerp(&v51, (const float *)&v143[3 * j], fraction, (float *)&v143[3 * j]);
                if ( v_up[0] > *(float *)&v143[3 * j + 2] )
                {
                  v43 = &v143[3 * j];
                  *v43 = LODWORD(v51);
                  v43[1] = v52;
                  v43[2] = LODWORD(angularMappingHeightOffsetMax);
                }
                if ( !HIBYTE(velSpeed)
                  || ((v42 = ikState->lastFootTraceNormals[j], *v42 != 0.0) || v42[1] != 0.0 || v42[2] != 0.0
                    ? (v41 = 0)
                    : (v41 = 1),
                      v41
                   || (v40 = Abs(ikState->lastFootTraceNormals[j]) - 1.0,
                       fabs(v40) >= 0.1)) )
                {
                  Vec3NormalizeTo((const float *)&v143[3 * j], ikState->lastFootTraceNormals[j]);
                }
                else
                {
                  XYdist = IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE / v_up[2];
                  if ( (float)((float)(IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE / v_up[2]) - 1.0) < 0.0 )
                    v38 = XYdist;
                  else
                    v38 = 1.0f;
                  if ( (float)(0.0 - XYdist) < 0.0 )
                    v37 = v38;
                  else
                    v37 = 0.0f;
                  Vec3Lerp(
                    ikState->lastFootTraceNormals[j],
                    (const float *)&v143[3 * j],
                    v37,
                    ikState->lastFootTraceNormals[j]);
                  Vec3Normalize(ikState->lastFootTraceNormals[j]);
                }
                v36 = Vec2Length(ikState->lastFootTraceNormals[j]);
                if ( v36 > v_up[1] )
                {
                  v35 = ikState->lastFootTraceNormals[j];
                  v34 = v_up[1] / v36;
                  v33 = v35;
                  *v35 = (float)(v_up[1] / v36) * *v35;
                  v35[1] = v34 * v33[1];
                  ikState->lastFootTraceNormals[j][2] = sqrtf(1.0 - (float)(v_up[1] * v_up[1]));
                  Vec3Normalize(ikState->lastFootTraceNormals[j]);
                }
                if ( *(float *)&v281[68] >= 1.0 )
                {
                  *(unsigned int *)&v29[68] = ikState->lastFootTraceNormals[j];
                  *(unsigned int *)&v29[72] = **(unsigned int **)&v29[68];
                  v30 = ikState->lastFootTraceNormals[j][1];
                  v31 = ikState->lastFootTraceNormals[j][2];
                }
                else
                {
                  v32[0] = 0.0f;
                  v32[1] = 0.0f;
                  v32[2] = 1.0f;
                  Vec3Lerp(v32, ikState->lastFootTraceNormals[j], *(float *)&v281[68], (float *)&v29[72]);
                  Vec3Normalize((float *)&v29[72]);
                }
                ikMatrixIdentity44((float (*)[4])v29);
                v28 = ikState->entityLegsXform[0];
                if ( (_BYTE *)ikState->entityLegsXform == v27
                  && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 125, 0, "%s", "in != out") )
                {
                  __debugbreak();
                }
                *(float *)v27 = *v28;
                *(float *)&v27[4] = v28[4];
                *(float *)&v27[8] = v28[8];
                *(float *)&v27[12] = v28[12];
                *(float *)&v27[16] = v28[1];
                *(float *)&v27[20] = v28[5];
                *(float *)&v27[24] = v28[9];
                *(float *)&v27[28] = v28[13];
                *(float *)&v27[32] = v28[2];
                *(float *)&v27[36] = v28[6];
                *(float *)&v27[40] = v28[10];
                *(float *)&v27[44] = v28[14];
                *(float *)&v27[48] = v28[3];
                *(float *)&v27[52] = v28[7];
                *(float *)&v27[56] = v28[11];
                *(float *)&v27[60] = v28[15];
                ikMatrixTransformVector34((const float *)&v29[72], (const float (*)[4])v27, (float *)&v29[32]);
                *(float *)v29 = ikIdentityMatrix44[0][0];
                *(unsigned int *)&v29[4] = 0;
                *(unsigned int *)&v29[8] = 0;
                Vec3Cross((const float *)&v29[32], (const float *)v29, (float *)&v29[16]);
                Vec3Normalize((float *)&v29[16]);
                Vec3Cross((const float *)&v29[16], (const float *)&v29[32], (float *)v29);
                Vec3Normalize((float *)v29);
                v25 = ikState->footMats[j][1][0];
                v26 = ikState->footMats[j][2][0];
                v20 = ikState->footMats[j][0][0];
                v21 = ikState->footMats[j][0][1];
                v22 = ikState->footMats[j][0][2];
                curFoot_8 = *(_QWORD *)&ikState->footMats[j][1][1];
                curFoot_24 = *(_QWORD *)&ikState->footMats[j][2][1];
                v19 = ikState->footMats[j][0];
                *v19 = (float)((float)(v20 * *(float *)v29) + (float)(v21 * *(float *)&v29[16]))
                     + (float)(v22 * *(float *)&v29[32]);
                v19[1] = (float)((float)(v20 * *(float *)&v29[4]) + (float)(v21 * *(float *)&v29[20]))
                       + (float)(v22 * *(float *)&v29[36]);
                v19[2] = (float)((float)(v20 * *(float *)&v29[8]) + (float)(v21 * *(float *)&v29[24]))
                       + (float)(v22 * *(float *)&v29[40]);
                v19[3] = 0.0f;
                v19[4] = (float)((float)(v25 * *(float *)v29) + (float)(*(float *)&curFoot_8 * *(float *)&v29[16]))
                       + (float)(*((float *)&curFoot_8 + 1) * *(float *)&v29[32]);
                v19[5] = (float)((float)(v25 * *(float *)&v29[4]) + (float)(*(float *)&curFoot_8 * *(float *)&v29[20]))
                       + (float)(*((float *)&curFoot_8 + 1) * *(float *)&v29[36]);
                v19[6] = (float)((float)(v25 * *(float *)&v29[8]) + (float)(*(float *)&curFoot_8 * *(float *)&v29[24]))
                       + (float)(*((float *)&curFoot_8 + 1) * *(float *)&v29[40]);
                v19[7] = 0.0f;
                v19[8] = (float)((float)(v26 * *(float *)v29) + (float)(*(float *)&curFoot_24 * *(float *)&v29[16]))
                       + (float)(*((float *)&curFoot_24 + 1) * *(float *)&v29[32]);
                v19[9] = (float)((float)(v26 * *(float *)&v29[4]) + (float)(*(float *)&curFoot_24 * *(float *)&v29[20]))
                       + (float)(*((float *)&curFoot_24 + 1) * *(float *)&v29[36]);
                v19[10] = (float)((float)(v26 * *(float *)&v29[8]) + (float)(*(float *)&curFoot_24 * *(float *)&v29[24]))
                        + (float)(*((float *)&curFoot_24 + 1) * *(float *)&v29[40]);
                v19[11] = 0.0f;
              }
            }
          }
        }
      }
    }
  }
  else
  {
    ikCalcBoneModelMatrix(ikState, 1, (*ikState->matArrayPostIK)[0], (float (*)[4])v281);
    ikState->lastTerrainMappingPelvisZ = *(float *)&v281[56] + ikState->entityXform[3][2];
  }
}

// local variable allocation has failed, the output may be wrong!
void  IK_Layer_ApplyFootIK(
        int a1@<ebp>,
        IKState *ikState,
        IKJointBones *jointBones,
        IKJointVars *jointVars,
        float (*childMat)[4])
{
  IKBoneNames childBone; // [esp-134h] [ebp-204h]
  float v6[5]; // [esp-130h] [ebp-200h] BYREF
  float v7[4]; // [esp-11Ch] [ebp-1ECh] BYREF
  float v8; // [esp-10Ch] [ebp-1DCh]
  float v9; // [esp-108h] [ebp-1D8h]
  float v10; // [esp-104h] [ebp-1D4h]
  int v11; // [esp-100h] [ebp-1D0h]
  float v12; // [esp-FCh] [ebp-1CCh]
  float v13; // [esp-F8h] [ebp-1C8h]
  float v14; // [esp-F4h] [ebp-1C4h]
  int v15; // [esp-F0h] [ebp-1C0h]
  float *v16; // [esp-D8h] [ebp-1A8h]
  IKBoneNames jointBone; // [esp-D4h] [ebp-1A4h]
  float v18[5]; // [esp-D0h] [ebp-1A0h] BYREF
  float v19[4]; // [esp-BCh] [ebp-18Ch] BYREF
  float v20; // [esp-ACh] [ebp-17Ch]
  float v21; // [esp-A8h] [ebp-178h]
  float v22; // [esp-A4h] [ebp-174h]
  int v23; // [esp-A0h] [ebp-170h]
  float v24; // [esp-9Ch] [ebp-16Ch]
  float v25; // [esp-98h] [ebp-168h]
  float v26; // [esp-94h] [ebp-164h]
  int v27; // [esp-90h] [ebp-160h]
  float *v28; // [esp-78h] [ebp-148h]
  IKBoneNames parentBone; // [esp-74h] [ebp-144h]
  float v30[5]; // [esp-70h] [ebp-140h] BYREF
  float v31[4]; // [esp-5Ch] [ebp-12Ch] BYREF
  float v32; // [esp-4Ch] [ebp-11Ch]
  float v33; // [esp-48h] [ebp-118h]
  float v34; // [esp-44h] [ebp-114h]
  int v35; // [esp-40h] [ebp-110h]
  float v36; // [esp-3Ch] [ebp-10Ch]
  float v37; // [esp-38h] [ebp-108h]
  float v38; // [esp-34h] [ebp-104h]
  int v39; // [esp-30h] [ebp-100h]
  float *v40; // [esp-10h] [ebp-E0h]
  _BYTE v41[188]; // [esp-Ch] [ebp-DCh] OVERLAPPED BYREF
  bool v43; // [esp+C3h] [ebp-Dh]
  unsigned int v44[2]; // [esp+C4h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+D0h] [ebp+0h]

  v44[0] = a1;
  v44[1] = retaddr;
  v43 = jointBones->parentBone == IKBONE_RTHIGH;
  if ( ikState->bJointVarsValid )
  {
    if ( ikState->ikBoneToObjBone[jointBones->childBone] != 161 )
    {
      if ( IK_IsCalcBone(ikState, jointBones->childBone) )
      {
        if ( v43 )
          IK_FlipHack(childMat);
        ikSolveLegJoint(
          (int)v44,
          ikState,
          jointBones,
          jointVars,
          &(*childMat)[12],
          (float (*)[4])&v41[64],
          (float (*)[4])&v41[128],
          v43);
        ikCalcBoneModelMatrix(ikState, jointBones->baseBone, (*ikState->matArrayPostIK)[0], (float (*)[4])v41);
        v40 = ikState->matArrayPostIK[jointBones->parentBone][0];
        if ( v41 == (_BYTE *)v31
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v31[0] = *(float *)v41;
        v31[1] = *(float *)&v41[16];
        v31[2] = *(float *)&v41[32];
        v31[3] = 0.0f;
        v32 = *(float *)&v41[4];
        v33 = *(float *)&v41[20];
        v34 = *(float *)&v41[36];
        v35 = 0;
        v36 = *(float *)&v41[8];
        v37 = *(float *)&v41[24];
        v38 = *(float *)&v41[40];
        v39 = 0;
        *v40 = (float)((float)(*(float *)&v41[64] * *(float *)v41) + (float)(*(float *)&v41[68] * *(float *)&v41[4]))
             + (float)(*(float *)&v41[72] * *(float *)&v41[8]);
        v40[1] = (float)((float)(*(float *)&v41[64] * v31[1]) + (float)(*(float *)&v41[68] * v33))
               + (float)(*(float *)&v41[72] * v37);
        v40[2] = (float)((float)(*(float *)&v41[64] * v31[2]) + (float)(*(float *)&v41[68] * v34))
               + (float)(*(float *)&v41[72] * v38);
        v40[3] = 0.0f;
        v40[4] = (float)((float)(*(float *)&v41[80] * v31[0]) + (float)(*(float *)&v41[84] * v32))
               + (float)(*(float *)&v41[88] * v36);
        v40[5] = (float)((float)(*(float *)&v41[80] * v31[1]) + (float)(*(float *)&v41[84] * v33))
               + (float)(*(float *)&v41[88] * v37);
        v40[6] = (float)((float)(*(float *)&v41[80] * v31[2]) + (float)(*(float *)&v41[84] * v34))
               + (float)(*(float *)&v41[88] * v38);
        v40[7] = 0.0f;
        v40[8] = (float)((float)(*(float *)&v41[96] * v31[0]) + (float)(*(float *)&v41[100] * v32))
               + (float)(*(float *)&v41[104] * v36);
        v40[9] = (float)((float)(*(float *)&v41[96] * v31[1]) + (float)(*(float *)&v41[100] * v33))
               + (float)(*(float *)&v41[104] * v37);
        v40[10] = (float)((float)(*(float *)&v41[96] * v31[2]) + (float)(*(float *)&v41[100] * v34))
                + (float)(*(float *)&v41[104] * v38);
        v40[11] = 0.0f;
        LODWORD(v30[4]) = &v41[48];
        LODWORD(v30[3]) = &v41[112];
        v30[0] = *(float *)&v41[112] - *(float *)&v41[48];
        v30[1] = *(float *)&v41[116] - *(float *)&v41[52];
        v30[2] = *(float *)&v41[120] - *(float *)&v41[56];
        ikMatrixTransformVector34(v30, (const float (*)[4])v31, v40 + 12);
        v40[15] = 1.0f;
        ikNormalizedMatrixAssert_func(ikState->matArrayPostIK[jointBones->parentBone]);
        parentBone = jointBones->parentBone;
        ikState->bHasActiveLayers = 1;
        if ( ikState->cacheActive )
        {
          memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
          ikState->cacheActive = 0;
        }
        if ( parentBone != IKBONE_NONE )
          ikState->modifiedIKBones |= 1 << parentBone;
        v28 = ikState->matArrayPostIK[jointBones->jointBone][0];
        if ( &v41[64] == (_BYTE *)v19
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v19[0] = *(float *)&v41[64];
        v19[1] = *(float *)&v41[80];
        v19[2] = *(float *)&v41[96];
        v19[3] = 0.0f;
        v20 = *(float *)&v41[68];
        v21 = *(float *)&v41[84];
        v22 = *(float *)&v41[100];
        v23 = 0;
        v24 = *(float *)&v41[72];
        v25 = *(float *)&v41[88];
        v26 = *(float *)&v41[104];
        v27 = 0;
        *v28 = (float)((float)(*(float *)&v41[128] * *(float *)&v41[64])
                     + (float)(*(float *)&v41[132] * *(float *)&v41[68]))
             + (float)(*(float *)&v41[136] * *(float *)&v41[72]);
        v28[1] = (float)((float)(*(float *)&v41[128] * v19[1]) + (float)(*(float *)&v41[132] * v21))
               + (float)(*(float *)&v41[136] * v25);
        v28[2] = (float)((float)(*(float *)&v41[128] * v19[2]) + (float)(*(float *)&v41[132] * v22))
               + (float)(*(float *)&v41[136] * v26);
        v28[3] = 0.0f;
        v28[4] = (float)((float)(*(float *)&v41[144] * v19[0]) + (float)(*(float *)&v41[148] * v20))
               + (float)(*(float *)&v41[152] * v24);
        v28[5] = (float)((float)(*(float *)&v41[144] * v19[1]) + (float)(*(float *)&v41[148] * v21))
               + (float)(*(float *)&v41[152] * v25);
        v28[6] = (float)((float)(*(float *)&v41[144] * v19[2]) + (float)(*(float *)&v41[148] * v22))
               + (float)(*(float *)&v41[152] * v26);
        v28[7] = 0.0f;
        v28[8] = (float)((float)(*(float *)&v41[160] * v19[0]) + (float)(*(float *)&v41[164] * v20))
               + (float)(*(float *)&v41[168] * v24);
        v28[9] = (float)((float)(*(float *)&v41[160] * v19[1]) + (float)(*(float *)&v41[164] * v21))
               + (float)(*(float *)&v41[168] * v25);
        v28[10] = (float)((float)(*(float *)&v41[160] * v19[2]) + (float)(*(float *)&v41[164] * v22))
                + (float)(*(float *)&v41[168] * v26);
        v28[11] = 0.0f;
        LODWORD(v18[4]) = &v41[112];
        LODWORD(v18[3]) = &v41[176];
        v18[0] = *(float *)&v41[176] - *(float *)&v41[112];
        v18[1] = *(float *)&v41[180] - *(float *)&v41[116];
        v18[2] = *(float *)&v41[184] - *(float *)&v41[120];
        ikMatrixTransformVector34(v18, (const float (*)[4])v19, v28 + 12);
        v28[15] = 1.0f;
        ikNormalizedMatrixAssert_func(ikState->matArrayPostIK[jointBones->jointBone]);
        jointBone = jointBones->jointBone;
        ikState->bHasActiveLayers = 1;
        if ( ikState->cacheActive )
        {
          memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
          ikState->cacheActive = 0;
        }
        if ( jointBone != IKBONE_NONE )
          ikState->modifiedIKBones |= 1 << jointBone;
        if ( jointVars->UpperLength + 1.0 <= Vec3Distance((const float *)&v41[112], (const float *)&v41[176])
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                728,
                0,
                "%s",
                "Vec3Distance( parentIKMat[3], jointIKMat[3] ) < jointVars->UpperLength + 1.f") )
        {
          __debugbreak();
        }
        if ( jointVars->LowerLength + 5.0 <= Vec3Distance(&(*childMat)[12], (const float *)&v41[176])
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                729,
                0,
                "%s",
                "Vec3Distance( childMat[3], jointIKMat[3] ) < jointVars->LowerLength + 5.f") )
        {
          __debugbreak();
        }
        if ( v43 )
          IK_FlipHack(childMat);
        v16 = ikState->matArrayPostIK[jointBones->childBone][0];
        if ( &v41[128] == (_BYTE *)v7
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v7[0] = *(float *)&v41[128];
        v7[1] = *(float *)&v41[144];
        v7[2] = *(float *)&v41[160];
        v7[3] = 0.0f;
        v8 = *(float *)&v41[132];
        v9 = *(float *)&v41[148];
        v10 = *(float *)&v41[164];
        v11 = 0;
        v12 = *(float *)&v41[136];
        v13 = *(float *)&v41[152];
        v14 = *(float *)&v41[168];
        v15 = 0;
        *v16 = (float)((float)((*childMat)[0] * *(float *)&v41[128]) + (float)((*childMat)[1] * *(float *)&v41[132]))
             + (float)((*childMat)[2] * *(float *)&v41[136]);
        v16[1] = (float)((float)((*childMat)[0] * v7[1]) + (float)((*childMat)[1] * v9)) + (float)((*childMat)[2] * v13);
        v16[2] = (float)((float)((*childMat)[0] * v7[2]) + (float)((*childMat)[1] * v10))
               + (float)((*childMat)[2] * v14);
        v16[3] = 0.0f;
        v16[4] = (float)((float)((*childMat)[4] * v7[0]) + (float)((*childMat)[5] * v8)) + (float)((*childMat)[6] * v12);
        v16[5] = (float)((float)((*childMat)[4] * v7[1]) + (float)((*childMat)[5] * v9)) + (float)((*childMat)[6] * v13);
        v16[6] = (float)((float)((*childMat)[4] * v7[2]) + (float)((*childMat)[5] * v10))
               + (float)((*childMat)[6] * v14);
        v16[7] = 0.0f;
        v16[8] = (float)((float)((*childMat)[8] * v7[0]) + (float)((*childMat)[9] * v8))
               + (float)((*childMat)[10] * v12);
        v16[9] = (float)((float)((*childMat)[8] * v7[1]) + (float)((*childMat)[9] * v9))
               + (float)((*childMat)[10] * v13);
        v16[10] = (float)((float)((*childMat)[8] * v7[2]) + (float)((*childMat)[9] * v10))
                + (float)((*childMat)[10] * v14);
        v16[11] = 0.0f;
        LODWORD(v6[4]) = &v41[176];
        LODWORD(v6[3]) = &(*childMat)[12];
        v6[0] = (*childMat)[12] - *(float *)&v41[176];
        v6[1] = (*childMat)[13] - *(float *)&v41[180];
        v6[2] = (*childMat)[14] - *(float *)&v41[184];
        ikMatrixTransformVector34(v6, (const float (*)[4])v7, v16 + 12);
        v16[15] = 1.0f;
        ikNormalizedMatrixAssert_func(ikState->matArrayPostIK[jointBones->childBone]);
        childBone = jointBones->childBone;
        ikState->bHasActiveLayers = 1;
        if ( ikState->cacheActive )
        {
          memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
          ikState->cacheActive = 0;
        }
        if ( childBone != IKBONE_NONE )
          ikState->modifiedIKBones |= 1 << childBone;
      }
    }
  }
}

// local variable allocation has failed, the output may be wrong!
void  IK_Layer_ApplyHandIK(
        int a1@<ebp>,
        IKState *ikState,
        IKJointBones *jointBones,
        IKJointVars *jointVars,
        float (*childMat)[4])
{
  IKBoneNames childBone; // [esp-134h] [ebp-204h]
  float v6[5]; // [esp-130h] [ebp-200h] BYREF
  float v7[4]; // [esp-11Ch] [ebp-1ECh] BYREF
  float v8; // [esp-10Ch] [ebp-1DCh]
  float v9; // [esp-108h] [ebp-1D8h]
  float v10; // [esp-104h] [ebp-1D4h]
  int v11; // [esp-100h] [ebp-1D0h]
  float v12; // [esp-FCh] [ebp-1CCh]
  float v13; // [esp-F8h] [ebp-1C8h]
  float v14; // [esp-F4h] [ebp-1C4h]
  int v15; // [esp-F0h] [ebp-1C0h]
  float *v16; // [esp-D8h] [ebp-1A8h]
  IKBoneNames jointBone; // [esp-D4h] [ebp-1A4h]
  float v18[5]; // [esp-D0h] [ebp-1A0h] BYREF
  float v19[4]; // [esp-BCh] [ebp-18Ch] BYREF
  float v20; // [esp-ACh] [ebp-17Ch]
  float v21; // [esp-A8h] [ebp-178h]
  float v22; // [esp-A4h] [ebp-174h]
  int v23; // [esp-A0h] [ebp-170h]
  float v24; // [esp-9Ch] [ebp-16Ch]
  float v25; // [esp-98h] [ebp-168h]
  float v26; // [esp-94h] [ebp-164h]
  int v27; // [esp-90h] [ebp-160h]
  float *v28; // [esp-78h] [ebp-148h]
  IKBoneNames parentBone; // [esp-74h] [ebp-144h]
  float v30[5]; // [esp-70h] [ebp-140h] BYREF
  float v31[4]; // [esp-5Ch] [ebp-12Ch] BYREF
  float v32; // [esp-4Ch] [ebp-11Ch]
  float v33; // [esp-48h] [ebp-118h]
  float v34; // [esp-44h] [ebp-114h]
  int v35; // [esp-40h] [ebp-110h]
  float v36; // [esp-3Ch] [ebp-10Ch]
  float v37; // [esp-38h] [ebp-108h]
  float v38; // [esp-34h] [ebp-104h]
  int v39; // [esp-30h] [ebp-100h]
  float *v40; // [esp-10h] [ebp-E0h]
  _BYTE v41[188]; // [esp-Ch] [ebp-DCh] OVERLAPPED BYREF
  bool v43; // [esp+C3h] [ebp-Dh]
  unsigned int v44[2]; // [esp+C4h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+D0h] [ebp+0h]

  v44[0] = a1;
  v44[1] = retaddr;
  v43 = jointBones->childBone == IKBONE_RHAND;
  if ( ikState->bJointVarsValid )
  {
    if ( ikState->ikBoneToObjBone[jointBones->childBone] != 161 )
    {
      if ( IK_IsCalcBone(ikState, jointBones->childBone) )
      {
        if ( v43 )
          IK_FlipHack(childMat);
        ikSolveArmJoint(
          (int)v44,
          ikState,
          jointBones,
          jointVars,
          &(*childMat)[12],
          (float (*)[4])&v41[64],
          (float (*)[4])&v41[128],
          v43);
        ikCalcBoneModelMatrix(ikState, jointBones->baseBone, (*ikState->matArrayPostIK)[0], (float (*)[4])v41);
        v40 = ikState->matArrayPostIK[jointBones->parentBone][0];
        if ( v41 == (_BYTE *)v31
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v31[0] = *(float *)v41;
        v31[1] = *(float *)&v41[16];
        v31[2] = *(float *)&v41[32];
        v31[3] = 0.0f;
        v32 = *(float *)&v41[4];
        v33 = *(float *)&v41[20];
        v34 = *(float *)&v41[36];
        v35 = 0;
        v36 = *(float *)&v41[8];
        v37 = *(float *)&v41[24];
        v38 = *(float *)&v41[40];
        v39 = 0;
        *v40 = (float)((float)(*(float *)&v41[64] * *(float *)v41) + (float)(*(float *)&v41[68] * *(float *)&v41[4]))
             + (float)(*(float *)&v41[72] * *(float *)&v41[8]);
        v40[1] = (float)((float)(*(float *)&v41[64] * v31[1]) + (float)(*(float *)&v41[68] * v33))
               + (float)(*(float *)&v41[72] * v37);
        v40[2] = (float)((float)(*(float *)&v41[64] * v31[2]) + (float)(*(float *)&v41[68] * v34))
               + (float)(*(float *)&v41[72] * v38);
        v40[3] = 0.0f;
        v40[4] = (float)((float)(*(float *)&v41[80] * v31[0]) + (float)(*(float *)&v41[84] * v32))
               + (float)(*(float *)&v41[88] * v36);
        v40[5] = (float)((float)(*(float *)&v41[80] * v31[1]) + (float)(*(float *)&v41[84] * v33))
               + (float)(*(float *)&v41[88] * v37);
        v40[6] = (float)((float)(*(float *)&v41[80] * v31[2]) + (float)(*(float *)&v41[84] * v34))
               + (float)(*(float *)&v41[88] * v38);
        v40[7] = 0.0f;
        v40[8] = (float)((float)(*(float *)&v41[96] * v31[0]) + (float)(*(float *)&v41[100] * v32))
               + (float)(*(float *)&v41[104] * v36);
        v40[9] = (float)((float)(*(float *)&v41[96] * v31[1]) + (float)(*(float *)&v41[100] * v33))
               + (float)(*(float *)&v41[104] * v37);
        v40[10] = (float)((float)(*(float *)&v41[96] * v31[2]) + (float)(*(float *)&v41[100] * v34))
                + (float)(*(float *)&v41[104] * v38);
        v40[11] = 0.0f;
        LODWORD(v30[4]) = &v41[48];
        LODWORD(v30[3]) = &v41[112];
        v30[0] = *(float *)&v41[112] - *(float *)&v41[48];
        v30[1] = *(float *)&v41[116] - *(float *)&v41[52];
        v30[2] = *(float *)&v41[120] - *(float *)&v41[56];
        ikMatrixTransformVector34(v30, (const float (*)[4])v31, v40 + 12);
        v40[15] = 1.0f;
        parentBone = jointBones->parentBone;
        ikState->bHasActiveLayers = 1;
        if ( ikState->cacheActive )
        {
          memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
          ikState->cacheActive = 0;
        }
        if ( parentBone != IKBONE_NONE )
          ikState->modifiedIKBones |= 1 << parentBone;
        v28 = ikState->matArrayPostIK[jointBones->jointBone][0];
        if ( &v41[64] == (_BYTE *)v19
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v19[0] = *(float *)&v41[64];
        v19[1] = *(float *)&v41[80];
        v19[2] = *(float *)&v41[96];
        v19[3] = 0.0f;
        v20 = *(float *)&v41[68];
        v21 = *(float *)&v41[84];
        v22 = *(float *)&v41[100];
        v23 = 0;
        v24 = *(float *)&v41[72];
        v25 = *(float *)&v41[88];
        v26 = *(float *)&v41[104];
        v27 = 0;
        *v28 = (float)((float)(*(float *)&v41[128] * *(float *)&v41[64])
                     + (float)(*(float *)&v41[132] * *(float *)&v41[68]))
             + (float)(*(float *)&v41[136] * *(float *)&v41[72]);
        v28[1] = (float)((float)(*(float *)&v41[128] * v19[1]) + (float)(*(float *)&v41[132] * v21))
               + (float)(*(float *)&v41[136] * v25);
        v28[2] = (float)((float)(*(float *)&v41[128] * v19[2]) + (float)(*(float *)&v41[132] * v22))
               + (float)(*(float *)&v41[136] * v26);
        v28[3] = 0.0f;
        v28[4] = (float)((float)(*(float *)&v41[144] * v19[0]) + (float)(*(float *)&v41[148] * v20))
               + (float)(*(float *)&v41[152] * v24);
        v28[5] = (float)((float)(*(float *)&v41[144] * v19[1]) + (float)(*(float *)&v41[148] * v21))
               + (float)(*(float *)&v41[152] * v25);
        v28[6] = (float)((float)(*(float *)&v41[144] * v19[2]) + (float)(*(float *)&v41[148] * v22))
               + (float)(*(float *)&v41[152] * v26);
        v28[7] = 0.0f;
        v28[8] = (float)((float)(*(float *)&v41[160] * v19[0]) + (float)(*(float *)&v41[164] * v20))
               + (float)(*(float *)&v41[168] * v24);
        v28[9] = (float)((float)(*(float *)&v41[160] * v19[1]) + (float)(*(float *)&v41[164] * v21))
               + (float)(*(float *)&v41[168] * v25);
        v28[10] = (float)((float)(*(float *)&v41[160] * v19[2]) + (float)(*(float *)&v41[164] * v22))
                + (float)(*(float *)&v41[168] * v26);
        v28[11] = 0.0f;
        LODWORD(v18[4]) = &v41[112];
        LODWORD(v18[3]) = &v41[176];
        v18[0] = *(float *)&v41[176] - *(float *)&v41[112];
        v18[1] = *(float *)&v41[180] - *(float *)&v41[116];
        v18[2] = *(float *)&v41[184] - *(float *)&v41[120];
        ikMatrixTransformVector34(v18, (const float (*)[4])v19, v28 + 12);
        v28[15] = 1.0f;
        jointBone = jointBones->jointBone;
        ikState->bHasActiveLayers = 1;
        if ( ikState->cacheActive )
        {
          memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
          ikState->cacheActive = 0;
        }
        if ( jointBone != IKBONE_NONE )
          ikState->modifiedIKBones |= 1 << jointBone;
        if ( v43 )
          IK_FlipHack(childMat);
        v16 = ikState->matArrayPostIK[jointBones->childBone][0];
        if ( &v41[128] == (_BYTE *)v7
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v7[0] = *(float *)&v41[128];
        v7[1] = *(float *)&v41[144];
        v7[2] = *(float *)&v41[160];
        v7[3] = 0.0f;
        v8 = *(float *)&v41[132];
        v9 = *(float *)&v41[148];
        v10 = *(float *)&v41[164];
        v11 = 0;
        v12 = *(float *)&v41[136];
        v13 = *(float *)&v41[152];
        v14 = *(float *)&v41[168];
        v15 = 0;
        *v16 = (float)((float)((*childMat)[0] * *(float *)&v41[128]) + (float)((*childMat)[1] * *(float *)&v41[132]))
             + (float)((*childMat)[2] * *(float *)&v41[136]);
        v16[1] = (float)((float)((*childMat)[0] * v7[1]) + (float)((*childMat)[1] * v9)) + (float)((*childMat)[2] * v13);
        v16[2] = (float)((float)((*childMat)[0] * v7[2]) + (float)((*childMat)[1] * v10))
               + (float)((*childMat)[2] * v14);
        v16[3] = 0.0f;
        v16[4] = (float)((float)((*childMat)[4] * v7[0]) + (float)((*childMat)[5] * v8)) + (float)((*childMat)[6] * v12);
        v16[5] = (float)((float)((*childMat)[4] * v7[1]) + (float)((*childMat)[5] * v9)) + (float)((*childMat)[6] * v13);
        v16[6] = (float)((float)((*childMat)[4] * v7[2]) + (float)((*childMat)[5] * v10))
               + (float)((*childMat)[6] * v14);
        v16[7] = 0.0f;
        v16[8] = (float)((float)((*childMat)[8] * v7[0]) + (float)((*childMat)[9] * v8))
               + (float)((*childMat)[10] * v12);
        v16[9] = (float)((float)((*childMat)[8] * v7[1]) + (float)((*childMat)[9] * v9))
               + (float)((*childMat)[10] * v13);
        v16[10] = (float)((float)((*childMat)[8] * v7[2]) + (float)((*childMat)[9] * v10))
                + (float)((*childMat)[10] * v14);
        v16[11] = 0.0f;
        LODWORD(v6[4]) = &v41[176];
        LODWORD(v6[3]) = &(*childMat)[12];
        v6[0] = (*childMat)[12] - *(float *)&v41[176];
        v6[1] = (*childMat)[13] - *(float *)&v41[180];
        v6[2] = (*childMat)[14] - *(float *)&v41[184];
        ikMatrixTransformVector34(v6, (const float (*)[4])v7, v16 + 12);
        v16[15] = 1.0f;
        childBone = jointBones->childBone;
        ikState->bHasActiveLayers = 1;
        if ( ikState->cacheActive )
        {
          memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
          ikState->cacheActive = 0;
        }
        if ( childBone != IKBONE_NONE )
          ikState->modifiedIKBones |= 1 << childBone;
      }
    }
  }
}

// local variable allocation has failed, the output may be wrong!
void  IK_Layer_LeftHandOnGun(float a1@<ebp>, IKState *ikState)
{
  float *v2; // eax
  float *v3; // edx
  float *v4; // ecx
  float v5[3]; // [esp+14h] [ebp-600h] BYREF
  float twistRot[3]; // [esp+20h] [ebp-5F4h] BYREF
  float twistScale; // [esp+2Ch] [ebp-5E8h]
  float *v8; // [esp+30h] [ebp-5E4h]
  float *v9; // [esp+34h] [ebp-5E0h]
  float v10[4]; // [esp+38h] [ebp-5DCh] BYREF
  float v11; // [esp+48h] [ebp-5CCh]
  float v12; // [esp+4Ch] [ebp-5C8h]
  float v13; // [esp+50h] [ebp-5C4h]
  int v14; // [esp+54h] [ebp-5C0h]
  float v15; // [esp+58h] [ebp-5BCh]
  float v16; // [esp+5Ch] [ebp-5B8h]
  float v17; // [esp+60h] [ebp-5B4h]
  int v18; // [esp+64h] [ebp-5B0h]
  float *v19; // [esp+7Ch] [ebp-598h]
  float *v20; // [esp+80h] [ebp-594h]
  float *v21; // [esp+84h] [ebp-590h]
  _BYTE *v22; // [esp+88h] [ebp-58Ch]
  _BYTE *v23; // [esp+8Ch] [ebp-588h]
  _BYTE *v24; // [esp+90h] [ebp-584h]
  _BYTE *v25; // [esp+94h] [ebp-580h]
  _BYTE v26[76]; // [esp+98h] [ebp-57Ch] OVERLAPPED BYREF
  float v27; // [esp+E4h] [ebp-530h]
  float v28; // [esp+E8h] [ebp-52Ch]
  float v29; // [esp+ECh] [ebp-528h]
  float v30; // [esp+F0h] [ebp-524h]
  float v31; // [esp+F4h] [ebp-520h]
  float v32; // [esp+F8h] [ebp-51Ch]
  float v33; // [esp+FCh] [ebp-518h]
  float v34; // [esp+100h] [ebp-514h]
  float v35; // [esp+104h] [ebp-510h]
  float v36; // [esp+108h] [ebp-50Ch]
  float v37; // [esp+10Ch] [ebp-508h]
  float v38; // [esp+110h] [ebp-504h]
  float v39; // [esp+114h] [ebp-500h]
  float v40; // [esp+118h] [ebp-4FCh]
  float v41; // [esp+11Ch] [ebp-4F8h]
  _BYTE *v42; // [esp+124h] [ebp-4F0h]
  _BYTE *v43; // [esp+128h] [ebp-4ECh]
  _BYTE *v44; // [esp+12Ch] [ebp-4E8h]
  int v45; // [esp+130h] [ebp-4E4h]
  int v46; // [esp+134h] [ebp-4E0h]
  int v47; // [esp+138h] [ebp-4DCh]
  __int64 v48; // [esp+13Ch] [ebp-4D8h]
  __int64 v49; // [esp+144h] [ebp-4D0h]
  __int64 v50; // [esp+14Ch] [ebp-4C8h]
  __int64 v51; // [esp+154h] [ebp-4C0h]
  __int64 v52; // [esp+15Ch] [ebp-4B8h]
  __int64 v53; // [esp+164h] [ebp-4B0h]
  int v54; // [esp+16Ch] [ebp-4A8h]
  _BYTE *v55; // [esp+170h] [ebp-4A4h]
  float *v56; // [esp+174h] [ebp-4A0h]
  float *v57; // [esp+178h] [ebp-49Ch]
  float *v58; // [esp+17Ch] [ebp-498h]
  float v59; // [esp+180h] [ebp-494h]
  float v60; // [esp+184h] [ebp-490h]
  float v61; // [esp+188h] [ebp-48Ch]
  __int64 v62; // [esp+18Ch] [ebp-488h]
  __int64 v63; // [esp+194h] [ebp-480h]
  __int64 v64; // [esp+19Ch] [ebp-478h]
  __int64 v65; // [esp+1A4h] [ebp-470h]
  __int64 v66; // [esp+1ACh] [ebp-468h]
  __int64 v67; // [esp+1B4h] [ebp-460h]
  float v68; // [esp+1BCh] [ebp-458h]
  float *v69; // [esp+1C4h] [ebp-450h]
  _BYTE *v70; // [esp+1C8h] [ebp-44Ch]
  _BYTE *v71; // [esp+1CCh] [ebp-448h]
  _BYTE *v72; // [esp+1D0h] [ebp-444h]
  _BYTE *v73; // [esp+1D4h] [ebp-440h]
  _BYTE v74[76]; // [esp+1D8h] [ebp-43Ch] OVERLAPPED BYREF
  float *v75; // [esp+224h] [ebp-3F0h]
  __int64 v76; // [esp+228h] [ebp-3ECh]
  _BYTE v77[12]; // [esp+230h] [ebp-3E4h]
  __int64 v78; // [esp+23Ch] [ebp-3D8h]
  __int64 v79; // [esp+244h] [ebp-3D0h]
  __int64 v80; // [esp+24Ch] [ebp-3C8h]
  __int64 v81; // [esp+254h] [ebp-3C0h]
  __int64 v82; // [esp+25Ch] [ebp-3B8h]
  float v83; // [esp+264h] [ebp-3B0h]
  float *v84; // [esp+268h] [ebp-3ACh]
  float *v85; // [esp+26Ch] [ebp-3A8h]
  float *v86; // [esp+270h] [ebp-3A4h]
  float *v87; // [esp+274h] [ebp-3A0h]
  float *v88; // [esp+278h] [ebp-39Ch]
  float *v89; // [esp+27Ch] [ebp-398h]
  __int64 v90; // [esp+280h] [ebp-394h]
  __int64 v91; // [esp+288h] [ebp-38Ch]
  float v92; // [esp+290h] [ebp-384h]
  float v93; // [esp+294h] [ebp-380h]
  float v94; // [esp+298h] [ebp-37Ch]
  float v95; // [esp+29Ch] [ebp-378h]
  float v96; // [esp+2A0h] [ebp-374h]
  float v97; // [esp+2A4h] [ebp-370h]
  float v98; // [esp+2A8h] [ebp-36Ch]
  float v99; // [esp+2ACh] [ebp-368h]
  float v100; // [esp+2B0h] [ebp-364h]
  float v101; // [esp+2B4h] [ebp-360h]
  float v102; // [esp+2B8h] [ebp-35Ch]
  float v103; // [esp+2BCh] [ebp-358h]
  _BYTE *v104; // [esp+2C4h] [ebp-350h]
  _BYTE *v105; // [esp+2C8h] [ebp-34Ch]
  _BYTE *v106; // [esp+2CCh] [ebp-348h]
  int v107; // [esp+2D0h] [ebp-344h]
  int v108; // [esp+2D4h] [ebp-340h]
  int v109; // [esp+2D8h] [ebp-33Ch]
  __int64 v110; // [esp+2DCh] [ebp-338h]
  __int64 v111; // [esp+2E4h] [ebp-330h]
  __int64 v112; // [esp+2ECh] [ebp-328h]
  __int64 v113; // [esp+2F4h] [ebp-320h]
  __int64 v114; // [esp+2FCh] [ebp-318h]
  __int64 v115; // [esp+304h] [ebp-310h]
  int v116; // [esp+30Ch] [ebp-308h]
  _BYTE *v117; // [esp+310h] [ebp-304h]
  float *v118; // [esp+314h] [ebp-300h]
  float *v119; // [esp+318h] [ebp-2FCh]
  float *v120; // [esp+31Ch] [ebp-2F8h]
  float v121; // [esp+320h] [ebp-2F4h]
  float v122; // [esp+324h] [ebp-2F0h]
  float v123; // [esp+328h] [ebp-2ECh]
  __int64 v124; // [esp+32Ch] [ebp-2E8h]
  __int64 v125; // [esp+334h] [ebp-2E0h]
  __int64 v126; // [esp+33Ch] [ebp-2D8h]
  __int64 v127; // [esp+344h] [ebp-2D0h]
  __int64 v128; // [esp+34Ch] [ebp-2C8h]
  __int64 v129; // [esp+354h] [ebp-2C0h]
  float v130; // [esp+35Ch] [ebp-2B8h]
  float *v131; // [esp+360h] [ebp-2B4h]
  float *v132; // [esp+364h] [ebp-2B0h]
  float *v133; // [esp+368h] [ebp-2ACh]
  float *v134; // [esp+36Ch] [ebp-2A8h]
  float *v135; // [esp+370h] [ebp-2A4h]
  float *v136; // [esp+374h] [ebp-2A0h]
  float v137; // [esp+378h] [ebp-29Ch] BYREF
  float v138; // [esp+37Ch] [ebp-298h]
  float v139; // [esp+380h] [ebp-294h]
  float workMat[4][4]; // [esp+384h] [ebp-290h] BYREF
  float *v141; // [esp+3C4h] [ebp-250h]
  float *v142; // [esp+3C8h] [ebp-24Ch]
  float *v143; // [esp+3CCh] [ebp-248h]
  float v144; // [esp+3D0h] [ebp-244h]
  float v145; // [esp+3D4h] [ebp-240h]
  float v146; // [esp+3D8h] [ebp-23Ch]
  __int64 v147; // [esp+3DCh] [ebp-238h]
  __int64 v148; // [esp+3E4h] [ebp-230h]
  __int64 v149; // [esp+3ECh] [ebp-228h]
  __int64 v150; // [esp+3F4h] [ebp-220h]
  __int64 v151; // [esp+3FCh] [ebp-218h]
  __int64 v152; // [esp+404h] [ebp-210h]
  float v153; // [esp+40Ch] [ebp-208h]
  float *v154; // [esp+414h] [ebp-200h]
  _BYTE v155[44]; // [esp+418h] [ebp-1FCh] OVERLAPPED BYREF
  float v156[3]; // [esp+464h] [ebp-1B0h] BYREF
  float axis[3][3]; // [esp+470h] [ebp-1A4h]
  float v158; // [esp+494h] [ebp-180h]
  float v159; // [esp+498h] [ebp-17Ch]
  float leftLerp; // [esp+49Ch] [ebp-178h]
  float gunPlaneOffset; // [esp+4A0h] [ebp-174h] BYREF
  float maxRange; // [esp+4A4h] [ebp-170h]
  float minRange; // [esp+4A8h] [ebp-16Ch]
  float gripPos[4]; // [esp+4ACh] [ebp-168h] BYREF
  float gripPosCenterEquipment[3]; // [esp+4BCh] [ebp-158h] BYREF
  float gripPosCenterShoulder[3]; // [esp+4C8h] [ebp-14Ch] BYREF
  float gripPosCenter[3]; // [esp+4D4h] [ebp-140h]
  float gripOffsetFromHandBone; // [esp+4E0h] [ebp-134h]
  float rangeShotgunAdd; // [esp+4E4h] [ebp-130h] BYREF
  float maxRangeBase; // [esp+4E8h] [ebp-12Ch]
  float minRangeBase; // [esp+4ECh] [ebp-128h]
  float *v172; // [esp+4F0h] [ebp-124h]
  _BYTE *v173; // [esp+4F4h] [ebp-120h]
  float v174; // [esp+4F8h] [ebp-11Ch] BYREF
  float v175; // [esp+4FCh] [ebp-118h]
  float v176; // [esp+500h] [ebp-114h]
  float leftHandIKMat[17]; // [esp+504h] [ebp-110h] OVERLAPPED BYREF
  int v178; // [esp+548h] [ebp-CCh]
  int v179; // [esp+54Ch] [ebp-C8h]
  int v180; // [esp+550h] [ebp-C4h]
  int v181; // [esp+554h] [ebp-C0h]
  int v182; // [esp+558h] [ebp-BCh]
  int v183; // [esp+55Ch] [ebp-B8h]
  int v184; // [esp+560h] [ebp-B4h]
  int v185; // [esp+564h] [ebp-B0h]
  float *v186; // [esp+580h] [ebp-94h]
  float *leftHandRotation; // [esp+584h] [ebp-90h]
  float *leftHandOffset; // [esp+588h] [ebp-8Ch]
  float v189; // [esp+58Ch] [ebp-88h]
  float fraction; // [esp+590h] [ebp-84h]
  float v191; // [esp+594h] [ebp-80h]
  float lerp; // [esp+598h] [ebp-7Ch]
  int v193; // [esp+59Ch] [ebp-78h]
  float v194; // [esp+5A0h] [ebp-74h] BYREF
  int v195; // [esp+5A4h] [ebp-70h]
  int leftHandLerpTime; // [esp+5A8h] [ebp-6Ch]
  float leftHandRotationLocal[3]; // [esp+5ACh] [ebp-68h] BYREF
  _BYTE leftHandOffsetLocal[76]; // [esp+5B8h] [ebp-5Ch] OVERLAPPED BYREF
  float layerLerp; // [esp+608h] [ebp-Ch] BYREF
  int ikLayerName; // [esp+60Ch] [ebp-8h]
  int retaddr; // [esp+614h] [ebp+0h]

  layerLerp = a1;
  ikLayerName = retaddr;
  *(unsigned int *)&leftHandOffsetLocal[72] = 11;
  *(float *)&leftHandOffsetLocal[68] = IKImport_GetLayerLerp(ikState, IKLAYER_LEFT_HAND_ON_GUN, flt_E1270C);
  ikState->leftHandLastProximityLerp = 0.0f;
  if ( *(float *)&leftHandOffsetLocal[68] > 0.0 )
  {
    leftHandOffsetLocal[67] = ikState->ikBoneToObjBone[10] != 161;
    if ( leftHandOffsetLocal[67] )
    {
      leftHandOffsetLocal[66] = ikState->ikBoneToObjBone[22] != 161;
      if ( leftHandOffsetLocal[66] )
      {
        if ( ikState->bJointVarsValid && IK_IsCalcBone(ikState, 10) )
        {
          ikCalcBoneModelMatrix(ikState, 22, (*ikState->matArrayPostIK)[0], (float (*)[4])leftHandOffsetLocal);
          if ( bGetMat )
          {
            leftHandOffsetStatic[0] = 0.0f;
            dword_9C3DE68 = 0;
            dword_9C3DE6C = 0;
            bGetMat = 0;
          }
          if ( IKImport_GetVar_IK_Hand_Tuning() || bHackUseDevGuiVars )
          {
            IKImport_GetVar_IK_Hand_Offset_Vec(leftHandOffsetStatic);
            IKImport_GetVar_IK_Hand_Rotation_Vec(leftHandRotationStatic);
            leftHandRotationLocal[0] = leftHandOffsetStatic[0];
            LODWORD(leftHandRotationLocal[1]) = dword_9C3DE68;
            LODWORD(leftHandRotationLocal[2]) = dword_9C3DE6C;
            v194 = leftHandRotationStatic[0];
            v195 = dword_9C3DE5C;
            leftHandLerpTime = dword_9C3DE60;
          }
          else
          {
            v193 = 100;
            if ( ikState->leftHandChangeTime <= ikState->timeMS - 100 )
            {
              leftHandOffset = ikState->leftHandOffset;
              leftHandRotationLocal[0] = ikState->leftHandOffset[0];
              leftHandRotationLocal[1] = ikState->leftHandOffset[1];
              leftHandRotationLocal[2] = ikState->leftHandOffset[2];
              leftHandRotation = ikState->leftHandRotation;
              v194 = ikState->leftHandRotation[0];
              v195 = LODWORD(ikState->leftHandRotation[1]);
              leftHandLerpTime = LODWORD(ikState->leftHandRotation[2]);
            }
            else
            {
              lerp = (float)(ikState->timeMS - ikState->leftHandChangeTime) / 100.0;
              if ( (float)(lerp - 1.0) < 0.0 )
                v191 = lerp;
              else
                v191 = 1.0f;
              if ( (float)(0.0 - lerp) < 0.0 )
                fraction = v191;
              else
                fraction = 0.0f;
              v189 = fraction;
              Vec3Lerp(ikState->leftHandOffsetPrev, ikState->leftHandOffset, fraction, leftHandRotationLocal);
              Vec3Lerp(ikState->leftHandRotationPrev, ikState->leftHandRotation, fraction, &v194);
            }
            leftHandOffsetStatic[0] = leftHandRotationLocal[0];
            dword_9C3DE68 = LODWORD(leftHandRotationLocal[1]);
            dword_9C3DE6C = LODWORD(leftHandRotationLocal[2]);
            leftHandRotationStatic[0] = v194;
            dword_9C3DE5C = v195;
            dword_9C3DE60 = leftHandLerpTime;
          }
          v186 = ikState->handMats[0][0];
          if ( leftHandOffsetLocal == (_BYTE *)&leftHandIKMat[13]
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
          {
            __debugbreak();
          }
          leftHandIKMat[13] = *(float *)leftHandOffsetLocal;
          leftHandIKMat[14] = *(float *)&leftHandOffsetLocal[16];
          leftHandIKMat[15] = *(float *)&leftHandOffsetLocal[32];
          leftHandIKMat[16] = 0.0f;
          v178 = *(unsigned int *)&leftHandOffsetLocal[4];
          v179 = *(unsigned int *)&leftHandOffsetLocal[20];
          v180 = *(unsigned int *)&leftHandOffsetLocal[36];
          v181 = 0;
          v182 = *(unsigned int *)&leftHandOffsetLocal[8];
          v183 = *(unsigned int *)&leftHandOffsetLocal[24];
          v184 = *(unsigned int *)&leftHandOffsetLocal[40];
          v185 = 0;
          v174 = (float)((float)(*v186 * *(float *)leftHandOffsetLocal)
                       + (float)(v186[1] * *(float *)&leftHandOffsetLocal[4]))
               + (float)(v186[2] * *(float *)&leftHandOffsetLocal[8]);
          v175 = (float)((float)(*v186 * *(float *)&leftHandOffsetLocal[16])
                       + (float)(v186[1] * *(float *)&leftHandOffsetLocal[20]))
               + (float)(v186[2] * *(float *)&leftHandOffsetLocal[24]);
          v176 = (float)((float)(*v186 * *(float *)&leftHandOffsetLocal[32])
                       + (float)(v186[1] * *(float *)&leftHandOffsetLocal[36]))
               + (float)(v186[2] * *(float *)&leftHandOffsetLocal[40]);
          leftHandIKMat[0] = 0.0f;
          leftHandIKMat[1] = (float)((float)(v186[4] * *(float *)leftHandOffsetLocal)
                                   + (float)(v186[5] * *(float *)&leftHandOffsetLocal[4]))
                           + (float)(v186[6] * *(float *)&leftHandOffsetLocal[8]);
          leftHandIKMat[2] = (float)((float)(v186[4] * *(float *)&leftHandOffsetLocal[16])
                                   + (float)(v186[5] * *(float *)&leftHandOffsetLocal[20]))
                           + (float)(v186[6] * *(float *)&leftHandOffsetLocal[24]);
          leftHandIKMat[3] = (float)((float)(v186[4] * *(float *)&leftHandOffsetLocal[32])
                                   + (float)(v186[5] * *(float *)&leftHandOffsetLocal[36]))
                           + (float)(v186[6] * *(float *)&leftHandOffsetLocal[40]);
          leftHandIKMat[4] = 0.0f;
          leftHandIKMat[5] = (float)((float)(v186[8] * *(float *)leftHandOffsetLocal)
                                   + (float)(v186[9] * *(float *)&leftHandOffsetLocal[4]))
                           + (float)(v186[10] * *(float *)&leftHandOffsetLocal[8]);
          leftHandIKMat[6] = (float)((float)(v186[8] * *(float *)&leftHandOffsetLocal[16])
                                   + (float)(v186[9] * *(float *)&leftHandOffsetLocal[20]))
                           + (float)(v186[10] * *(float *)&leftHandOffsetLocal[24]);
          leftHandIKMat[7] = (float)((float)(v186[8] * *(float *)&leftHandOffsetLocal[32])
                                   + (float)(v186[9] * *(float *)&leftHandOffsetLocal[36]))
                           + (float)(v186[10] * *(float *)&leftHandOffsetLocal[40]);
          leftHandIKMat[8] = 0.0f;
          v173 = &leftHandOffsetLocal[48];
          v172 = v186 + 12;
          rangeShotgunAdd = v186[12] - *(float *)&leftHandOffsetLocal[48];
          maxRangeBase = v186[13] - *(float *)&leftHandOffsetLocal[52];
          minRangeBase = v186[14] - *(float *)&leftHandOffsetLocal[56];
          ikMatrixTransformVector34(&rangeShotgunAdd, (const float (*)[4])&leftHandIKMat[13], &leftHandIKMat[9]);
          leftHandIKMat[12] = 1.0f;
          gripOffsetFromHandBone = 3.0f;
          gripPosCenter[2] = 8.0f;
          gripPosCenter[1] = 5.0f;
          gripPosCenter[0] = 2.3f;
          gripPosCenterShoulder[0] = -4.0f;
          gripPosCenterShoulder[1] = 0.1f;
          gripPosCenterShoulder[2] = FLOAT_N3_4000001;
          gripPosCenterEquipment[0] = FLOAT_N18_0;
          gripPosCenterEquipment[1] = FLOAT_1_6;
          gripPosCenterEquipment[2] = 0.0f;
          gripPos[1] = 1.0f;
          gripPos[2] = 7.0f;
          gripPos[3] = 0.4f;
          LODWORD(gripPos[0]) = &leftHandIKMat[9];
          gunPlaneOffset = (float)(2.3 * v174) + leftHandIKMat[9];
          maxRange = (float)(2.3 * v175) + leftHandIKMat[10];
          minRange = (float)(2.3 * v176) + leftHandIKMat[11];
          leftLerp = IKImport_GetLayerLerp(ikState, IKLAYER_SHOTGUN_ACTIVE) * 5.0 + 3.0;
          v159 = IKImport_GetLayerLerp(ikState, IKLAYER_SHOTGUN_ACTIVE) * 5.0 + 8.0;
          if ( ikState->weaponOffsetType == IKWEAPOFS_SHOULDER )
          {
            v158 = Vec3Distance(&gunPlaneOffset, gripPosCenterEquipment);
          }
          else if ( ikState->weaponOffsetType == IKWEAPOFS_GRENADE )
          {
            v158 = Vec3Distance(&gunPlaneOffset, &gripPos[1]);
          }
          else
          {
            v158 = Vec3Distance(&gunPlaneOffset, gripPosCenterShoulder);
          }
          if ( leftLerp <= v158 )
          {
            if ( v159 <= v158 )
              axis[2][2] = 0.0f;
            else
              axis[2][2] = 1.0 - (float)((float)(v158 - leftLerp) / (float)(v159 - leftLerp));
          }
          else
          {
            axis[2][2] = 1.0f;
          }
          if ( !ikSystem.inViewer )
            *(float *)&leftHandOffsetLocal[68] = *(float *)&leftHandOffsetLocal[68] * axis[2][2];
          ikState->leftHandLastProximityLerp = axis[2][2];
          if ( *(float *)&leftHandOffsetLocal[68] >= 0.0099999998 )
          {
            LODWORD(axis[2][1]) = &leftHandIKMat[9];
            LODWORD(axis[2][0]) = &leftHandIKMat[9];
            leftHandIKMat[9] = leftHandIKMat[9] + leftHandRotationLocal[0];
            leftHandIKMat[10] = leftHandIKMat[10] + leftHandRotationLocal[1];
            leftHandIKMat[11] = leftHandIKMat[11] + leftHandRotationLocal[2];
            AnglesToAxis(&v194, (float (*)[3])v156);
            ikMatrixSet44((float (*)[4])v155, vec3_origin, (const float (*)[3])v156, 1.0);
            v154 = &v174;
            v144 = v174;
            v145 = v175;
            v146 = v176;
            v147 = *(_QWORD *)leftHandIKMat;
            v143 = &leftHandIKMat[1];
            v148 = *(_QWORD *)&leftHandIKMat[2];
            v149 = *(_QWORD *)&leftHandIKMat[4];
            v142 = &leftHandIKMat[5];
            v150 = *(_QWORD *)&leftHandIKMat[6];
            v151 = *(_QWORD *)&leftHandIKMat[8];
            v141 = &leftHandIKMat[9];
            v152 = *(_QWORD *)&leftHandIKMat[10];
            v153 = leftHandIKMat[12];
            v136 = &v137;
            v137 = v174;
            v138 = v175;
            v139 = v176;
            *(_QWORD *)&workMat[0][0] = *(_QWORD *)leftHandIKMat;
            v135 = &workMat[0][1];
            *(_QWORD *)&workMat[0][2] = *(_QWORD *)&leftHandIKMat[2];
            *(_QWORD *)&workMat[1][0] = *(_QWORD *)&leftHandIKMat[4];
            v134 = &workMat[1][1];
            *(_QWORD *)&workMat[1][2] = *(_QWORD *)&leftHandIKMat[6];
            *(_QWORD *)&workMat[2][0] = *(_QWORD *)&leftHandIKMat[8];
            v133 = &workMat[2][1];
            *(_QWORD *)&workMat[2][2] = *(_QWORD *)&leftHandIKMat[10];
            workMat[3][0] = leftHandIKMat[12];
            v174 = (float)((float)(v174 * *(float *)v155) + (float)(v175 * *(float *)&v155[16]))
                 + (float)(v176 * *(float *)&v155[32]);
            v175 = (float)((float)(v137 * *(float *)&v155[4]) + (float)(v175 * *(float *)&v155[20]))
                 + (float)(v176 * *(float *)&v155[36]);
            v176 = (float)((float)(v137 * *(float *)&v155[8]) + (float)(v138 * *(float *)&v155[24]))
                 + (float)(v176 * *(float *)&v155[40]);
            leftHandIKMat[0] = 0.0f;
            leftHandIKMat[1] = (float)((float)(leftHandIKMat[1] * *(float *)v155)
                                     + (float)(leftHandIKMat[2] * *(float *)&v155[16]))
                             + (float)(leftHandIKMat[3] * *(float *)&v155[32]);
            leftHandIKMat[2] = (float)((float)(workMat[0][1] * *(float *)&v155[4])
                                     + (float)(leftHandIKMat[2] * *(float *)&v155[20]))
                             + (float)(leftHandIKMat[3] * *(float *)&v155[36]);
            leftHandIKMat[3] = (float)((float)(workMat[0][1] * *(float *)&v155[8])
                                     + (float)(workMat[0][2] * *(float *)&v155[24]))
                             + (float)(leftHandIKMat[3] * *(float *)&v155[40]);
            leftHandIKMat[4] = 0.0f;
            leftHandIKMat[5] = (float)((float)(leftHandIKMat[5] * *(float *)v155)
                                     + (float)(leftHandIKMat[6] * *(float *)&v155[16]))
                             + (float)(leftHandIKMat[7] * *(float *)&v155[32]);
            leftHandIKMat[6] = (float)((float)(workMat[1][1] * *(float *)&v155[4])
                                     + (float)(leftHandIKMat[6] * *(float *)&v155[20]))
                             + (float)(leftHandIKMat[7] * *(float *)&v155[36]);
            leftHandIKMat[7] = (float)((float)(workMat[1][1] * *(float *)&v155[8])
                                     + (float)(workMat[1][2] * *(float *)&v155[24]))
                             + (float)(leftHandIKMat[7] * *(float *)&v155[40]);
            leftHandIKMat[8] = 0.0f;
            if ( *(float *)&leftHandOffsetLocal[68] == 1.0 )
            {
              v132 = ikState->handMats[0][0];
              v131 = &v174;
              v121 = v174;
              v122 = v175;
              v123 = v176;
              v124 = *(_QWORD *)leftHandIKMat;
              v120 = &leftHandIKMat[1];
              v125 = *(_QWORD *)&leftHandIKMat[2];
              v126 = *(_QWORD *)&leftHandIKMat[4];
              v119 = &leftHandIKMat[5];
              v127 = *(_QWORD *)&leftHandIKMat[6];
              v128 = *(_QWORD *)&leftHandIKMat[8];
              v118 = &leftHandIKMat[9];
              v129 = *(_QWORD *)&leftHandIKMat[10];
              v130 = leftHandIKMat[12];
              v117 = leftHandOffsetLocal;
              v107 = *(unsigned int *)leftHandOffsetLocal;
              v108 = *(unsigned int *)&leftHandOffsetLocal[4];
              v109 = *(unsigned int *)&leftHandOffsetLocal[8];
              v110 = *(_QWORD *)&leftHandOffsetLocal[12];
              v106 = &leftHandOffsetLocal[16];
              v111 = *(_QWORD *)&leftHandOffsetLocal[20];
              v112 = *(_QWORD *)&leftHandOffsetLocal[28];
              v105 = &leftHandOffsetLocal[32];
              v113 = *(_QWORD *)&leftHandOffsetLocal[36];
              v114 = *(_QWORD *)&leftHandOffsetLocal[44];
              v104 = &leftHandOffsetLocal[48];
              v115 = *(_QWORD *)&leftHandOffsetLocal[52];
              v116 = *(unsigned int *)&leftHandOffsetLocal[60];
              *(float *)&v90 = (float)((float)((float)(v174 * *(float *)leftHandOffsetLocal)
                                             + (float)(v175 * *(float *)&leftHandOffsetLocal[16]))
                                     + (float)(v176 * *(float *)&leftHandOffsetLocal[32]))
                             + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[48]);
              *((float *)&v90 + 1) = (float)((float)((float)(v174 * *(float *)&leftHandOffsetLocal[4])
                                                   + (float)(v175 * *(float *)&leftHandOffsetLocal[20]))
                                           + (float)(v176 * *(float *)&leftHandOffsetLocal[36]))
                                   + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[52]);
              *(float *)&v91 = (float)((float)((float)(v174 * *(float *)&leftHandOffsetLocal[8])
                                             + (float)(v175 * *(float *)&leftHandOffsetLocal[24]))
                                     + (float)(v176 * *(float *)&leftHandOffsetLocal[40]))
                             + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[56]);
              *((float *)&v91 + 1) = (float)((float)((float)(v174 * *(float *)&leftHandOffsetLocal[12])
                                                   + (float)(v175 * *(float *)&leftHandOffsetLocal[28]))
                                           + (float)(v176 * *(float *)&leftHandOffsetLocal[44]))
                                   + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[60]);
              v92 = (float)((float)((float)(leftHandIKMat[1] * *(float *)leftHandOffsetLocal)
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[16]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[32]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[48]);
              v93 = (float)((float)((float)(leftHandIKMat[1] * *(float *)&leftHandOffsetLocal[4])
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[20]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[36]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[52]);
              v94 = (float)((float)((float)(leftHandIKMat[1] * *(float *)&leftHandOffsetLocal[8])
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[24]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[40]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[56]);
              v95 = (float)((float)((float)(leftHandIKMat[1] * *(float *)&leftHandOffsetLocal[12])
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[28]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[44]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[60]);
              v96 = (float)((float)((float)(leftHandIKMat[5] * *(float *)leftHandOffsetLocal)
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[16]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[32]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[48]);
              v97 = (float)((float)((float)(leftHandIKMat[5] * *(float *)&leftHandOffsetLocal[4])
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[20]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[36]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[52]);
              v98 = (float)((float)((float)(leftHandIKMat[5] * *(float *)&leftHandOffsetLocal[8])
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[24]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[40]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[56]);
              v99 = (float)((float)((float)(leftHandIKMat[5] * *(float *)&leftHandOffsetLocal[12])
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[28]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[44]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[60]);
              v100 = (float)((float)((float)(leftHandIKMat[9] * *(float *)leftHandOffsetLocal)
                                   + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[16]))
                           + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[32]))
                   + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[48]);
              v101 = (float)((float)((float)(leftHandIKMat[9] * *(float *)&leftHandOffsetLocal[4])
                                   + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[20]))
                           + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[36]))
                   + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[52]);
              v102 = (float)((float)((float)(leftHandIKMat[9] * *(float *)&leftHandOffsetLocal[8])
                                   + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[24]))
                           + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[40]))
                   + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[56]);
              v103 = (float)((float)((float)(leftHandIKMat[9] * *(float *)&leftHandOffsetLocal[12])
                                   + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[28]))
                           + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[44]))
                   + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[60]);
              v89 = ikState->handMats[0][0];
              *(_QWORD *)&ikState->handMats[0][0][0] = v90;
              *(_QWORD *)&ikState->handMats[0][0][2] = v91;
              v88 = v132 + 4;
              v2 = v132 + 4;
              v132[4] = v92;
              v2[1] = v93;
              v2[2] = v94;
              v2[3] = v95;
              v87 = v132 + 8;
              v3 = v132 + 8;
              v132[8] = v96;
              v3[1] = v97;
              v3[2] = v98;
              v3[3] = v99;
              v86 = v132 + 12;
              v4 = v132 + 12;
              v132[12] = v100;
              v4[1] = v101;
              v4[2] = v102;
              v4[3] = v103;
            }
            else
            {
              v85 = ikState->handMats[0][0];
              v84 = ikState->handMats[0][0];
              v76 = *(_QWORD *)&ikState->handMats[0][0][0];
              *(_QWORD *)v77 = *(_QWORD *)&ikState->handMats[0][0][2];
              v75 = ikState->handMats[0][1];
              *(float *)&v77[8] = ikState->handMats[0][1][0];
              v78 = *(_QWORD *)&ikState->handMats[0][1][1];
              *(float *)&v79 = ikState->handMats[0][1][3];
              *(unsigned int *)&v74[72] = ikState->handMats[0][2];
              HIDWORD(v79) = LODWORD(ikState->handMats[0][2][0]);
              v80 = *(_QWORD *)&ikState->handMats[0][2][1];
              *(float *)&v81 = ikState->handMats[0][2][3];
              *(unsigned int *)&v74[68] = ikState->handMats[0][3];
              HIDWORD(v81) = LODWORD(ikState->handMats[0][3][0]);
              v82 = *(_QWORD *)&ikState->handMats[0][3][1];
              v83 = ikState->handMats[0][3][3];
              v73 = v74;
              *(_QWORD *)v74 = v76;
              *(unsigned int *)&v74[8] = *(unsigned int *)v77;
              *(_QWORD *)&v74[12] = *(_QWORD *)&v77[4];
              v72 = &v74[16];
              *(_QWORD *)&v74[20] = v78;
              *(_QWORD *)&v74[28] = v79;
              v71 = &v74[32];
              *(_QWORD *)&v74[36] = v80;
              *(_QWORD *)&v74[44] = v81;
              v70 = &v74[48];
              *(_QWORD *)&v74[52] = v82;
              *(float *)&v74[60] = v83;
              v69 = &v174;
              v59 = v174;
              v60 = v175;
              v61 = v176;
              v62 = *(_QWORD *)leftHandIKMat;
              v58 = &leftHandIKMat[1];
              v63 = *(_QWORD *)&leftHandIKMat[2];
              v64 = *(_QWORD *)&leftHandIKMat[4];
              v57 = &leftHandIKMat[5];
              v65 = *(_QWORD *)&leftHandIKMat[6];
              v66 = *(_QWORD *)&leftHandIKMat[8];
              v56 = &leftHandIKMat[9];
              v67 = *(_QWORD *)&leftHandIKMat[10];
              v68 = leftHandIKMat[12];
              v55 = leftHandOffsetLocal;
              v45 = *(unsigned int *)leftHandOffsetLocal;
              v46 = *(unsigned int *)&leftHandOffsetLocal[4];
              v47 = *(unsigned int *)&leftHandOffsetLocal[8];
              v48 = *(_QWORD *)&leftHandOffsetLocal[12];
              v44 = &leftHandOffsetLocal[16];
              v49 = *(_QWORD *)&leftHandOffsetLocal[20];
              v50 = *(_QWORD *)&leftHandOffsetLocal[28];
              v43 = &leftHandOffsetLocal[32];
              v51 = *(_QWORD *)&leftHandOffsetLocal[36];
              v52 = *(_QWORD *)&leftHandOffsetLocal[44];
              v42 = &leftHandOffsetLocal[48];
              v53 = *(_QWORD *)&leftHandOffsetLocal[52];
              v54 = *(unsigned int *)&leftHandOffsetLocal[60];
              *(float *)&v26[72] = (float)((float)((float)(v174 * *(float *)leftHandOffsetLocal)
                                                 + (float)(v175 * *(float *)&leftHandOffsetLocal[16]))
                                         + (float)(v176 * *(float *)&leftHandOffsetLocal[32]))
                                 + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[48]);
              v27 = (float)((float)((float)(v174 * *(float *)&leftHandOffsetLocal[4])
                                  + (float)(v175 * *(float *)&leftHandOffsetLocal[20]))
                          + (float)(v176 * *(float *)&leftHandOffsetLocal[36]))
                  + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[52]);
              v28 = (float)((float)((float)(v174 * *(float *)&leftHandOffsetLocal[8])
                                  + (float)(v175 * *(float *)&leftHandOffsetLocal[24]))
                          + (float)(v176 * *(float *)&leftHandOffsetLocal[40]))
                  + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[56]);
              v29 = (float)((float)((float)(v174 * *(float *)&leftHandOffsetLocal[12])
                                  + (float)(v175 * *(float *)&leftHandOffsetLocal[28]))
                          + (float)(v176 * *(float *)&leftHandOffsetLocal[44]))
                  + (float)(leftHandIKMat[0] * *(float *)&leftHandOffsetLocal[60]);
              v30 = (float)((float)((float)(leftHandIKMat[1] * *(float *)leftHandOffsetLocal)
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[16]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[32]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[48]);
              v31 = (float)((float)((float)(leftHandIKMat[1] * *(float *)&leftHandOffsetLocal[4])
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[20]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[36]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[52]);
              v32 = (float)((float)((float)(leftHandIKMat[1] * *(float *)&leftHandOffsetLocal[8])
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[24]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[40]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[56]);
              v33 = (float)((float)((float)(leftHandIKMat[1] * *(float *)&leftHandOffsetLocal[12])
                                  + (float)(leftHandIKMat[2] * *(float *)&leftHandOffsetLocal[28]))
                          + (float)(leftHandIKMat[3] * *(float *)&leftHandOffsetLocal[44]))
                  + (float)(leftHandIKMat[4] * *(float *)&leftHandOffsetLocal[60]);
              v34 = (float)((float)((float)(leftHandIKMat[5] * *(float *)leftHandOffsetLocal)
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[16]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[32]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[48]);
              v35 = (float)((float)((float)(leftHandIKMat[5] * *(float *)&leftHandOffsetLocal[4])
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[20]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[36]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[52]);
              v36 = (float)((float)((float)(leftHandIKMat[5] * *(float *)&leftHandOffsetLocal[8])
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[24]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[40]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[56]);
              v37 = (float)((float)((float)(leftHandIKMat[5] * *(float *)&leftHandOffsetLocal[12])
                                  + (float)(leftHandIKMat[6] * *(float *)&leftHandOffsetLocal[28]))
                          + (float)(leftHandIKMat[7] * *(float *)&leftHandOffsetLocal[44]))
                  + (float)(leftHandIKMat[8] * *(float *)&leftHandOffsetLocal[60]);
              v38 = (float)((float)((float)(leftHandIKMat[9] * *(float *)leftHandOffsetLocal)
                                  + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[16]))
                          + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[32]))
                  + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[48]);
              v39 = (float)((float)((float)(leftHandIKMat[9] * *(float *)&leftHandOffsetLocal[4])
                                  + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[20]))
                          + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[36]))
                  + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[52]);
              v40 = (float)((float)((float)(leftHandIKMat[9] * *(float *)&leftHandOffsetLocal[8])
                                  + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[24]))
                          + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[40]))
                  + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[56]);
              v41 = (float)((float)((float)(leftHandIKMat[9] * *(float *)&leftHandOffsetLocal[12])
                                  + (float)(leftHandIKMat[10] * *(float *)&leftHandOffsetLocal[28]))
                          + (float)(leftHandIKMat[11] * *(float *)&leftHandOffsetLocal[44]))
                  + (float)(leftHandIKMat[12] * *(float *)&leftHandOffsetLocal[60]);
              v25 = v26;
              *(float *)v26 = *(float *)&v26[72];
              *(float *)&v26[4] = v27;
              *(float *)&v26[8] = v28;
              *(float *)&v26[12] = v29;
              v24 = &v26[16];
              *(float *)&v26[16] = v30;
              *(float *)&v26[20] = v31;
              *(float *)&v26[24] = v32;
              *(float *)&v26[28] = v33;
              v23 = &v26[32];
              *(float *)&v26[32] = v34;
              *(float *)&v26[36] = v35;
              *(float *)&v26[40] = v36;
              *(float *)&v26[44] = v37;
              v22 = &v26[48];
              *(float *)&v26[48] = v38;
              *(float *)&v26[52] = v39;
              *(float *)&v26[56] = v40;
              *(float *)&v26[60] = v41;
              ikMatrixLerp44(
                (float (*)[4])v74,
                (float (*)[4])v26,
                *(float *)&leftHandOffsetLocal[68],
                ikState->handMats[0]);
            }
            v21 = ikState->leftHandFromRightHandMat[0];
            v20 = ikState->handMats[1][0];
            v19 = ikState->handMats[0][0];
            if ( (float *)ikState->handMats[1] == v10
              && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
            {
              __debugbreak();
            }
            v10[0] = *v20;
            v10[1] = v20[4];
            v10[2] = v20[8];
            v10[3] = 0.0f;
            v11 = v20[1];
            v12 = v20[5];
            v13 = v20[9];
            v14 = 0;
            v15 = v20[2];
            v16 = v20[6];
            v17 = v20[10];
            v18 = 0;
            *v21 = (float)((float)(*v19 * v10[0]) + (float)(v19[1] * v11)) + (float)(v19[2] * v15);
            v21[1] = (float)((float)(*v19 * v10[1]) + (float)(v19[1] * v12)) + (float)(v19[2] * v16);
            v21[2] = (float)((float)(*v19 * v10[2]) + (float)(v19[1] * v13)) + (float)(v19[2] * v17);
            v21[3] = 0.0f;
            v21[4] = (float)((float)(v19[4] * v10[0]) + (float)(v19[5] * v11)) + (float)(v19[6] * v15);
            v21[5] = (float)((float)(v19[4] * v10[1]) + (float)(v19[5] * v12)) + (float)(v19[6] * v16);
            v21[6] = (float)((float)(v19[4] * v10[2]) + (float)(v19[5] * v13)) + (float)(v19[6] * v17);
            v21[7] = 0.0f;
            v21[8] = (float)((float)(v19[8] * v10[0]) + (float)(v19[9] * v11)) + (float)(v19[10] * v15);
            v21[9] = (float)((float)(v19[8] * v10[1]) + (float)(v19[9] * v12)) + (float)(v19[10] * v16);
            v21[10] = (float)((float)(v19[8] * v10[2]) + (float)(v19[9] * v13)) + (float)(v19[10] * v17);
            v21[11] = 0.0f;
            v9 = v20 + 12;
            v8 = v19 + 12;
            twistRot[1] = v19[12] - v20[12];
            twistRot[2] = v19[13] - v20[13];
            twistScale = v19[14] - v20[14];
            ikMatrixTransformVector34(&twistRot[1], (const float (*)[4])v10, v21 + 12);
            v21[15] = 1.0f;
            twistRot[0] = FLOAT_N0_89999998;
            v5[0] = 0.0f;
            v5[1] = 0.0f;
            v5[2] = (float)(*(float *)&leftHandOffsetLocal[68] * -0.89999998) * v194;
            ikRotateBone(&layerLerp, ikState, IKBONE_LWRIST_TWIST, v5, 1);
          }
        }
      }
    }
  }
}

void __cdecl IK_Layer_PlayerControllers(IKState *ikState)
{
  double v1; // st7
  float v2; // [esp+4h] [ebp-74h]
  float v3; // [esp+8h] [ebp-70h]
  float v4; // [esp+Ch] [ebp-6Ch]
  float v5; // [esp+10h] [ebp-68h]
  float v6; // [esp+14h] [ebp-64h]
  float v7; // [esp+1Ch] [ebp-5Ch]
  float v8; // [esp+20h] [ebp-58h]
  float rot_s[3]; // [esp+24h] [ebp-54h] BYREF
  int j; // [esp+30h] [ebp-48h]
  int i; // [esp+34h] [ebp-44h]
  float pelvisYawFeetScaleLeft[2]; // [esp+38h] [ebp-40h]
  float rot[3]; // [esp+40h] [ebp-38h] BYREF
  int ikLayerName; // [esp+4Ch] [ebp-2Ch]
  float pelvisYawFeetScaleRight[2]; // [esp+50h] [ebp-28h]
  float layerLerp; // [esp+58h] [ebp-20h]
  float pitchScalar; // [esp+5Ch] [ebp-1Ch]
  IKBoneNames ikControllerBones[6]; // [esp+60h] [ebp-18h]
  int savedregs; // [esp+78h] [ebp+0h] BYREF

  ikLayerName = 18;
  layerLerp = IKImport_GetLayerLerp(ikState, IKLAYER_PLAYER_CONTROLLERS, flt_E12728);
  if ( layerLerp > 0.0 )
  {
    ikControllerBones[0] = IKBONE_SPINE;
    ikControllerBones[1] = IKBONE_SPINE1;
    ikControllerBones[2] = IKBONE_SPINE2;
    ikControllerBones[3] = IKBONE_NECK;
    ikControllerBones[4] = IKBONE_HEAD;
    ikControllerBones[5] = IKBONE_PELVIS;
    if ( ikState->hasControllers )
    {
      v8 = IKImport_GetLayerLerp(ikState, IKLAYER_PRONE_ACTIVE);
      pitchScalar = (float)((float)(1.0 - v8) * pitchScalarDefault) + (float)(pitchScalarProne * v8);
      memset(rot, 0, sizeof(rot));
      for ( i = 0; i < 6; ++i )
      {
        rot[0] = ikState->controllers[i][0] * pitchScalar;
        if ( IKImport_GetLayerLerp(ikState, IKLAYER_PRONE_SLOW_FADE_ACTIVE) > 0.0 )
        {
          v1 = IKImport_GetLayerLerp(ikState, IKLAYER_PRONE_SLOW_FADE_ACTIVE);
          v7 = v1 + v1;
          if ( (float)(v7 - 1.0) < 0.0 )
            v2 = v1 + v1;
          else
            v2 = 1.0f;
          rot[0] = (float)(1.0 - v2) * rot[0];
        }
        ikRotateBone((float *)&savedregs, ikState, ikControllerBones[i], rot, 0);
      }
      for ( j = 0; j < 6; ++j )
      {
        rot[0] = ikState->controllers[j][0];
        rot[1] = ikState->controllers[j][1];
        rot[2] = ikState->controllers[j][2];
        rot[0] = 0.0f;
        if ( ikControllerBones[j] == IKBONE_PELVIS && IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE) > 0.0 )
        {
          v6 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
          rot_s[0] = v6 * rot[0];
          rot_s[1] = v6 * rot[1];
          rot_s[2] = v6 * rot[2];
          ikRotateBone((float *)&savedregs, ikState, IKBONE_SPINE, rot_s, 0);
          if ( IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE) < 1.0 )
          {
            v5 = 1.0 - IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
            rot_s[0] = v5 * rot[0];
            rot_s[1] = v5 * rot[1];
            rot_s[2] = v5 * rot[2];
            ikRotateBone((float *)&savedregs, ikState, IKBONE_PELVIS, rot_s, 0);
          }
        }
        else
        {
          ikRotateBone((float *)&savedregs, ikState, ikControllerBones[j], rot, 0);
        }
      }
      pelvisYawFeetScaleLeft[0] = 0.3f;
      pelvisYawFeetScaleLeft[1] = 0.3f;
      pelvisYawFeetScaleRight[0] = 0.6f;
      pelvisYawFeetScaleRight[1] = 0.6f;
      memset(rot, 0, sizeof(rot));
      v4 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
      rot[1] = (float)((float)((float)(1.0 - v4) * pelvisYawFeetScaleLeft[0]) + (float)(pelvisYawFeetScaleLeft[1] * v4))
             * ikState->controllers[5][1];
      ikRotateBone((float *)&savedregs, ikState, IKBONE_LFOOT, rot, 0);
      v3 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
      rot[1] = (float)((float)((float)(1.0 - v3) * pelvisYawFeetScaleRight[0]) + (float)(pelvisYawFeetScaleRight[1] * v3))
             * ikState->controllers[5][1];
      ikRotateBone((float *)&savedregs, ikState, IKBONE_RFOOT, rot, 0);
    }
  }
}

// local variable allocation has failed, the output may be wrong!
void  IK_Layer_PlayerPitch(float a1@<ebp>, IKState *ikState, bool preControllers)
{
  float v3; // xmm0_4
  double v4; // xmm0_8
  float v5; // xmm0_4
  float v6; // xmm0_4
  double v7; // xmm0_8
  float *v8; // ecx
  float *v9; // eax
  float *v10; // edx
  float *v11; // ecx
  float *v12; // eax
  float *v13; // edx
  long double v14; // [esp+8h] [ebp-4ECh]
  long double v15; // [esp+8h] [ebp-4ECh]
  long double v16; // [esp+10h] [ebp-4E4h] BYREF
  float v17; // [esp+18h] [ebp-4DCh]
  float v18; // [esp+1Ch] [ebp-4D8h]
  float *v19; // [esp+20h] [ebp-4D4h]
  float *v20; // [esp+24h] [ebp-4D0h]
  float v21[4]; // [esp+28h] [ebp-4CCh] BYREF
  float v22; // [esp+38h] [ebp-4BCh]
  float v23; // [esp+3Ch] [ebp-4B8h]
  float v24; // [esp+40h] [ebp-4B4h]
  int v25; // [esp+44h] [ebp-4B0h]
  float v26; // [esp+48h] [ebp-4ACh]
  float v27; // [esp+4Ch] [ebp-4A8h]
  float v28; // [esp+50h] [ebp-4A4h]
  int v29; // [esp+54h] [ebp-4A0h]
  float *v30; // [esp+6Ch] [ebp-488h]
  float *v31; // [esp+70h] [ebp-484h]
  float *v32; // [esp+74h] [ebp-480h]
  float *v33; // [esp+78h] [ebp-47Ch]
  float *v34; // [esp+7Ch] [ebp-478h]
  float *v35; // [esp+80h] [ebp-474h]
  float *v36; // [esp+84h] [ebp-470h]
  float *v37; // [esp+88h] [ebp-46Ch]
  float *v38; // [esp+8Ch] [ebp-468h]
  float *v39; // [esp+90h] [ebp-464h]
  float *v40; // [esp+94h] [ebp-460h]
  __int64 v41; // [esp+98h] [ebp-45Ch]
  __int64 v42; // [esp+A0h] [ebp-454h]
  float v43; // [esp+A8h] [ebp-44Ch]
  float v44; // [esp+ACh] [ebp-448h]
  float v45; // [esp+B0h] [ebp-444h]
  float v46; // [esp+B4h] [ebp-440h]
  float v47; // [esp+B8h] [ebp-43Ch]
  float v48; // [esp+BCh] [ebp-438h]
  float v49; // [esp+C0h] [ebp-434h]
  float v50; // [esp+C4h] [ebp-430h]
  float v51; // [esp+C8h] [ebp-42Ch]
  float v52; // [esp+CCh] [ebp-428h]
  float v53; // [esp+D0h] [ebp-424h]
  float v54; // [esp+D4h] [ebp-420h]
  float *v55; // [esp+DCh] [ebp-418h]
  float *v56; // [esp+E0h] [ebp-414h]
  float *v57; // [esp+E4h] [ebp-410h]
  __int64 v58; // [esp+E8h] [ebp-40Ch]
  __int64 v59; // [esp+F0h] [ebp-404h]
  __int64 v60; // [esp+F8h] [ebp-3FCh]
  __int64 v61; // [esp+100h] [ebp-3F4h]
  __int64 v62; // [esp+108h] [ebp-3ECh]
  __int64 v63; // [esp+110h] [ebp-3E4h]
  __int64 v64; // [esp+118h] [ebp-3DCh]
  __int64 v65; // [esp+120h] [ebp-3D4h]
  float *v66; // [esp+128h] [ebp-3CCh]
  float *v67; // [esp+12Ch] [ebp-3C8h]
  float *v68; // [esp+130h] [ebp-3C4h]
  float *v69; // [esp+134h] [ebp-3C0h]
  __int64 v70; // [esp+138h] [ebp-3BCh]
  __int64 v71; // [esp+140h] [ebp-3B4h]
  __int64 v72; // [esp+148h] [ebp-3ACh]
  __int64 v73; // [esp+150h] [ebp-3A4h]
  __int64 v74; // [esp+158h] [ebp-39Ch]
  __int64 v75; // [esp+160h] [ebp-394h]
  __int64 v76; // [esp+168h] [ebp-38Ch]
  __int64 v77; // [esp+170h] [ebp-384h]
  float *v78; // [esp+178h] [ebp-37Ch]
  float *v79; // [esp+17Ch] [ebp-378h]
  float *v80; // [esp+180h] [ebp-374h]
  float *v81; // [esp+184h] [ebp-370h]
  float *v82; // [esp+188h] [ebp-36Ch]
  float *v83; // [esp+18Ch] [ebp-368h]
  float *v84; // [esp+190h] [ebp-364h]
  float *v85; // [esp+194h] [ebp-360h]
  __int64 v86; // [esp+198h] [ebp-35Ch]
  __int64 v87; // [esp+1A0h] [ebp-354h]
  float v88; // [esp+1A8h] [ebp-34Ch]
  float v89; // [esp+1ACh] [ebp-348h]
  float v90; // [esp+1B0h] [ebp-344h]
  float v91; // [esp+1B4h] [ebp-340h]
  float v92; // [esp+1B8h] [ebp-33Ch]
  float v93; // [esp+1BCh] [ebp-338h]
  float v94; // [esp+1C0h] [ebp-334h]
  float v95; // [esp+1C4h] [ebp-330h]
  float v96; // [esp+1C8h] [ebp-32Ch]
  float v97; // [esp+1CCh] [ebp-328h]
  float v98; // [esp+1D0h] [ebp-324h]
  float v99; // [esp+1D4h] [ebp-320h]
  _BYTE *v100; // [esp+1DCh] [ebp-318h]
  _BYTE *v101; // [esp+1E0h] [ebp-314h]
  _BYTE *v102; // [esp+1E4h] [ebp-310h]
  int v103; // [esp+1E8h] [ebp-30Ch]
  int v104; // [esp+1ECh] [ebp-308h]
  int v105; // [esp+1F0h] [ebp-304h]
  __int64 v106; // [esp+1F4h] [ebp-300h]
  __int64 v107; // [esp+1FCh] [ebp-2F8h]
  __int64 v108; // [esp+204h] [ebp-2F0h]
  __int64 v109; // [esp+20Ch] [ebp-2E8h]
  __int64 v110; // [esp+214h] [ebp-2E0h]
  __int64 v111; // [esp+21Ch] [ebp-2D8h]
  int v112; // [esp+224h] [ebp-2D0h]
  _BYTE *v113; // [esp+228h] [ebp-2CCh]
  float *v114; // [esp+22Ch] [ebp-2C8h]
  float *v115; // [esp+230h] [ebp-2C4h]
  float *v116; // [esp+234h] [ebp-2C0h]
  float v117; // [esp+238h] [ebp-2BCh]
  float v118; // [esp+23Ch] [ebp-2B8h]
  float v119; // [esp+240h] [ebp-2B4h]
  __int64 v120; // [esp+244h] [ebp-2B0h]
  __int64 v121; // [esp+24Ch] [ebp-2A8h]
  __int64 v122; // [esp+254h] [ebp-2A0h]
  __int64 v123; // [esp+25Ch] [ebp-298h]
  __int64 v124; // [esp+264h] [ebp-290h]
  __int64 v125; // [esp+26Ch] [ebp-288h]
  float v126; // [esp+274h] [ebp-280h]
  float *v127; // [esp+278h] [ebp-27Ch]
  float *v128; // [esp+27Ch] [ebp-278h]
  float v129; // [esp+280h] [ebp-274h]
  float v130; // [esp+284h] [ebp-270h]
  float v131; // [esp+288h] [ebp-26Ch]
  float v132; // [esp+28Ch] [ebp-268h]
  float headPitchFrac; // [esp+290h] [ebp-264h]
  float v134; // [esp+294h] [ebp-260h] BYREF
  float neckPitchFrac; // [esp+298h] [ebp-25Ch]
  float headAndNeckPitchScale; // [esp+29Ch] [ebp-258h]
  float *v137; // [esp+2A0h] [ebp-254h]
  _BYTE *v138; // [esp+2A4h] [ebp-250h]
  float v139; // [esp+2A8h] [ebp-24Ch] BYREF
  float v140; // [esp+2ACh] [ebp-248h]
  float v141; // [esp+2B0h] [ebp-244h]
  float rightHandFromHead[17]; // [esp+2B4h] [ebp-240h] OVERLAPPED BYREF
  int v143; // [esp+2F8h] [ebp-1FCh]
  int v144; // [esp+2FCh] [ebp-1F8h]
  int v145; // [esp+300h] [ebp-1F4h]
  int v146; // [esp+304h] [ebp-1F0h]
  int v147; // [esp+308h] [ebp-1ECh]
  int v148; // [esp+30Ch] [ebp-1E8h]
  int v149; // [esp+310h] [ebp-1E4h]
  int v150; // [esp+314h] [ebp-1E0h]
  float *v151; // [esp+334h] [ebp-1C0h]
  _BYTE v152[64]; // [esp+338h] [ebp-1BCh] OVERLAPPED BYREF
  float *v153; // [esp+384h] [ebp-170h]
  float *v154; // [esp+388h] [ebp-16Ch]
  float *v155; // [esp+38Ch] [ebp-168h]
  float *v156; // [esp+390h] [ebp-164h]
  float v157; // [esp+394h] [ebp-160h] BYREF
  float v158; // [esp+398h] [ebp-15Ch]
  float v159; // [esp+39Ch] [ebp-158h]
  float weapOfs[3]; // [esp+3A0h] [ebp-154h]
  int v161; // [esp+3ACh] [ebp-148h] OVERLAPPED
  float v162; // [esp+3B0h] [ebp-144h]
  float v163; // [esp+3B4h] [ebp-140h]
  float v164; // [esp+3B8h] [ebp-13Ch] BYREF
  float v165; // [esp+3BCh] [ebp-138h]
  float fPitchPow; // [esp+3C0h] [ebp-134h]
  float rot[3]; // [esp+3C4h] [ebp-130h]
  float v168; // [esp+3D0h] [ebp-124h]
  float pelvisPitchScale; // [esp+3D4h] [ebp-120h]
  float *v170; // [esp+3D8h] [ebp-11Ch]
  float pelvisPitchScaleDn; // [esp+3DCh] [ebp-118h]
  float pelvisPitchScaleUp; // [esp+3E0h] [ebp-114h]
  float *v173; // [esp+3E4h] [ebp-110h]
  __int64 v174; // [esp+3E8h] [ebp-10Ch]
  float v175; // [esp+3F0h] [ebp-104h]
  float footVec[5]; // [esp+3F4h] [ebp-100h] BYREF
  float ofsTwistVec[3]; // [esp+408h] [ebp-ECh]
  float v178; // [esp+414h] [ebp-E0h]
  float v179; // [esp+418h] [ebp-DCh]
  float v180; // [esp+41Ch] [ebp-D8h] BYREF
  float twistScaleCap; // [esp+420h] [ebp-D4h]
  float maxTwist; // [esp+424h] [ebp-D0h]
  float ofsTwistCrouch[3]; // [esp+428h] [ebp-CCh] BYREF
  float ofsTwist[3]; // [esp+434h] [ebp-C0h] BYREF
  float ofsDnCrouch[3]; // [esp+440h] [ebp-B4h] BYREF
  float ofsDn[3]; // [esp+44Ch] [ebp-A8h] BYREF
  float v187; // [esp+458h] [ebp-9Ch]
  float ofsUpCrouch[3]; // [esp+45Ch] [ebp-98h] BYREF
  float ofsUp[3]; // [esp+468h] [ebp-8Ch]
  float fScale[3]; // [esp+474h] [ebp-80h] OVERLAPPED BYREF
  float axis[3][3]; // [esp+480h] [ebp-74h] BYREF
  float ofsVec[3]; // [esp+4A4h] [ebp-50h]
  float notMovingLerp; // [esp+4B0h] [ebp-44h]
  float v194; // [esp+4B4h] [ebp-40h]
  float v195; // [esp+4B8h] [ebp-3Ch]
  float v196; // [esp+4BCh] [ebp-38h]
  float movingLerp; // [esp+4C0h] [ebp-34h]
  float v198; // [esp+4C4h] [ebp-30h]
  float fraction; // [esp+4C8h] [ebp-2Ch]
  float v200; // [esp+4CCh] [ebp-28h] BYREF
  float xySpeed; // [esp+4D0h] [ebp-24h]
  float crouchLerp; // [esp+4D4h] [ebp-20h]
  float playerAngles[4]; // [esp+4D8h] [ebp-1Ch]
  float layerLerp; // [esp+4E8h] [ebp-Ch] BYREF
  int ikLayerName; // [esp+4ECh] [ebp-8h]
  int retaddr; // [esp+4F4h] [ebp+0h]

  layerLerp = a1;
  ikLayerName = retaddr;
  LODWORD(playerAngles[2]) = 13;
  playerAngles[1] = IKImport_GetLayerLerp(ikState, IKLAYER_PLAYER_PITCH, flt_E12714);
  if ( playerAngles[1] > 0.0 )
  {
    LODWORD(playerAngles[0]) = ikState->angles;
    v200 = ikState->angles[0];
    xySpeed = 0.0f;
    crouchLerp = 0.0f;
    fraction = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
    v198 = Vec2Length(ikState->velocity);
    movingLerp = v198 / velIdleMax;
    if ( (float)((float)(v198 / velIdleMax) - 1.0) < 0.0 )
      v196 = movingLerp;
    else
      v196 = 1.0f;
    if ( (float)(0.0 - movingLerp) < 0.0 )
      v195 = v196;
    else
      v195 = 0.0f;
    v194 = v195;
    notMovingLerp = IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING);
    ofsVec[2] = IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING_FAST);
    if ( (float)(notMovingLerp - ofsVec[2]) < 0.0 )
      v3 = ofsVec[2];
    else
      v3 = notMovingLerp;
    ofsVec[1] = v3;
    ofsVec[0] = v3;
    memset(axis[2], 0, sizeof(float[3]));
    AnglesToAxis(&v200, (float (*)[3])fScale);
    if ( !preControllers )
    {
      ofsUp[2] = v200;
      LODWORD(ofsUp[1]) = LODWORD(v200) & _mask__AbsFloat_;
      v4 = (float)(fabs(v200) / 90.0);
      __libm_sse2_pow(v14, v16);
      *(float *)&v4 = v4;
      ofsUp[0] = *(float *)&v4 * playerAngles[1];
      if ( v200 >= 0.0 )
      {
        ofsDnCrouch[0] = FLOAT_N4_5;
        ofsDnCrouch[1] = 0.0f;
        ofsDnCrouch[2] = FLOAT_N1_7;
        ofsTwist[0] = FLOAT_N6_5;
        ofsTwist[1] = 0.0f;
        ofsTwist[2] = FLOAT_8_5;
        Vec3Lerp(ofsDnCrouch, ofsTwist, fraction, axis[2]);
      }
      else
      {
        ofsUpCrouch[0] = 5.0f999999;
        ofsUpCrouch[1] = 0.0f;
        ofsUpCrouch[2] = FLOAT_N2_5;
        ofsDn[1] = 5.0f999999;
        ofsDn[2] = 0.0f;
        v187 = FLOAT_4_5;
        Vec3Lerp(ofsUpCrouch, &ofsDn[1], fraction, axis[2]);
        ofsDn[0] = 1.0 - IKImport_GetLayerLerp(ikState, IKLAYER_MINIGUN_ACTIVE);
        axis[2][0] = ofsDn[0] * axis[2][0];
        axis[2][1] = ofsDn[0] * axis[2][1];
        axis[2][2] = ofsDn[0] * axis[2][2];
      }
      axis[2][2] = axis[2][2] * ofsVec[0];
      axis[2][0] = ofsUp[0] * axis[2][0];
      axis[2][1] = ofsUp[0] * axis[2][1];
      axis[2][2] = ofsUp[0] * axis[2][2];
      ofsTwistCrouch[0] = 6.0f;
      ofsTwistCrouch[1] = 0.0f;
      ofsTwistCrouch[2] = -1.0f;
      v180 = 4.0f;
      twistScaleCap = 0.0f;
      maxTwist = -1.0f;
      v179 = 90.0f;
      v178 = 1.0f;
      ofsTwistVec[2] = ikState->controller_origin_angles[1] / 90.0;
      LODWORD(ofsTwistVec[1]) = LODWORD(ofsTwistVec[2]) & _mask__AbsFloat_;
      __libm_sse2_pow(v15, v16);
      LODWORD(ofsUp[0]) = LODWORD(ofsTwistVec[2]) & _mask__AbsFloat_;
      if ( (float)(1.0 - fabs(ofsTwistVec[2])) < 0.0 )
        v5 = v178;
      else
        v5 = ofsUp[0];
      ofsTwistVec[0] = v5;
      ofsUp[0] = v5;
      Vec3Lerp(ofsTwistCrouch, &v180, fraction, &footVec[2]);
      axis[2][0] = (float)(ofsUp[0] * footVec[2]) + axis[2][0];
      axis[2][1] = (float)(ofsUp[0] * footVec[3]) + axis[2][1];
      axis[2][2] = (float)(ofsUp[0] * footVec[4]) + axis[2][2];
      LODWORD(footVec[1]) = (*ikState->matArrayPostIK)[7];
      LODWORD(footVec[0]) = (*ikState->matArrayPostIK)[7];
      *(float *)LODWORD(footVec[1]) = *(float *)LODWORD(footVec[0]) + axis[2][0];
      *(float *)(LODWORD(footVec[1]) + 4) = *(float *)(LODWORD(footVec[0]) + 4) + axis[2][1];
      *(float *)(LODWORD(footVec[1]) + 8) = *(float *)(LODWORD(footVec[0]) + 8) + axis[2][2];
      ikState->bHasActiveLayers = 1;
      if ( ikState->cacheActive )
      {
        memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
        ikState->cacheActive = 0;
      }
      ikState->modifiedIKBones |= 2u;
      if ( v194 > 0.0 )
      {
        v174 = *(_QWORD *)&axis[2][0];
        v175 = 0.0f;
        v173 = ikState->footMats[0][3];
        LODWORD(pelvisPitchScaleUp) = ikState->footMats[0][3];
        ikState->footMats[0][3][0] = (float)(v194 * axis[2][0]) + ikState->footMats[0][3][0];
        v173[1] = (float)(v194 * *((float *)&v174 + 1)) + *(float *)(LODWORD(pelvisPitchScaleUp) + 4);
        v173[2] = (float)(v194 * v175) + *(float *)(LODWORD(pelvisPitchScaleUp) + 8);
        LODWORD(pelvisPitchScaleDn) = ikState->footMats[1][3];
        v170 = ikState->footMats[1][3];
        ikState->footMats[1][3][0] = (float)(v194 * *(float *)&v174) + ikState->footMats[1][3][0];
        *(float *)(LODWORD(pelvisPitchScaleDn) + 4) = (float)(v194 * *((float *)&v174 + 1)) + v170[1];
        *(float *)(LODWORD(pelvisPitchScaleDn) + 8) = (float)(v194 * v175) + v170[2];
      }
    }
    if ( preControllers )
    {
      pelvisPitchScale = 0.0f;
      v168 = 0.0f;
      if ( v200 > 0.0 )
        v6 = v168;
      else
        v6 = pelvisPitchScale;
      rot[2] = v6;
      rot[1] = (float)(1.0 - v194) * v6;
      if ( rot[1] > 0.0 )
      {
        rot[0] = playerAngles[1] * rot[1];
        v164 = (float)(playerAngles[1] * rot[1]) * v200;
        v165 = (float)(playerAngles[1] * rot[1]) * xySpeed;
        fPitchPow = (float)(playerAngles[1] * rot[1]) * crouchLerp;
        ikRotateBone(&layerLerp, ikState, IKBONE_PELVIS, &v164, 0);
      }
      v163 = FLOAT_1_1;
      v162 = v200;
      v161 = LODWORD(v200) & _mask__AbsFloat_;
      v7 = (float)(fabs(v200) / 90.0);
      __libm_sse2_pow(v14, v16);
      *(float *)&v7 = v7;
      weapOfs[2] = *(float *)&v7 * playerAngles[1];
      LODWORD(weapOfs[1]) = v200 <= 0.0;
      HIBYTE(weapOfs[0]) = v200 <= 0.0;
      IKImport_GetPitchWeaponOffset(ikState, SLOBYTE(weapOfs[1]), &v157);
      v157 = weapOfs[2] * v157;
      v158 = weapOfs[2] * v158;
      v159 = weapOfs[2] * v159;
      v156 = ikState->handMats[0][3];
      v155 = ikState->handMats[0][3];
      ikState->handMats[0][3][0] = ikState->handMats[0][3][0] + v157;
      v156[1] = v155[1] + v158;
      v156[2] = v155[2] + v159;
      v154 = ikState->handMats[1][3];
      v153 = ikState->handMats[1][3];
      ikState->handMats[1][3][0] = ikState->handMats[1][3][0] + v157;
      v154[1] = v153[1] + v158;
      v154[2] = v153[2] + v159;
      if ( v200 < 0.0 )
      {
        ikCalcBoneModelMatrix(ikState, 6, (*ikState->matArrayPostIK)[0], (float (*)[4])v152);
        v151 = ikState->handMats[1][0];
        if ( v152 == (_BYTE *)&rightHandFromHead[13]
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        rightHandFromHead[13] = *(float *)v152;
        rightHandFromHead[14] = *(float *)&v152[16];
        rightHandFromHead[15] = *(float *)&v152[32];
        rightHandFromHead[16] = 0.0f;
        v143 = *(unsigned int *)&v152[4];
        v144 = *(unsigned int *)&v152[20];
        v145 = *(unsigned int *)&v152[36];
        v146 = 0;
        v147 = *(unsigned int *)&v152[8];
        v148 = *(unsigned int *)&v152[24];
        v149 = *(unsigned int *)&v152[40];
        v150 = 0;
        v139 = (float)((float)(*v151 * *(float *)v152) + (float)(v151[1] * *(float *)&v152[4]))
             + (float)(v151[2] * *(float *)&v152[8]);
        v140 = (float)((float)(*v151 * *(float *)&v152[16]) + (float)(v151[1] * *(float *)&v152[20]))
             + (float)(v151[2] * *(float *)&v152[24]);
        v141 = (float)((float)(*v151 * *(float *)&v152[32]) + (float)(v151[1] * *(float *)&v152[36]))
             + (float)(v151[2] * *(float *)&v152[40]);
        rightHandFromHead[0] = 0.0f;
        rightHandFromHead[1] = (float)((float)(v151[4] * *(float *)v152) + (float)(v151[5] * *(float *)&v152[4]))
                             + (float)(v151[6] * *(float *)&v152[8]);
        rightHandFromHead[2] = (float)((float)(v151[4] * *(float *)&v152[16]) + (float)(v151[5] * *(float *)&v152[20]))
                             + (float)(v151[6] * *(float *)&v152[24]);
        rightHandFromHead[3] = (float)((float)(v151[4] * *(float *)&v152[32]) + (float)(v151[5] * *(float *)&v152[36]))
                             + (float)(v151[6] * *(float *)&v152[40]);
        rightHandFromHead[4] = 0.0f;
        rightHandFromHead[5] = (float)((float)(v151[8] * *(float *)v152) + (float)(v151[9] * *(float *)&v152[4]))
                             + (float)(v151[10] * *(float *)&v152[8]);
        rightHandFromHead[6] = (float)((float)(v151[8] * *(float *)&v152[16]) + (float)(v151[9] * *(float *)&v152[20]))
                             + (float)(v151[10] * *(float *)&v152[24]);
        rightHandFromHead[7] = (float)((float)(v151[8] * *(float *)&v152[32]) + (float)(v151[9] * *(float *)&v152[36]))
                             + (float)(v151[10] * *(float *)&v152[40]);
        rightHandFromHead[8] = 0.0f;
        v138 = &v152[48];
        v137 = v151 + 12;
        v134 = v151[12] - *(float *)&v152[48];
        neckPitchFrac = v151[13] - *(float *)&v152[52];
        headAndNeckPitchScale = v151[14] - *(float *)&v152[56];
        ikMatrixTransformVector34(&v134, (const float (*)[4])&rightHandFromHead[13], &rightHandFromHead[9]);
        rightHandFromHead[12] = 1.0f;
        headPitchFrac = 0.3f;
        v132 = 0.7f;
        v131 = (float)(playerAngles[1] * 0.69999999) * 0.30000001;
        v164 = v131 * v200;
        v165 = v131 * xySpeed;
        fPitchPow = v131 * crouchLerp;
        ikRotateBone(&layerLerp, ikState, IKBONE_NECK, &v164, 0);
        v130 = 0.3f;
        v129 = (float)(playerAngles[1] * 0.30000001) * 0.30000001;
        v164 = v129 * v200;
        v165 = v129 * xySpeed;
        fPitchPow = v129 * crouchLerp;
        ikRotateBone(&layerLerp, ikState, IKBONE_HEAD, &v164, 0);
        ikCalcBoneModelMatrix(ikState, 6, (*ikState->matArrayPostIK)[0], (float (*)[4])v152);
        v128 = ikState->handMats[1][0];
        v127 = &v139;
        v117 = v139;
        v118 = v140;
        v119 = v141;
        v120 = *(_QWORD *)rightHandFromHead;
        v116 = &rightHandFromHead[1];
        v121 = *(_QWORD *)&rightHandFromHead[2];
        v122 = *(_QWORD *)&rightHandFromHead[4];
        v115 = &rightHandFromHead[5];
        v123 = *(_QWORD *)&rightHandFromHead[6];
        v124 = *(_QWORD *)&rightHandFromHead[8];
        v114 = &rightHandFromHead[9];
        v125 = *(_QWORD *)&rightHandFromHead[10];
        v126 = rightHandFromHead[12];
        v113 = v152;
        v103 = *(unsigned int *)v152;
        v104 = *(unsigned int *)&v152[4];
        v105 = *(unsigned int *)&v152[8];
        v106 = *(_QWORD *)&v152[12];
        v102 = &v152[16];
        v107 = *(_QWORD *)&v152[20];
        v108 = *(_QWORD *)&v152[28];
        v101 = &v152[32];
        v109 = *(_QWORD *)&v152[36];
        v110 = *(_QWORD *)&v152[44];
        v100 = &v152[48];
        v111 = *(_QWORD *)&v152[52];
        v112 = *(unsigned int *)&v152[60];
        *(float *)&v86 = (float)((float)((float)(v139 * *(float *)v152) + (float)(v140 * *(float *)&v152[16]))
                               + (float)(v141 * *(float *)&v152[32]))
                       + (float)(rightHandFromHead[0] * *(float *)&v152[48]);
        *((float *)&v86 + 1) = (float)((float)((float)(v139 * *(float *)&v152[4]) + (float)(v140 * *(float *)&v152[20]))
                                     + (float)(v141 * *(float *)&v152[36]))
                             + (float)(rightHandFromHead[0] * *(float *)&v152[52]);
        *(float *)&v87 = (float)((float)((float)(v139 * *(float *)&v152[8]) + (float)(v140 * *(float *)&v152[24]))
                               + (float)(v141 * *(float *)&v152[40]))
                       + (float)(rightHandFromHead[0] * *(float *)&v152[56]);
        *((float *)&v87 + 1) = (float)((float)((float)(v139 * *(float *)&v152[12]) + (float)(v140 * *(float *)&v152[28]))
                                     + (float)(v141 * *(float *)&v152[44]))
                             + (float)(rightHandFromHead[0] * *(float *)&v152[60]);
        v88 = (float)((float)((float)(rightHandFromHead[1] * *(float *)v152)
                            + (float)(rightHandFromHead[2] * *(float *)&v152[16]))
                    + (float)(rightHandFromHead[3] * *(float *)&v152[32]))
            + (float)(rightHandFromHead[4] * *(float *)&v152[48]);
        v89 = (float)((float)((float)(rightHandFromHead[1] * *(float *)&v152[4])
                            + (float)(rightHandFromHead[2] * *(float *)&v152[20]))
                    + (float)(rightHandFromHead[3] * *(float *)&v152[36]))
            + (float)(rightHandFromHead[4] * *(float *)&v152[52]);
        v90 = (float)((float)((float)(rightHandFromHead[1] * *(float *)&v152[8])
                            + (float)(rightHandFromHead[2] * *(float *)&v152[24]))
                    + (float)(rightHandFromHead[3] * *(float *)&v152[40]))
            + (float)(rightHandFromHead[4] * *(float *)&v152[56]);
        v91 = (float)((float)((float)(rightHandFromHead[1] * *(float *)&v152[12])
                            + (float)(rightHandFromHead[2] * *(float *)&v152[28]))
                    + (float)(rightHandFromHead[3] * *(float *)&v152[44]))
            + (float)(rightHandFromHead[4] * *(float *)&v152[60]);
        v92 = (float)((float)((float)(rightHandFromHead[5] * *(float *)v152)
                            + (float)(rightHandFromHead[6] * *(float *)&v152[16]))
                    + (float)(rightHandFromHead[7] * *(float *)&v152[32]))
            + (float)(rightHandFromHead[8] * *(float *)&v152[48]);
        v93 = (float)((float)((float)(rightHandFromHead[5] * *(float *)&v152[4])
                            + (float)(rightHandFromHead[6] * *(float *)&v152[20]))
                    + (float)(rightHandFromHead[7] * *(float *)&v152[36]))
            + (float)(rightHandFromHead[8] * *(float *)&v152[52]);
        v94 = (float)((float)((float)(rightHandFromHead[5] * *(float *)&v152[8])
                            + (float)(rightHandFromHead[6] * *(float *)&v152[24]))
                    + (float)(rightHandFromHead[7] * *(float *)&v152[40]))
            + (float)(rightHandFromHead[8] * *(float *)&v152[56]);
        v95 = (float)((float)((float)(rightHandFromHead[5] * *(float *)&v152[12])
                            + (float)(rightHandFromHead[6] * *(float *)&v152[28]))
                    + (float)(rightHandFromHead[7] * *(float *)&v152[44]))
            + (float)(rightHandFromHead[8] * *(float *)&v152[60]);
        v96 = (float)((float)((float)(rightHandFromHead[9] * *(float *)v152)
                            + (float)(rightHandFromHead[10] * *(float *)&v152[16]))
                    + (float)(rightHandFromHead[11] * *(float *)&v152[32]))
            + (float)(rightHandFromHead[12] * *(float *)&v152[48]);
        v97 = (float)((float)((float)(rightHandFromHead[9] * *(float *)&v152[4])
                            + (float)(rightHandFromHead[10] * *(float *)&v152[20]))
                    + (float)(rightHandFromHead[11] * *(float *)&v152[36]))
            + (float)(rightHandFromHead[12] * *(float *)&v152[52]);
        v98 = (float)((float)((float)(rightHandFromHead[9] * *(float *)&v152[8])
                            + (float)(rightHandFromHead[10] * *(float *)&v152[24]))
                    + (float)(rightHandFromHead[11] * *(float *)&v152[40]))
            + (float)(rightHandFromHead[12] * *(float *)&v152[56]);
        v99 = (float)((float)((float)(rightHandFromHead[9] * *(float *)&v152[12])
                            + (float)(rightHandFromHead[10] * *(float *)&v152[28]))
                    + (float)(rightHandFromHead[11] * *(float *)&v152[44]))
            + (float)(rightHandFromHead[12] * *(float *)&v152[60]);
        v85 = ikState->handMats[1][0];
        *(_QWORD *)&ikState->handMats[1][0][0] = v86;
        *(_QWORD *)&ikState->handMats[1][0][2] = v87;
        v84 = v128 + 4;
        v8 = v128 + 4;
        v128[4] = v88;
        v8[1] = v89;
        v8[2] = v90;
        v8[3] = v91;
        v83 = v128 + 8;
        v9 = v128 + 8;
        v128[8] = v92;
        v9[1] = v93;
        v9[2] = v94;
        v9[3] = v95;
        v82 = v128 + 12;
        v10 = v128 + 12;
        v128[12] = v96;
        v10[1] = v97;
        v10[2] = v98;
        v10[3] = v99;
      }
      v81 = ikState->handMats[0][0];
      v80 = ikState->handMats[1][0];
      v79 = ikState->leftHandFromRightHandMat[0];
      v78 = ikState->leftHandFromRightHandMat[0];
      v70 = *(_QWORD *)&ikState->leftHandFromRightHandMat[0][0];
      v71 = *(_QWORD *)&ikState->leftHandFromRightHandMat[0][2];
      v69 = ikState->leftHandFromRightHandMat[1];
      v72 = *(_QWORD *)&ikState->leftHandFromRightHandMat[1][0];
      v73 = *(_QWORD *)&ikState->leftHandFromRightHandMat[1][2];
      v68 = ikState->leftHandFromRightHandMat[2];
      v74 = *(_QWORD *)&ikState->leftHandFromRightHandMat[2][0];
      v75 = *(_QWORD *)&ikState->leftHandFromRightHandMat[2][2];
      v67 = ikState->leftHandFromRightHandMat[3];
      v76 = *(_QWORD *)&ikState->leftHandFromRightHandMat[3][0];
      v77 = *(_QWORD *)&ikState->leftHandFromRightHandMat[3][2];
      v66 = ikState->handMats[1][0];
      v58 = *(_QWORD *)&ikState->handMats[1][0][0];
      v59 = *(_QWORD *)&ikState->handMats[1][0][2];
      v57 = ikState->handMats[1][1];
      v60 = *(_QWORD *)&ikState->handMats[1][1][0];
      v61 = *(_QWORD *)&ikState->handMats[1][1][2];
      v56 = ikState->handMats[1][2];
      v62 = *(_QWORD *)&ikState->handMats[1][2][0];
      v63 = *(_QWORD *)&ikState->handMats[1][2][2];
      v55 = ikState->handMats[1][3];
      v64 = *(_QWORD *)&ikState->handMats[1][3][0];
      v65 = *(_QWORD *)&ikState->handMats[1][3][2];
      *(float *)&v41 = (float)((float)((float)(*(float *)&v70 * *(float *)&v58)
                                     + (float)(*((float *)&v70 + 1) * *(float *)&v60))
                             + (float)(*(float *)&v71 * *(float *)&v62))
                     + (float)(*((float *)&v71 + 1) * *(float *)&v64);
      *((float *)&v41 + 1) = (float)((float)((float)(*(float *)&v70 * *((float *)&v58 + 1))
                                           + (float)(*((float *)&v70 + 1) * *((float *)&v60 + 1)))
                                   + (float)(*(float *)&v71 * *((float *)&v62 + 1)))
                           + (float)(*((float *)&v71 + 1) * *((float *)&v64 + 1));
      *(float *)&v42 = (float)((float)((float)(*(float *)&v70 * *(float *)&v59)
                                     + (float)(*((float *)&v70 + 1) * *(float *)&v61))
                             + (float)(*(float *)&v71 * *(float *)&v63))
                     + (float)(*((float *)&v71 + 1) * *(float *)&v65);
      *((float *)&v42 + 1) = (float)((float)((float)(*(float *)&v70 * *((float *)&v59 + 1))
                                           + (float)(*((float *)&v70 + 1) * *((float *)&v61 + 1)))
                                   + (float)(*(float *)&v71 * *((float *)&v63 + 1)))
                           + (float)(*((float *)&v71 + 1) * *((float *)&v65 + 1));
      v43 = (float)((float)((float)(*(float *)&v72 * *(float *)&v58) + (float)(*((float *)&v72 + 1) * *(float *)&v60))
                  + (float)(*(float *)&v73 * *(float *)&v62))
          + (float)(*((float *)&v73 + 1) * *(float *)&v64);
      v44 = (float)((float)((float)(*(float *)&v72 * *((float *)&v58 + 1))
                          + (float)(*((float *)&v72 + 1) * *((float *)&v60 + 1)))
                  + (float)(*(float *)&v73 * *((float *)&v62 + 1)))
          + (float)(*((float *)&v73 + 1) * *((float *)&v64 + 1));
      v45 = (float)((float)((float)(*(float *)&v72 * *(float *)&v59) + (float)(*((float *)&v72 + 1) * *(float *)&v61))
                  + (float)(*(float *)&v73 * *(float *)&v63))
          + (float)(*((float *)&v73 + 1) * *(float *)&v65);
      v46 = (float)((float)((float)(*(float *)&v72 * *((float *)&v59 + 1))
                          + (float)(*((float *)&v72 + 1) * *((float *)&v61 + 1)))
                  + (float)(*(float *)&v73 * *((float *)&v63 + 1)))
          + (float)(*((float *)&v73 + 1) * *((float *)&v65 + 1));
      v47 = (float)((float)((float)(*(float *)&v74 * *(float *)&v58) + (float)(*((float *)&v74 + 1) * *(float *)&v60))
                  + (float)(*(float *)&v75 * *(float *)&v62))
          + (float)(*((float *)&v75 + 1) * *(float *)&v64);
      v48 = (float)((float)((float)(*(float *)&v74 * *((float *)&v58 + 1))
                          + (float)(*((float *)&v74 + 1) * *((float *)&v60 + 1)))
                  + (float)(*(float *)&v75 * *((float *)&v62 + 1)))
          + (float)(*((float *)&v75 + 1) * *((float *)&v64 + 1));
      v49 = (float)((float)((float)(*(float *)&v74 * *(float *)&v59) + (float)(*((float *)&v74 + 1) * *(float *)&v61))
                  + (float)(*(float *)&v75 * *(float *)&v63))
          + (float)(*((float *)&v75 + 1) * *(float *)&v65);
      v50 = (float)((float)((float)(*(float *)&v74 * *((float *)&v59 + 1))
                          + (float)(*((float *)&v74 + 1) * *((float *)&v61 + 1)))
                  + (float)(*(float *)&v75 * *((float *)&v63 + 1)))
          + (float)(*((float *)&v75 + 1) * *((float *)&v65 + 1));
      v51 = (float)((float)((float)(*(float *)&v76 * *(float *)&v58) + (float)(*((float *)&v76 + 1) * *(float *)&v60))
                  + (float)(*(float *)&v77 * *(float *)&v62))
          + (float)(*((float *)&v77 + 1) * *(float *)&v64);
      v52 = (float)((float)((float)(*(float *)&v76 * *((float *)&v58 + 1))
                          + (float)(*((float *)&v76 + 1) * *((float *)&v60 + 1)))
                  + (float)(*(float *)&v77 * *((float *)&v62 + 1)))
          + (float)(*((float *)&v77 + 1) * *((float *)&v64 + 1));
      v53 = (float)((float)((float)(*(float *)&v76 * *(float *)&v59) + (float)(*((float *)&v76 + 1) * *(float *)&v61))
                  + (float)(*(float *)&v77 * *(float *)&v63))
          + (float)(*((float *)&v77 + 1) * *(float *)&v65);
      v54 = (float)((float)((float)(*(float *)&v76 * *((float *)&v59 + 1))
                          + (float)(*((float *)&v76 + 1) * *((float *)&v61 + 1)))
                  + (float)(*(float *)&v77 * *((float *)&v63 + 1)))
          + (float)(*((float *)&v77 + 1) * *((float *)&v65 + 1));
      v40 = ikState->handMats[0][0];
      *(_QWORD *)&ikState->handMats[0][0][0] = v41;
      *(_QWORD *)&ikState->handMats[0][0][2] = v42;
      v39 = v81 + 4;
      v11 = v81 + 4;
      v81[4] = v43;
      v11[1] = v44;
      v11[2] = v45;
      v11[3] = v46;
      v38 = v81 + 8;
      v12 = v81 + 8;
      v81[8] = v47;
      v12[1] = v48;
      v12[2] = v49;
      v12[3] = v50;
      v37 = v81 + 12;
      v13 = v81 + 12;
      v81[12] = v51;
      v13[1] = v52;
      v13[2] = v53;
      v13[3] = v54;
    }
    v36 = ikState->handMats[0][3];
    v35 = ikState->handMats[0][3];
    ikState->handMats[0][3][0] = ikState->handMats[0][3][0] + axis[2][0];
    v36[1] = v35[1] + axis[2][1];
    v36[2] = v35[2] + axis[2][2];
    v34 = ikState->handMats[1][3];
    v33 = ikState->handMats[1][3];
    ikState->handMats[1][3][0] = ikState->handMats[1][3][0] + axis[2][0];
    v34[1] = v33[1] + axis[2][1];
    v34[2] = v33[2] + axis[2][2];
    v32 = ikState->leftHandFromRightHandMat[0];
    v31 = ikState->handMats[1][0];
    v30 = ikState->handMats[0][0];
    if ( (float *)ikState->handMats[1] == v21
      && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
    {
      __debugbreak();
    }
    v21[0] = *v31;
    v21[1] = v31[4];
    v21[2] = v31[8];
    v21[3] = 0.0f;
    v22 = v31[1];
    v23 = v31[5];
    v24 = v31[9];
    v25 = 0;
    v26 = v31[2];
    v27 = v31[6];
    v28 = v31[10];
    v29 = 0;
    *v32 = (float)((float)(*v30 * v21[0]) + (float)(v30[1] * v22)) + (float)(v30[2] * v26);
    v32[1] = (float)((float)(*v30 * v21[1]) + (float)(v30[1] * v23)) + (float)(v30[2] * v27);
    v32[2] = (float)((float)(*v30 * v21[2]) + (float)(v30[1] * v24)) + (float)(v30[2] * v28);
    v32[3] = 0.0f;
    v32[4] = (float)((float)(v30[4] * v21[0]) + (float)(v30[5] * v22)) + (float)(v30[6] * v26);
    v32[5] = (float)((float)(v30[4] * v21[1]) + (float)(v30[5] * v23)) + (float)(v30[6] * v27);
    v32[6] = (float)((float)(v30[4] * v21[2]) + (float)(v30[5] * v24)) + (float)(v30[6] * v28);
    v32[7] = 0.0f;
    v32[8] = (float)((float)(v30[8] * v21[0]) + (float)(v30[9] * v22)) + (float)(v30[10] * v26);
    v32[9] = (float)((float)(v30[8] * v21[1]) + (float)(v30[9] * v23)) + (float)(v30[10] * v27);
    v32[10] = (float)((float)(v30[8] * v21[2]) + (float)(v30[9] * v24)) + (float)(v30[10] * v28);
    v32[11] = 0.0f;
    v20 = v31 + 12;
    v19 = v30 + 12;
    *((float *)&v16 + 1) = v30[12] - v31[12];
    v17 = v30[13] - v31[13];
    v18 = v30[14] - v31[14];
    ikMatrixTransformVector34((const float *)&v16 + 1, (const float (*)[4])v21, v32 + 12);
    v32[15] = 1.0f;
  }
}

// local variable allocation has failed, the output may be wrong!
void  IK_Layer_PreventHeadClip(float a1@<ebp>, IKState *ikState)
{
  float v2; // [esp+1Ch] [ebp-2D8h]
  float v3; // [esp+28h] [ebp-2CCh]
  float v4; // [esp+34h] [ebp-2C0h]
  float v5; // [esp+40h] [ebp-2B4h]
  float v6; // [esp+4Ch] [ebp-2A8h]
  float *v7; // [esp+50h] [ebp-2A4h]
  float v8[5]; // [esp+54h] [ebp-2A0h] BYREF
  float v9; // [esp+68h] [ebp-28Ch]
  float v10; // [esp+6Ch] [ebp-288h]
  float v11; // [esp+70h] [ebp-284h]
  float torsoFromLegsMat[17]; // [esp+74h] [ebp-280h] OVERLAPPED BYREF
  float v13; // [esp+B8h] [ebp-23Ch]
  float v14; // [esp+BCh] [ebp-238h]
  float v15; // [esp+C0h] [ebp-234h]
  int v16; // [esp+C4h] [ebp-230h]
  float v17; // [esp+C8h] [ebp-22Ch]
  float v18; // [esp+CCh] [ebp-228h]
  float v19; // [esp+D0h] [ebp-224h]
  int v20; // [esp+D4h] [ebp-220h]
  float *v21; // [esp+E8h] [ebp-20Ch]
  float *v22; // [esp+ECh] [ebp-208h]
  float v23; // [esp+F0h] [ebp-204h]
  float v24; // [esp+F4h] [ebp-200h] BYREF
  float traceHitPos[3]; // [esp+100h] [ebp-1F4h] BYREF
  float traceHitNormal[3]; // [esp+10Ch] [ebp-1E8h] BYREF
  float end[3]; // [esp+118h] [ebp-1DCh] BYREF
  float start[3]; // [esp+124h] [ebp-1D0h]
  float fwd[3]; // [esp+130h] [ebp-1C4h] BYREF
  float traceHitFraction; // [esp+13Ch] [ebp-1B8h]
  unsigned int headCenterPointWorld[6]; // [esp+140h] [ebp-1B4h]
  float v32[3]; // [esp+158h] [ebp-19Ch] BYREF
  float headMatWorld[4][4]; // [esp+164h] [ebp-190h] BYREF
  float v34; // [esp+1A4h] [ebp-150h]
  float v35; // [esp+1A8h] [ebp-14Ch]
  float v36; // [esp+1ACh] [ebp-148h]
  float v37; // [esp+1B0h] [ebp-144h]
  float v38; // [esp+1B4h] [ebp-140h]
  float v39; // [esp+1B8h] [ebp-13Ch]
  float v40; // [esp+1BCh] [ebp-138h]
  float v41; // [esp+1C0h] [ebp-134h]
  float v42; // [esp+1C4h] [ebp-130h]
  float v43; // [esp+1C8h] [ebp-12Ch]
  float v44; // [esp+1CCh] [ebp-128h]
  float v45; // [esp+1D0h] [ebp-124h]
  float v46; // [esp+1D4h] [ebp-120h]
  float *v47; // [esp+1DCh] [ebp-118h]
  float *v48; // [esp+1E0h] [ebp-114h]
  float *v49; // [esp+1E4h] [ebp-110h]
  __int64 v50; // [esp+1E8h] [ebp-10Ch]
  __int64 v51; // [esp+1F0h] [ebp-104h]
  __int64 v52; // [esp+1F8h] [ebp-FCh]
  __int64 v53; // [esp+200h] [ebp-F4h]
  __int64 v54; // [esp+208h] [ebp-ECh]
  __int64 v55; // [esp+210h] [ebp-E4h]
  __int64 v56; // [esp+218h] [ebp-DCh]
  __int64 v57; // [esp+220h] [ebp-D4h]
  float *v58; // [esp+228h] [ebp-CCh]
  _BYTE *v59; // [esp+22Ch] [ebp-C8h]
  _BYTE *v60; // [esp+230h] [ebp-C4h]
  _BYTE *v61; // [esp+234h] [ebp-C0h]
  int v62; // [esp+238h] [ebp-BCh]
  int v63; // [esp+23Ch] [ebp-B8h]
  int v64; // [esp+240h] [ebp-B4h]
  __int64 v65; // [esp+244h] [ebp-B0h]
  __int64 v66; // [esp+24Ch] [ebp-A8h]
  __int64 v67; // [esp+254h] [ebp-A0h]
  __int64 v68; // [esp+25Ch] [ebp-98h]
  __int64 v69; // [esp+264h] [ebp-90h]
  __int64 v70; // [esp+26Ch] [ebp-88h]
  int v71; // [esp+274h] [ebp-80h]
  _BYTE *v72; // [esp+278h] [ebp-7Ch]
  float *v73; // [esp+27Ch] [ebp-78h]
  _BYTE *v74; // [esp+280h] [ebp-74h]
  float v75; // [esp+284h] [ebp-70h]
  _BYTE *v76; // [esp+288h] [ebp-6Ch]
  float v77; // [esp+28Ch] [ebp-68h]
  float v78; // [esp+290h] [ebp-64h]
  float boxRadius; // [esp+294h] [ebp-60h]
  _BYTE headClipPlayerCollisionHullRadius[76]; // [esp+298h] [ebp-5Ch] OVERLAPPED BYREF
  float layerLerp; // [esp+2E8h] [ebp-Ch]
  int ikLayerName; // [esp+2ECh] [ebp-8h]
  int retaddr; // [esp+2F4h] [ebp+0h]

  layerLerp = a1;
  ikLayerName = retaddr;
  *(unsigned int *)&headClipPlayerCollisionHullRadius[72] = 20;
  *(float *)&headClipPlayerCollisionHullRadius[68] = IKImport_GetLayerLerp(
                                                       ikState,
                                                       IKLAYER_PREVENT_HEAD_CLIP,
                                                       flt_E12730);
  if ( *(float *)&headClipPlayerCollisionHullRadius[68] > 0.0 )
  {
    ikCalcBoneModelMatrix(ikState, 6, (*ikState->matArrayPostIK)[0], (float (*)[4])headClipPlayerCollisionHullRadius);
    boxRadius = 0.0f;
    v78 = 12.0f;
    v77 = FLOAT_14_0;
    if ( Vec2Length((const float *)&headClipPlayerCollisionHullRadius[48]) > 14.0 - 0.0 )
    {
      Vec2Normalize((float *)&headClipPlayerCollisionHullRadius[48]);
      v76 = &headClipPlayerCollisionHullRadius[48];
      v75 = v77 - boxRadius;
      v74 = &headClipPlayerCollisionHullRadius[48];
      *(float *)&headClipPlayerCollisionHullRadius[48] = (float)(v77 - boxRadius)
                                                       * *(float *)&headClipPlayerCollisionHullRadius[48];
      *(float *)&headClipPlayerCollisionHullRadius[52] = (float)(v77 - boxRadius)
                                                       * *(float *)&headClipPlayerCollisionHullRadius[52];
    }
    v73 = ikState->entityLegsXform[0];
    v72 = headClipPlayerCollisionHullRadius;
    v62 = *(unsigned int *)headClipPlayerCollisionHullRadius;
    v63 = *(unsigned int *)&headClipPlayerCollisionHullRadius[4];
    v64 = *(unsigned int *)&headClipPlayerCollisionHullRadius[8];
    v65 = *(_QWORD *)&headClipPlayerCollisionHullRadius[12];
    v61 = &headClipPlayerCollisionHullRadius[16];
    v66 = *(_QWORD *)&headClipPlayerCollisionHullRadius[20];
    v67 = *(_QWORD *)&headClipPlayerCollisionHullRadius[28];
    v60 = &headClipPlayerCollisionHullRadius[32];
    v68 = *(_QWORD *)&headClipPlayerCollisionHullRadius[36];
    v69 = *(_QWORD *)&headClipPlayerCollisionHullRadius[44];
    v59 = &headClipPlayerCollisionHullRadius[48];
    v70 = *(_QWORD *)&headClipPlayerCollisionHullRadius[52];
    v71 = *(unsigned int *)&headClipPlayerCollisionHullRadius[60];
    v58 = ikState->entityLegsXform[0];
    v50 = *(_QWORD *)&ikState->entityLegsXform[0][0];
    v51 = *(_QWORD *)&ikState->entityLegsXform[0][2];
    v49 = ikState->entityLegsXform[1];
    v52 = *(_QWORD *)&ikState->entityLegsXform[1][0];
    v53 = *(_QWORD *)&ikState->entityLegsXform[1][2];
    v48 = ikState->entityLegsXform[2];
    v54 = *(_QWORD *)&ikState->entityLegsXform[2][0];
    v55 = *(_QWORD *)&ikState->entityLegsXform[2][2];
    v47 = ikState->entityLegsXform[3];
    v56 = *(_QWORD *)&ikState->entityLegsXform[3][0];
    v57 = *(_QWORD *)&ikState->entityLegsXform[3][2];
    headMatWorld[3][1] = (float)((float)((float)(*(float *)headClipPlayerCollisionHullRadius * *(float *)&v50)
                                       + (float)(*(float *)&headClipPlayerCollisionHullRadius[4] * *(float *)&v52))
                               + (float)(*(float *)&headClipPlayerCollisionHullRadius[8] * *(float *)&v54))
                       + (float)(*(float *)&headClipPlayerCollisionHullRadius[12] * *(float *)&v56);
    headMatWorld[3][2] = (float)((float)((float)(*(float *)headClipPlayerCollisionHullRadius * *((float *)&v50 + 1))
                                       + (float)(*(float *)&headClipPlayerCollisionHullRadius[4] * *((float *)&v52 + 1)))
                               + (float)(*(float *)&headClipPlayerCollisionHullRadius[8] * *((float *)&v54 + 1)))
                       + (float)(*(float *)&headClipPlayerCollisionHullRadius[12] * *((float *)&v56 + 1));
    headMatWorld[3][3] = (float)((float)((float)(*(float *)headClipPlayerCollisionHullRadius * *(float *)&v51)
                                       + (float)(*(float *)&headClipPlayerCollisionHullRadius[4] * *(float *)&v53))
                               + (float)(*(float *)&headClipPlayerCollisionHullRadius[8] * *(float *)&v55))
                       + (float)(*(float *)&headClipPlayerCollisionHullRadius[12] * *(float *)&v57);
    v34 = (float)((float)((float)(*(float *)headClipPlayerCollisionHullRadius * *((float *)&v51 + 1))
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[4] * *((float *)&v53 + 1)))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[8] * *((float *)&v55 + 1)))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[12] * *((float *)&v57 + 1));
    v35 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[16] * *(float *)&v50)
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[20] * *(float *)&v52))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[24] * *(float *)&v54))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[28] * *(float *)&v56);
    v36 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[16] * *((float *)&v50 + 1))
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[20] * *((float *)&v52 + 1)))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[24] * *((float *)&v54 + 1)))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[28] * *((float *)&v56 + 1));
    v37 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[16] * *(float *)&v51)
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[20] * *(float *)&v53))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[24] * *(float *)&v55))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[28] * *(float *)&v57);
    v38 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[16] * *((float *)&v51 + 1))
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[20] * *((float *)&v53 + 1)))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[24] * *((float *)&v55 + 1)))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[28] * *((float *)&v57 + 1));
    v39 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[32] * *(float *)&v50)
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[36] * *(float *)&v52))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[40] * *(float *)&v54))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[44] * *(float *)&v56);
    v40 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[32] * *((float *)&v50 + 1))
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[36] * *((float *)&v52 + 1)))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[40] * *((float *)&v54 + 1)))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[44] * *((float *)&v56 + 1));
    v41 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[32] * *(float *)&v51)
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[36] * *(float *)&v53))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[40] * *(float *)&v55))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[44] * *(float *)&v57);
    v42 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[32] * *((float *)&v51 + 1))
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[36] * *((float *)&v53 + 1)))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[40] * *((float *)&v55 + 1)))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[44] * *((float *)&v57 + 1));
    v43 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[48] * *(float *)&v50)
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[52] * *(float *)&v52))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[56] * *(float *)&v54))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[60] * *(float *)&v56);
    v44 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[48] * *((float *)&v50 + 1))
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[52] * *((float *)&v52 + 1)))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[56] * *((float *)&v54 + 1)))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[60] * *((float *)&v56 + 1));
    v45 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[48] * *(float *)&v51)
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[52] * *(float *)&v53))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[56] * *(float *)&v55))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[60] * *(float *)&v57);
    v46 = (float)((float)((float)(*(float *)&headClipPlayerCollisionHullRadius[48] * *((float *)&v51 + 1))
                        + (float)(*(float *)&headClipPlayerCollisionHullRadius[52] * *((float *)&v53 + 1)))
                + (float)(*(float *)&headClipPlayerCollisionHullRadius[56] * *((float *)&v55 + 1)))
        + (float)(*(float *)&headClipPlayerCollisionHullRadius[60] * *((float *)&v57 + 1));
    headCenterPointWorld[5] = v32;
    v32[0] = headMatWorld[3][1];
    v32[1] = headMatWorld[3][2];
    v32[2] = headMatWorld[3][3];
    headMatWorld[0][0] = v34;
    headCenterPointWorld[4] = &headMatWorld[0][1];
    headMatWorld[0][1] = v35;
    headMatWorld[0][2] = v36;
    headMatWorld[0][3] = v37;
    headMatWorld[1][0] = v38;
    headCenterPointWorld[3] = &headMatWorld[1][1];
    headMatWorld[1][1] = v39;
    headMatWorld[1][2] = v40;
    headMatWorld[1][3] = v41;
    headMatWorld[2][0] = v42;
    headCenterPointWorld[2] = &headMatWorld[2][1];
    headMatWorld[2][1] = v43;
    headMatWorld[2][2] = v44;
    headMatWorld[2][3] = v45;
    headMatWorld[3][0] = v46;
    headCenterPointWorld[1] = &headMatWorld[2][1];
    fwd[1] = v43;
    fwd[2] = v44;
    traceHitFraction = v45;
    fwd[0] = 0.0f;
    start[0] = headMatWorld[3][1];
    start[1] = headMatWorld[3][2];
    start[2] = headMatWorld[3][3];
    end[0] = v43;
    end[1] = v44;
    end[2] = v45;
    traceHitNormal[0] = (float)(v78 * headMatWorld[3][1]) + v43;
    traceHitNormal[1] = (float)(v78 * headMatWorld[3][2]) + v44;
    traceHitNormal[2] = (float)(v78 * headMatWorld[3][3]) + v45;
    if ( IKImport_TraceBox(ikState, end, traceHitNormal, boxRadius, &v24, traceHitPos, -1, fwd, 0) )
    {
      v23 = 12.0f;
      v22 = ikState->entityLegsXform[0];
      v21 = ikState->entityXform[0];
      if ( (float *)ikState->entityLegsXform == &torsoFromLegsMat[13]
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
      {
        __debugbreak();
      }
      torsoFromLegsMat[13] = *v22;
      torsoFromLegsMat[14] = v22[4];
      torsoFromLegsMat[15] = v22[8];
      torsoFromLegsMat[16] = 0.0f;
      v13 = v22[1];
      v14 = v22[5];
      v15 = v22[9];
      v16 = 0;
      v17 = v22[2];
      v18 = v22[6];
      v19 = v22[10];
      v20 = 0;
      v9 = (float)((float)(*v21 * torsoFromLegsMat[13]) + (float)(v21[1] * v13)) + (float)(v21[2] * v17);
      v10 = (float)((float)(*v21 * torsoFromLegsMat[14]) + (float)(v21[1] * v14)) + (float)(v21[2] * v18);
      v11 = (float)((float)(*v21 * torsoFromLegsMat[15]) + (float)(v21[1] * v15)) + (float)(v21[2] * v19);
      torsoFromLegsMat[0] = 0.0f;
      torsoFromLegsMat[1] = (float)((float)(v21[4] * torsoFromLegsMat[13]) + (float)(v21[5] * v13))
                          + (float)(v21[6] * v17);
      torsoFromLegsMat[2] = (float)((float)(v21[4] * torsoFromLegsMat[14]) + (float)(v21[5] * v14))
                          + (float)(v21[6] * v18);
      torsoFromLegsMat[3] = (float)((float)(v21[4] * torsoFromLegsMat[15]) + (float)(v21[5] * v15))
                          + (float)(v21[6] * v19);
      torsoFromLegsMat[4] = 0.0f;
      torsoFromLegsMat[5] = (float)((float)(v21[8] * torsoFromLegsMat[13]) + (float)(v21[9] * v13))
                          + (float)(v21[10] * v17);
      torsoFromLegsMat[6] = (float)((float)(v21[8] * torsoFromLegsMat[14]) + (float)(v21[9] * v14))
                          + (float)(v21[10] * v18);
      torsoFromLegsMat[7] = (float)((float)(v21[8] * torsoFromLegsMat[15]) + (float)(v21[9] * v15))
                          + (float)(v21[10] * v19);
      torsoFromLegsMat[8] = 0.0f;
      LODWORD(v8[4]) = v22 + 12;
      LODWORD(v8[3]) = v21 + 12;
      v8[0] = v21[12] - v22[12];
      v8[1] = v21[13] - v22[13];
      v8[2] = v21[14] - v22[14];
      ikMatrixTransformVector34(v8, (const float (*)[4])&torsoFromLegsMat[13], &torsoFromLegsMat[9]);
      torsoFromLegsMat[12] = 1.0f;
      v7 = (*ikState->matArrayPostIK)[7];
      v6 = COERCE_FLOAT(COERCE_UNSIGNED_INT(1.0 - fwd[0]) ^ _mask__NegFloat_) * v23;
      *v7 = (float)(v6 * v9) + *v7;
      v7[1] = (float)(v6 * v10) + v7[1];
      v5 = COERCE_FLOAT(COERCE_UNSIGNED_INT(1.0 - fwd[0]) ^ _mask__NegFloat_) * v23;
      ikState->handMats[1][3][0] = (float)(v5 * v9) + ikState->handMats[1][3][0];
      ikState->handMats[1][3][1] = (float)(v5 * v10) + ikState->handMats[1][3][1];
      v4 = COERCE_FLOAT(COERCE_UNSIGNED_INT(1.0 - fwd[0]) ^ _mask__NegFloat_) * v23;
      ikState->handMats[0][3][0] = (float)(v4 * v9) + ikState->handMats[0][3][0];
      ikState->handMats[0][3][1] = (float)(v4 * v10) + ikState->handMats[0][3][1];
      v3 = COERCE_FLOAT(COERCE_UNSIGNED_INT(1.0 - fwd[0]) ^ _mask__NegFloat_) * v23;
      ikState->footMats[1][3][0] = (float)(v3 * v9) + ikState->footMats[1][3][0];
      ikState->footMats[1][3][1] = (float)(v3 * v10) + ikState->footMats[1][3][1];
      v2 = COERCE_FLOAT(COERCE_UNSIGNED_INT(1.0 - fwd[0]) ^ _mask__NegFloat_) * v23;
      ikState->footMats[0][3][0] = (float)(v2 * v9) + ikState->footMats[0][3][0];
      ikState->footMats[0][3][1] = (float)(v2 * v10) + ikState->footMats[0][3][1];
      ikState->bHasActiveLayers = 1;
      if ( ikState->cacheActive )
      {
        memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
        ikState->cacheActive = 0;
      }
      ikState->modifiedIKBones |= 2u;
    }
  }
}

// local variable allocation has failed, the output may be wrong!
void  IK_ProcessLayers(int a1@<ebp>, IKState *ikState)
{
  __int64 v2; // [esp-1BCh] [ebp-30Ch]
  __int64 v3; // [esp-1B4h] [ebp-304h]
  __int64 v4; // [esp-1ACh] [ebp-2FCh]
  __int64 v5; // [esp-1A4h] [ebp-2F4h]
  __int64 v6; // [esp-19Ch] [ebp-2ECh]
  __int64 v7; // [esp-194h] [ebp-2E4h]
  __int64 v8; // [esp-18Ch] [ebp-2DCh]
  __int64 v9; // [esp-184h] [ebp-2D4h]
  __int64 v10; // [esp-16Ch] [ebp-2BCh]
  __int64 v11; // [esp-164h] [ebp-2B4h]
  __int64 v12; // [esp-15Ch] [ebp-2ACh]
  __int64 v13; // [esp-154h] [ebp-2A4h]
  __int64 v14; // [esp-14Ch] [ebp-29Ch]
  __int64 v15; // [esp-144h] [ebp-294h]
  __int64 v16; // [esp-13Ch] [ebp-28Ch]
  __int64 v17; // [esp-134h] [ebp-284h]
  __int64 v18; // [esp-108h] [ebp-258h]
  __int64 v19; // [esp-100h] [ebp-250h]
  __int64 v20; // [esp-F8h] [ebp-248h]
  __int64 v21; // [esp-F0h] [ebp-240h]
  __int64 v22; // [esp-E8h] [ebp-238h]
  __int64 v23; // [esp-E0h] [ebp-230h]
  __int64 v24; // [esp-D8h] [ebp-228h]
  float v25; // [esp-D0h] [ebp-220h]
  float v26[5]; // [esp-20h] [ebp-170h] BYREF
  float v27; // [esp-Ch] [ebp-15Ch]
  float v28; // [esp-8h] [ebp-158h]
  float v29; // [esp-4h] [ebp-154h]
  float rightHandFromSpineMat[17]; // [esp+0h] [ebp-150h] OVERLAPPED BYREF
  int v31; // [esp+44h] [ebp-10Ch]
  int v32; // [esp+48h] [ebp-108h]
  int v33; // [esp+4Ch] [ebp-104h]
  int v34; // [esp+50h] [ebp-100h]
  int v35; // [esp+54h] [ebp-FCh]
  int v36; // [esp+58h] [ebp-F8h]
  int v37; // [esp+5Ch] [ebp-F4h]
  int v38; // [esp+60h] [ebp-F0h]
  float *v39; // [esp+80h] [ebp-D0h]
  _BYTE v40[76]; // [esp+84h] [ebp-CCh] OVERLAPPED BYREF
  float v41[5]; // [esp+D0h] [ebp-80h] BYREF
  float v42[4]; // [esp+E4h] [ebp-6Ch] BYREF
  float v43; // [esp+F4h] [ebp-5Ch]
  float v44; // [esp+F8h] [ebp-58h]
  float v45; // [esp+FCh] [ebp-54h]
  int v46; // [esp+100h] [ebp-50h]
  float v47; // [esp+104h] [ebp-4Ch]
  float v48; // [esp+108h] [ebp-48h]
  float v49; // [esp+10Ch] [ebp-44h]
  int v50; // [esp+110h] [ebp-40h]
  float *v51; // [esp+130h] [ebp-20h]
  float *v52; // [esp+134h] [ebp-1Ch]
  float *v53; // [esp+138h] [ebp-18h]
  bool v54; // [esp+13Eh] [ebp-12h]
  bool v55; // [esp+13Fh] [ebp-11h]
  unsigned int v56[3]; // [esp+144h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+150h] [ebp+0h]

  v56[0] = a1;
  v56[1] = retaddr;
  if ( ikState->bJointVarsValid )
  {
    if ( ikSystem.inViewer )
    {
      ikState->bHasActiveLayers = 1;
      ikState->bHasArmSolverLayer = 1;
    }
    BLOPS_NULLSUB();
    v55 = ikState->ikBoneToObjBone[10] != 161;
    if ( !v55
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
            1773,
            0,
            "%s",
            "IK_IsBoneAnimated( ikState, IKBONE_LHAND )") )
    {
      __debugbreak();
    }
    ikCalcBoneModelMatrix(ikState, 10, (*ikState->matArrayPostIK)[0], ikState->handMats[0]);
    v54 = ikState->ikBoneToObjBone[15] != 161;
    if ( !v54
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
            1776,
            0,
            "%s",
            "IK_IsBoneAnimated( ikState, IKBONE_RHAND )") )
    {
      __debugbreak();
    }
    ikCalcBoneModelMatrix(ikState, 15, (*ikState->matArrayPostIK)[0], ikState->handMats[1]);
    v53 = ikState->leftHandFromRightHandMat[0];
    v52 = ikState->handMats[1][0];
    v51 = ikState->handMats[0][0];
    if ( (float *)ikState->handMats[1] == v42
      && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
    {
      __debugbreak();
    }
    v42[0] = *v52;
    v42[1] = v52[4];
    v42[2] = v52[8];
    v42[3] = 0.0f;
    v43 = v52[1];
    v44 = v52[5];
    v45 = v52[9];
    v46 = 0;
    v47 = v52[2];
    v48 = v52[6];
    v49 = v52[10];
    v50 = 0;
    *v53 = (float)((float)(*v51 * v42[0]) + (float)(v51[1] * v43)) + (float)(v51[2] * v47);
    v53[1] = (float)((float)(*v51 * v42[1]) + (float)(v51[1] * v44)) + (float)(v51[2] * v48);
    v53[2] = (float)((float)(*v51 * v42[2]) + (float)(v51[1] * v45)) + (float)(v51[2] * v49);
    v53[3] = 0.0f;
    v53[4] = (float)((float)(v51[4] * v42[0]) + (float)(v51[5] * v43)) + (float)(v51[6] * v47);
    v53[5] = (float)((float)(v51[4] * v42[1]) + (float)(v51[5] * v44)) + (float)(v51[6] * v48);
    v53[6] = (float)((float)(v51[4] * v42[2]) + (float)(v51[5] * v45)) + (float)(v51[6] * v49);
    v53[7] = 0.0f;
    v53[8] = (float)((float)(v51[8] * v42[0]) + (float)(v51[9] * v43)) + (float)(v51[10] * v47);
    v53[9] = (float)((float)(v51[8] * v42[1]) + (float)(v51[9] * v44)) + (float)(v51[10] * v48);
    v53[10] = (float)((float)(v51[8] * v42[2]) + (float)(v51[9] * v45)) + (float)(v51[10] * v49);
    v53[11] = 0.0f;
    LODWORD(v41[4]) = v52 + 12;
    LODWORD(v41[3]) = v51 + 12;
    v41[0] = v51[12] - v52[12];
    v41[1] = v51[13] - v52[13];
    v41[2] = v51[14] - v52[14];
    ikMatrixTransformVector34(v41, (const float (*)[4])v42, v53 + 12);
    v53[15] = 1.0f;
    v40[75] = ikState->ikBoneToObjBone[18] != 161;
    if ( !v40[75]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
            1782,
            0,
            "%s",
            "IK_IsBoneAnimated( ikState, IKBONE_LFOOT )") )
    {
      __debugbreak();
    }
    ikCalcBoneModelMatrix(ikState, 18, (*ikState->matArrayPostIK)[0], ikState->footMats[0]);
    v40[74] = ikState->ikBoneToObjBone[21] != 161;
    if ( !v40[74]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
            1785,
            0,
            "%s",
            "IK_IsBoneAnimated( ikState, IKBONE_RFOOT )") )
    {
      __debugbreak();
    }
    ikCalcBoneModelMatrix(ikState, 21, (*ikState->matArrayPostIK)[0], ikState->footMats[1]);
    IK_Layer_LeftHandOnGun(COERCE_FLOAT(v56), ikState);
    if ( IKImport_IsClient(ikState) )
    {
      IK_Layer_PlayerPitch(COERCE_FLOAT(v56), ikState, 1);
      ikCalcBoneModelMatrix(ikState, 4, (*ikState->matArrayPostIK)[0], (float (*)[4])v40);
      v39 = ikState->handMats[1][0];
      if ( v40 == (_BYTE *)&rightHandFromSpineMat[13]
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
      {
        __debugbreak();
      }
      rightHandFromSpineMat[13] = *(float *)v40;
      rightHandFromSpineMat[14] = *(float *)&v40[16];
      rightHandFromSpineMat[15] = *(float *)&v40[32];
      rightHandFromSpineMat[16] = 0.0f;
      v31 = *(unsigned int *)&v40[4];
      v32 = *(unsigned int *)&v40[20];
      v33 = *(unsigned int *)&v40[36];
      v34 = 0;
      v35 = *(unsigned int *)&v40[8];
      v36 = *(unsigned int *)&v40[24];
      v37 = *(unsigned int *)&v40[40];
      v38 = 0;
      v27 = (float)((float)(*v39 * *(float *)v40) + (float)(v39[1] * *(float *)&v40[4]))
          + (float)(v39[2] * *(float *)&v40[8]);
      v28 = (float)((float)(*v39 * *(float *)&v40[16]) + (float)(v39[1] * *(float *)&v40[20]))
          + (float)(v39[2] * *(float *)&v40[24]);
      v29 = (float)((float)(*v39 * *(float *)&v40[32]) + (float)(v39[1] * *(float *)&v40[36]))
          + (float)(v39[2] * *(float *)&v40[40]);
      rightHandFromSpineMat[0] = 0.0f;
      rightHandFromSpineMat[1] = (float)((float)(v39[4] * *(float *)v40) + (float)(v39[5] * *(float *)&v40[4]))
                               + (float)(v39[6] * *(float *)&v40[8]);
      rightHandFromSpineMat[2] = (float)((float)(v39[4] * *(float *)&v40[16]) + (float)(v39[5] * *(float *)&v40[20]))
                               + (float)(v39[6] * *(float *)&v40[24]);
      rightHandFromSpineMat[3] = (float)((float)(v39[4] * *(float *)&v40[32]) + (float)(v39[5] * *(float *)&v40[36]))
                               + (float)(v39[6] * *(float *)&v40[40]);
      rightHandFromSpineMat[4] = 0.0f;
      rightHandFromSpineMat[5] = (float)((float)(v39[8] * *(float *)v40) + (float)(v39[9] * *(float *)&v40[4]))
                               + (float)(v39[10] * *(float *)&v40[8]);
      rightHandFromSpineMat[6] = (float)((float)(v39[8] * *(float *)&v40[16]) + (float)(v39[9] * *(float *)&v40[20]))
                               + (float)(v39[10] * *(float *)&v40[24]);
      rightHandFromSpineMat[7] = (float)((float)(v39[8] * *(float *)&v40[32]) + (float)(v39[9] * *(float *)&v40[36]))
                               + (float)(v39[10] * *(float *)&v40[40]);
      rightHandFromSpineMat[8] = 0.0f;
      LODWORD(v26[4]) = &v40[48];
      LODWORD(v26[3]) = v39 + 12;
      v26[0] = v39[12] - *(float *)&v40[48];
      v26[1] = v39[13] - *(float *)&v40[52];
      v26[2] = v39[14] - *(float *)&v40[56];
      ikMatrixTransformVector34(v26, (const float (*)[4])&rightHandFromSpineMat[13], &rightHandFromSpineMat[9]);
      rightHandFromSpineMat[12] = 1.0f;
      IK_Layer_PlayerControllers(ikState);
      ikCalcBoneModelMatrix(ikState, 4, (*ikState->matArrayPostIK)[0], (float (*)[4])v40);
      *(float *)&v18 = (float)((float)((float)(v27 * *(float *)&v40[4]) + (float)(v28 * *(float *)&v40[20]))
                             + (float)(v29 * *(float *)&v40[36]))
                     + (float)(rightHandFromSpineMat[0] * *(float *)&v40[52]);
      *((float *)&v18 + 1) = (float)((float)((float)(v27 * *(float *)&v40[8]) + (float)(v28 * *(float *)&v40[24]))
                                   + (float)(v29 * *(float *)&v40[40]))
                           + (float)(rightHandFromSpineMat[0] * *(float *)&v40[56]);
      *(float *)&v19 = (float)((float)((float)(v27 * *(float *)&v40[12]) + (float)(v28 * *(float *)&v40[28]))
                             + (float)(v29 * *(float *)&v40[44]))
                     + (float)(rightHandFromSpineMat[0] * *(float *)&v40[60]);
      *((float *)&v19 + 1) = (float)((float)((float)(rightHandFromSpineMat[1] * *(float *)v40)
                                           + (float)(rightHandFromSpineMat[2] * *(float *)&v40[16]))
                                   + (float)(rightHandFromSpineMat[3] * *(float *)&v40[32]))
                           + (float)(rightHandFromSpineMat[4] * *(float *)&v40[48]);
      *(float *)&v20 = (float)((float)((float)(rightHandFromSpineMat[1] * *(float *)&v40[4])
                                     + (float)(rightHandFromSpineMat[2] * *(float *)&v40[20]))
                             + (float)(rightHandFromSpineMat[3] * *(float *)&v40[36]))
                     + (float)(rightHandFromSpineMat[4] * *(float *)&v40[52]);
      *((float *)&v20 + 1) = (float)((float)((float)(rightHandFromSpineMat[1] * *(float *)&v40[8])
                                           + (float)(rightHandFromSpineMat[2] * *(float *)&v40[24]))
                                   + (float)(rightHandFromSpineMat[3] * *(float *)&v40[40]))
                           + (float)(rightHandFromSpineMat[4] * *(float *)&v40[56]);
      *(float *)&v21 = (float)((float)((float)(rightHandFromSpineMat[1] * *(float *)&v40[12])
                                     + (float)(rightHandFromSpineMat[2] * *(float *)&v40[28]))
                             + (float)(rightHandFromSpineMat[3] * *(float *)&v40[44]))
                     + (float)(rightHandFromSpineMat[4] * *(float *)&v40[60]);
      *((float *)&v21 + 1) = (float)((float)((float)(rightHandFromSpineMat[5] * *(float *)v40)
                                           + (float)(rightHandFromSpineMat[6] * *(float *)&v40[16]))
                                   + (float)(rightHandFromSpineMat[7] * *(float *)&v40[32]))
                           + (float)(rightHandFromSpineMat[8] * *(float *)&v40[48]);
      *(float *)&v22 = (float)((float)((float)(rightHandFromSpineMat[5] * *(float *)&v40[4])
                                     + (float)(rightHandFromSpineMat[6] * *(float *)&v40[20]))
                             + (float)(rightHandFromSpineMat[7] * *(float *)&v40[36]))
                     + (float)(rightHandFromSpineMat[8] * *(float *)&v40[52]);
      *((float *)&v22 + 1) = (float)((float)((float)(rightHandFromSpineMat[5] * *(float *)&v40[8])
                                           + (float)(rightHandFromSpineMat[6] * *(float *)&v40[24]))
                                   + (float)(rightHandFromSpineMat[7] * *(float *)&v40[40]))
                           + (float)(rightHandFromSpineMat[8] * *(float *)&v40[56]);
      *(float *)&v23 = (float)((float)((float)(rightHandFromSpineMat[5] * *(float *)&v40[12])
                                     + (float)(rightHandFromSpineMat[6] * *(float *)&v40[28]))
                             + (float)(rightHandFromSpineMat[7] * *(float *)&v40[44]))
                     + (float)(rightHandFromSpineMat[8] * *(float *)&v40[60]);
      *((float *)&v23 + 1) = (float)((float)((float)(rightHandFromSpineMat[9] * *(float *)v40)
                                           + (float)(rightHandFromSpineMat[10] * *(float *)&v40[16]))
                                   + (float)(rightHandFromSpineMat[11] * *(float *)&v40[32]))
                           + (float)(rightHandFromSpineMat[12] * *(float *)&v40[48]);
      *(float *)&v24 = (float)((float)((float)(rightHandFromSpineMat[9] * *(float *)&v40[4])
                                     + (float)(rightHandFromSpineMat[10] * *(float *)&v40[20]))
                             + (float)(rightHandFromSpineMat[11] * *(float *)&v40[36]))
                     + (float)(rightHandFromSpineMat[12] * *(float *)&v40[52]);
      *((float *)&v24 + 1) = (float)((float)((float)(rightHandFromSpineMat[9] * *(float *)&v40[8])
                                           + (float)(rightHandFromSpineMat[10] * *(float *)&v40[24]))
                                   + (float)(rightHandFromSpineMat[11] * *(float *)&v40[40]))
                           + (float)(rightHandFromSpineMat[12] * *(float *)&v40[56]);
      v25 = (float)((float)((float)(rightHandFromSpineMat[9] * *(float *)&v40[12])
                          + (float)(rightHandFromSpineMat[10] * *(float *)&v40[28]))
                  + (float)(rightHandFromSpineMat[11] * *(float *)&v40[44]))
          + (float)(rightHandFromSpineMat[12] * *(float *)&v40[60]);
      ikState->handMats[1][0][0] = (float)((float)((float)(v27 * *(float *)v40) + (float)(v28 * *(float *)&v40[16]))
                                         + (float)(v29 * *(float *)&v40[32]))
                                 + (float)(rightHandFromSpineMat[0] * *(float *)&v40[48]);
      *(_QWORD *)&ikState->handMats[1][0][1] = v18;
      *(_QWORD *)&ikState->handMats[1][0][3] = v19;
      *(_QWORD *)&ikState->handMats[1][1][1] = v20;
      *(_QWORD *)&ikState->handMats[1][1][3] = v21;
      *(_QWORD *)&ikState->handMats[1][2][1] = v22;
      *(_QWORD *)&ikState->handMats[1][2][3] = v23;
      *(_QWORD *)&ikState->handMats[1][3][1] = v24;
      ikState->handMats[1][3][3] = v25;
      v10 = *(_QWORD *)&ikState->leftHandFromRightHandMat[0][0];
      v11 = *(_QWORD *)&ikState->leftHandFromRightHandMat[0][2];
      v12 = *(_QWORD *)&ikState->leftHandFromRightHandMat[1][0];
      v13 = *(_QWORD *)&ikState->leftHandFromRightHandMat[1][2];
      v14 = *(_QWORD *)&ikState->leftHandFromRightHandMat[2][0];
      v15 = *(_QWORD *)&ikState->leftHandFromRightHandMat[2][2];
      v16 = *(_QWORD *)&ikState->leftHandFromRightHandMat[3][0];
      v17 = *(_QWORD *)&ikState->leftHandFromRightHandMat[3][2];
      v2 = *(_QWORD *)&ikState->handMats[1][0][0];
      v3 = *(_QWORD *)&ikState->handMats[1][0][2];
      v4 = *(_QWORD *)&ikState->handMats[1][1][0];
      v5 = *(_QWORD *)&ikState->handMats[1][1][2];
      v6 = *(_QWORD *)&ikState->handMats[1][2][0];
      v7 = *(_QWORD *)&ikState->handMats[1][2][2];
      v8 = *(_QWORD *)&ikState->handMats[1][3][0];
      v9 = *(_QWORD *)&ikState->handMats[1][3][2];
      ikState->handMats[0][0][0] = (float)((float)((float)(*(float *)&v10 * *(float *)&v2)
                                                 + (float)(*((float *)&v10 + 1) * *(float *)&v4))
                                         + (float)(*(float *)&v11 * *(float *)&v6))
                                 + (float)(*((float *)&v11 + 1) * *(float *)&v8);
      ikState->handMats[0][0][1] = (float)((float)((float)(*(float *)&v10 * *((float *)&v2 + 1))
                                                 + (float)(*((float *)&v10 + 1) * *((float *)&v4 + 1)))
                                         + (float)(*(float *)&v11 * *((float *)&v6 + 1)))
                                 + (float)(*((float *)&v11 + 1) * *((float *)&v8 + 1));
      ikState->handMats[0][0][2] = (float)((float)((float)(*(float *)&v10 * *(float *)&v3)
                                                 + (float)(*((float *)&v10 + 1) * *(float *)&v5))
                                         + (float)(*(float *)&v11 * *(float *)&v7))
                                 + (float)(*((float *)&v11 + 1) * *(float *)&v9);
      ikState->handMats[0][0][3] = (float)((float)((float)(*(float *)&v10 * *((float *)&v3 + 1))
                                                 + (float)(*((float *)&v10 + 1) * *((float *)&v5 + 1)))
                                         + (float)(*(float *)&v11 * *((float *)&v7 + 1)))
                                 + (float)(*((float *)&v11 + 1) * *((float *)&v9 + 1));
      ikState->handMats[0][1][0] = (float)((float)((float)(*(float *)&v12 * *(float *)&v2)
                                                 + (float)(*((float *)&v12 + 1) * *(float *)&v4))
                                         + (float)(*(float *)&v13 * *(float *)&v6))
                                 + (float)(*((float *)&v13 + 1) * *(float *)&v8);
      ikState->handMats[0][1][1] = (float)((float)((float)(*(float *)&v12 * *((float *)&v2 + 1))
                                                 + (float)(*((float *)&v12 + 1) * *((float *)&v4 + 1)))
                                         + (float)(*(float *)&v13 * *((float *)&v6 + 1)))
                                 + (float)(*((float *)&v13 + 1) * *((float *)&v8 + 1));
      ikState->handMats[0][1][2] = (float)((float)((float)(*(float *)&v12 * *(float *)&v3)
                                                 + (float)(*((float *)&v12 + 1) * *(float *)&v5))
                                         + (float)(*(float *)&v13 * *(float *)&v7))
                                 + (float)(*((float *)&v13 + 1) * *(float *)&v9);
      ikState->handMats[0][1][3] = (float)((float)((float)(*(float *)&v12 * *((float *)&v3 + 1))
                                                 + (float)(*((float *)&v12 + 1) * *((float *)&v5 + 1)))
                                         + (float)(*(float *)&v13 * *((float *)&v7 + 1)))
                                 + (float)(*((float *)&v13 + 1) * *((float *)&v9 + 1));
      ikState->handMats[0][2][0] = (float)((float)((float)(*(float *)&v14 * *(float *)&v2)
                                                 + (float)(*((float *)&v14 + 1) * *(float *)&v4))
                                         + (float)(*(float *)&v15 * *(float *)&v6))
                                 + (float)(*((float *)&v15 + 1) * *(float *)&v8);
      ikState->handMats[0][2][1] = (float)((float)((float)(*(float *)&v14 * *((float *)&v2 + 1))
                                                 + (float)(*((float *)&v14 + 1) * *((float *)&v4 + 1)))
                                         + (float)(*(float *)&v15 * *((float *)&v6 + 1)))
                                 + (float)(*((float *)&v15 + 1) * *((float *)&v8 + 1));
      ikState->handMats[0][2][2] = (float)((float)((float)(*(float *)&v14 * *(float *)&v3)
                                                 + (float)(*((float *)&v14 + 1) * *(float *)&v5))
                                         + (float)(*(float *)&v15 * *(float *)&v7))
                                 + (float)(*((float *)&v15 + 1) * *(float *)&v9);
      ikState->handMats[0][2][3] = (float)((float)((float)(*(float *)&v14 * *((float *)&v3 + 1))
                                                 + (float)(*((float *)&v14 + 1) * *((float *)&v5 + 1)))
                                         + (float)(*(float *)&v15 * *((float *)&v7 + 1)))
                                 + (float)(*((float *)&v15 + 1) * *((float *)&v9 + 1));
      ikState->handMats[0][3][0] = (float)((float)((float)(*(float *)&v16 * *(float *)&v2)
                                                 + (float)(*((float *)&v16 + 1) * *(float *)&v4))
                                         + (float)(*(float *)&v17 * *(float *)&v6))
                                 + (float)(*((float *)&v17 + 1) * *(float *)&v8);
      ikState->handMats[0][3][1] = (float)((float)((float)(*(float *)&v16 * *((float *)&v2 + 1))
                                                 + (float)(*((float *)&v16 + 1) * *((float *)&v4 + 1)))
                                         + (float)(*(float *)&v17 * *((float *)&v6 + 1)))
                                 + (float)(*((float *)&v17 + 1) * *((float *)&v8 + 1));
      ikState->handMats[0][3][2] = (float)((float)((float)(*(float *)&v16 * *(float *)&v3)
                                                 + (float)(*((float *)&v16 + 1) * *(float *)&v5))
                                         + (float)(*(float *)&v17 * *(float *)&v7))
                                 + (float)(*((float *)&v17 + 1) * *(float *)&v9);
      ikState->handMats[0][3][3] = (float)((float)((float)(*(float *)&v16 * *((float *)&v3 + 1))
                                                 + (float)(*((float *)&v16 + 1) * *((float *)&v5 + 1)))
                                         + (float)(*(float *)&v17 * *((float *)&v7 + 1)))
                                 + (float)(*((float *)&v17 + 1) * *((float *)&v9 + 1));
    }
    else
    {
      IK_Layer_PlayerControllers(ikState);
    }
    if ( IKImport_IsClient(ikState) )
    {
      IK_Layer_PlayerPitch(COERCE_FLOAT(v56), ikState, 0);
      IK_Layer_PreventHeadClip(COERCE_FLOAT(v56), ikState);
    }
    IK_Layer_TerrainMapping(COERCE_FLOAT(v56), ikState);
    if ( ikState->bHasActiveLayers )
    {
      if ( ikState->bHasLegSolverLayer )
      {
        IK_Layer_ApplyFootIK(
          (int)v56,
          ikState,
          &ikState->localIkSystem->joints[2],
          &ikState->jointVars[2],
          ikState->footMats[0]);
        IK_Layer_ApplyFootIK(
          (int)v56,
          ikState,
          &ikState->localIkSystem->joints[3],
          &ikState->jointVars[3],
          ikState->footMats[1]);
      }
      if ( ikState->bHasArmSolverLayer )
      {
        IK_Layer_ApplyHandIK(
          (int)v56,
          ikState,
          ikState->localIkSystem->joints,
          ikState->jointVars,
          ikState->handMats[0]);
        IK_Layer_ApplyHandIK(
          (int)v56,
          ikState,
          &ikState->localIkSystem->joints[1],
          &ikState->jointVars[1],
          ikState->handMats[1]);
      }
    }
  }
}

