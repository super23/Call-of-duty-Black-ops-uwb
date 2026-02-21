#include "ik_layers.h"
#include "ik_import.h"
#include "ik_math.h"
#include <universal/com_math_anglevectors.h>

// IK's are like onions. Onions have layers!

float frametimeCapTime = 0.1f;
float frametimeResetCap = 0.5f;
float frametimeCap = 0.2f;
float pitchScalarDefault = 0.6f;
float pitchScalarProne = 1.0f;
float velIdleMax = 100.0f;



float leftHandRotationStatic[3];
float leftHandOffsetStatic[3];
float fMaxFootReachSubtract;
float fMaxHandReachSubtract;



// local variable allocation has failed, the output may be wrong!
void    IK_Layer_TerrainMapping(IKState *ikState)
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
    __int64 curFoot; // [esp+38h] [ebp-77Ch]
    __int64 curFoot_8; // [esp+40h] [ebp-774h]
    __int64 curFoot_16; // [esp+48h] [ebp-76Ch]
    __int64 curFoot_24; // [esp+50h] [ebp-764h]
    __int64 curFoot_32; // [esp+58h] [ebp-75Ch]
    __int64 curFoot_40; // [esp+60h] [ebp-754h]
    float invXform[4][4]; // [esp+D8h] [ebp-6DCh] BYREF
    float *v27; // [esp+124h] [ebp-690h]
    float terrainAxis[4][4]; // [esp+128h] [ebp-68Ch] BYREF
    float *v29; // [esp+16Ch] [ebp-648h]
    float v30[3]; // [esp+170h] [ebp-644h] BYREF
    float v31[3]; // [esp+17Ch] [ebp-638h] BYREF
    float *v32; // [esp+188h] [ebp-62Ch]
    float v33; // [esp+18Ch] [ebp-628h]
    float *v34; // [esp+190h] [ebp-624h]
    float XYdist; // [esp+194h] [ebp-620h]
    float v36; // [esp+198h] [ebp-61Ch]
    float v37; // [esp+19Ch] [ebp-618h]
    float v38; // [esp+1A0h] [ebp-614h]
    float v39; // [esp+1A4h] [ebp-610h]
    int v40; // [esp+1A8h] [ebp-60Ch]
    float *v41; // [esp+1ACh] [ebp-608h]
    float *v42; // [esp+1B0h] [ebp-604h]
    float angularWeight; // [esp+1B4h] [ebp-600h]
    float v44; // [esp+1B8h] [ebp-5FCh]
    float footOffsetAngularScale; // [esp+1BCh] [ebp-5F8h]
    float v46; // [esp+1C0h] [ebp-5F4h]
    float v47; // [esp+1C4h] [ebp-5F0h]
    float v48; // [esp+1C8h] [ebp-5ECh]
    float angularMappingHeightOffsetMax; // [esp+1CCh] [ebp-5E8h]
    float v_up[3]; // [esp+1D0h] [ebp-5E4h] BYREF
    float terrainNormalMinZ; // [esp+1DCh] [ebp-5D8h]
    float terrainNormalRangeCap; // [esp+1E0h] [ebp-5D4h]
    float terrainNormalLerpTime; // [esp+1E4h] [ebp-5D0h]
    float v54; // [esp+1E8h] [ebp-5CCh]
    float bSq; // [esp+1ECh] [ebp-5C8h]
    float cMaxSq; // [esp+1F0h] [ebp-5C4h]
    float v57; // [esp+1F4h] [ebp-5C0h]
    int v58; // [esp+1F8h] [ebp-5BCh]
    float v59; // [esp+1FCh] [ebp-5B8h]
    float cSq; // [esp+200h] [ebp-5B4h]
    float aSq; // [esp+204h] [ebp-5B0h]
    float v62; // [esp+208h] [ebp-5ACh]
    float v63; // [esp+20Ch] [ebp-5A8h]
    float *v64; // [esp+210h] [ebp-5A4h]
    const float *v65; // [esp+214h] [ebp-5A0h]
    float parentMat[4][4]; // [esp+218h] [ebp-59Ch] BYREF
    float terrainLegExtendAdjust; // [esp+260h] [ebp-554h]
    float v68; // [esp+264h] [ebp-550h]
    float v69; // [esp+268h] [ebp-54Ch]
    float changeZ; // [esp+26Ch] [ebp-548h]
    float realPelvisZ; // [esp+270h] [ebp-544h]
    float PELVIS_ABS_SCALAR; // [esp+274h] [ebp-540h]
    float zOfs; // [esp+278h] [ebp-53Ch]
    float pelvisMovingZAdjust; // [esp+27Ch] [ebp-538h]
    float v75; // [esp+280h] [ebp-534h]
    float v76; // [esp+284h] [ebp-530h]
    float v77; // [esp+288h] [ebp-52Ch]
    float v78; // [esp+28Ch] [ebp-528h]
    float pelvisForceZAdjust; // [esp+290h] [ebp-524h]
    float pelvisMoveScale; // [esp+294h] [ebp-520h]
    float lowerAdjust; // [esp+298h] [ebp-51Ch]
    int lowerFoot; // [esp+29Ch] [ebp-518h]
    float v83; // [esp+2A0h] [ebp-514h]
    float v84; // [esp+2A4h] [ebp-510h]
    float v85; // [esp+2A8h] [ebp-50Ch]
    float v86; // [esp+2ACh] [ebp-508h]
    float minScale; // [esp+2B0h] [ebp-504h]
    float footOffsetAdjusted; // [esp+2B4h] [ebp-500h]
    float footOffsetFromStandard; // [esp+2B8h] [ebp-4FCh]
    float v90; // [esp+2BCh] [ebp-4F8h]
    float v91; // [esp+2C0h] [ebp-4F4h]
    float v92; // [esp+2C4h] [ebp-4F0h]
    float v93; // [esp+2C8h] [ebp-4ECh]
    float v94; // [esp+2CCh] [ebp-4E8h]
    float v95; // [esp+2D0h] [ebp-4E4h]
    float fastLerpScalar[2]; // [esp+2D4h] [ebp-4E0h]
    float terrainDiffNoLerpingThreshold; // [esp+2DCh] [ebp-4D8h]
    float terrainDiff; // [esp+2E0h] [ebp-4D4h]
    float realZ; // [esp+2E4h] [ebp-4D0h]
    float v100[2]; // [esp+2E8h] [ebp-4CCh] BYREF
    float *v101; // [esp+2F0h] [ebp-4C4h]
    float *v102; // [esp+2F4h] [ebp-4C0h]
    float *v103; // [esp+2F8h] [ebp-4BCh]
    float *v104; // [esp+2FCh] [ebp-4B8h]
    float *v105; // [esp+300h] [ebp-4B4h]
    float *v106; // [esp+304h] [ebp-4B0h]
    float *v107; // [esp+308h] [ebp-4ACh]
    float end[3]; // [esp+30Ch] [ebp-4A8h] BYREF
    float start[3]; // [esp+318h] [ebp-49Ch] BYREF
    float v110; // [esp+324h] [ebp-490h]
    float v111; // [esp+328h] [ebp-48Ch]
    float v112; // [esp+32Ch] [ebp-488h]
    float v113; // [esp+330h] [ebp-484h]
    float footDirectionToNormalDot; // [esp+334h] [ebp-480h]
    float *v115; // [esp+338h] [ebp-47Ch]
    float *v116; // [esp+33Ch] [ebp-478h]
    float lowerFootTerrainDotClampMax; // [esp+340h] [ebp-474h]
    float lowerFootTerrainDotClampMin; // [esp+344h] [ebp-470h]
    float lowerFootTerrainDotScale; // [esp+348h] [ebp-46Ch]
    float lowerFootTerrainSteepnessScale; // [esp+34Ch] [ebp-468h]
    float traceFootAdjust; // [esp+350h] [ebp-464h]
    float *v122; // [esp+354h] [ebp-460h]
    float *v123; // [esp+358h] [ebp-45Ch]
    float v124; // [esp+35Ch] [ebp-458h]
    float tracePos[3]; // [esp+360h] [ebp-454h]
    float thisTracePos[3]; // [esp+36Ch] [ebp-448h] BYREF
    float thisNormal[3]; // [esp+378h] [ebp-43Ch] BYREF
    float *v128; // [esp+384h] [ebp-430h]
    float v129; // [esp+388h] [ebp-42Ch]
    float *v130; // [esp+38Ch] [ebp-428h]
    int n; // [esp+390h] [ebp-424h]
    float maxTraceZ; // [esp+394h] [ebp-420h]
    bool traceHitSomething; // [esp+39Bh] [ebp-419h]
    float v134[2]; // [esp+39Ch] [ebp-418h] BYREF
    float *v135; // [esp+3A4h] [ebp-410h]
    float *v136; // [esp+3A8h] [ebp-40Ch]
    float footTraceCacheDistThreshold; // [esp+3ACh] [ebp-408h]
    float *v138; // [esp+3B0h] [ebp-404h]
    float *v139; // [esp+3B4h] [ebp-400h]
    float *v140; // [esp+3B8h] [ebp-3FCh]
    float *v141; // [esp+3BCh] [ebp-3F8h]
    float *v142; // [esp+3C0h] [ebp-3F4h]
    float footTraceNormals[2][3]; // [esp+3C4h] [ebp-3F0h] BYREF
    float v144; // [esp+3DCh] [ebp-3D8h]
    float startPos[3]; // [esp+3E0h] [ebp-3D4h] BYREF
    float endPos[3]; // [esp+3ECh] [ebp-3C8h] BYREF
    float footTraceBoxRadius; // [esp+3F8h] [ebp-3BCh]
    bool modifiedFeet[4]; // [esp+3FCh] [ebp-3B8h]
    float groundOffsets[2]; // [esp+400h] [ebp-3B4h]
    float terrainZ; // [esp+408h] [ebp-3ACh]
    float maxPelvisReach; // [esp+40Ch] [ebp-3A8h]
    float highestGroundZ; // [esp+410h] [ebp-3A4h]
    float lowestGroundZ; // [esp+414h] [ebp-3A0h]
    float *v154; // [esp+418h] [ebp-39Ch]
    float *v155; // [esp+41Ch] [ebp-398h]
    float *v156; // [esp+420h] [ebp-394h]
    float *v157; // [esp+424h] [ebp-390h]
    float v158; // [esp+428h] [ebp-38Ch]
    float v159; // [esp+42Ch] [ebp-388h]
    float v160; // [esp+430h] [ebp-384h]
    float v161; // [esp+434h] [ebp-380h]
    float v162; // [esp+438h] [ebp-37Ch]
    float v163; // [esp+43Ch] [ebp-378h]
    float v164; // [esp+440h] [ebp-374h]
    float v165; // [esp+444h] [ebp-370h]
    float v166; // [esp+448h] [ebp-36Ch]
    float v167; // [esp+44Ch] [ebp-368h]
    float v168; // [esp+450h] [ebp-364h]
    float v169; // [esp+454h] [ebp-360h]
    float v170; // [esp+458h] [ebp-35Ch]
    float v171; // [esp+45Ch] [ebp-358h]
    float v172; // [esp+460h] [ebp-354h]
    float v173; // [esp+464h] [ebp-350h]
    float *v174; // [esp+46Ch] [ebp-348h]
    float *v175; // [esp+470h] [ebp-344h]
    float *v176; // [esp+474h] [ebp-340h]
    float v177; // [esp+478h] [ebp-33Ch]
    __int64 v178; // [esp+47Ch] [ebp-338h]
    float v179; // [esp+484h] [ebp-330h]
    float v180; // [esp+488h] [ebp-32Ch]
    __int64 v181; // [esp+48Ch] [ebp-328h]
    float v182; // [esp+494h] [ebp-320h]
    float v183; // [esp+498h] [ebp-31Ch]
    __int64 v184; // [esp+49Ch] [ebp-318h]
    float v185; // [esp+4A4h] [ebp-310h]
    __int64 v186; // [esp+4A8h] [ebp-30Ch]
    __int64 v187; // [esp+4B0h] [ebp-304h]
    float *v188; // [esp+4B8h] [ebp-2FCh]
    float *v189; // [esp+4BCh] [ebp-2F8h]
    float *v190; // [esp+4C0h] [ebp-2F4h]
    float *v191; // [esp+4C4h] [ebp-2F0h]
    __int64 v192; // [esp+4C8h] [ebp-2ECh]
    __int64 v193; // [esp+4D0h] [ebp-2E4h]
    __int64 v194; // [esp+4D8h] [ebp-2DCh]
    __int64 v195; // [esp+4E0h] [ebp-2D4h]
    __int64 v196; // [esp+4E8h] [ebp-2CCh]
    __int64 v197; // [esp+4F0h] [ebp-2C4h]
    __int64 v198; // [esp+4F8h] [ebp-2BCh]
    __int64 v199; // [esp+500h] [ebp-2B4h]
    float *v200; // [esp+50Ch] [ebp-2A8h]
    float *v201; // [esp+510h] [ebp-2A4h]
    float *v202; // [esp+514h] [ebp-2A0h]
    float worldFootMats[2][4][4]; // [esp+518h] [ebp-29Ch] BYREF
    float *v204; // [esp+598h] [ebp-21Ch]
    float *v205; // [esp+59Ch] [ebp-218h]
    float *v206; // [esp+5A0h] [ebp-214h]
    float *v207; // [esp+5A4h] [ebp-210h]
    float tmpMat[4][4]; // [esp+5A8h] [ebp-20Ch] BYREF
    float *v209; // [esp+5F4h] [ebp-1C0h]
    float *v210; // [esp+5F8h] [ebp-1BCh]
    float *v211; // [esp+5FCh] [ebp-1B8h]
    __int64 v212; // [esp+600h] [ebp-1B4h]
    __int64 v213; // [esp+608h] [ebp-1ACh]
    __int64 v214; // [esp+610h] [ebp-1A4h]
    __int64 v215; // [esp+618h] [ebp-19Ch]
    __int64 v216; // [esp+620h] [ebp-194h]
    __int64 v217; // [esp+628h] [ebp-18Ch]
    __int64 v218; // [esp+630h] [ebp-184h]
    __int64 v219; // [esp+638h] [ebp-17Ch]
    float *v220; // [esp+644h] [ebp-170h]
    float *v221; // [esp+648h] [ebp-16Ch]
    int i; // [esp+64Ch] [ebp-168h]
    float *v223; // [esp+650h] [ebp-164h]
    float *v224; // [esp+654h] [ebp-160h]
    float footTraceLift; // [esp+658h] [ebp-15Ch]
    float v226; // [esp+65Ch] [ebp-158h]
    float minPelvisHeight; // [esp+660h] [ebp-154h]
    float v228; // [esp+664h] [ebp-150h]
    float v229; // [esp+668h] [ebp-14Ch]
    float v230; // [esp+66Ch] [ebp-148h]
    float maxFootRaise; // [esp+670h] [ebp-144h]
    float v232; // [esp+674h] [ebp-140h]
    float maxFootDrop; // [esp+678h] [ebp-13Ch]
    float v234; // [esp+67Ch] [ebp-138h]
    float velTimeScalar; // [esp+680h] [ebp-134h]
    float velSpeedScale; // [esp+684h] [ebp-130h]
    float v237; // [esp+688h] [ebp-12Ch]
    float v238; // [esp+68Ch] [ebp-128h]
    float velSpeed; // [esp+690h] [ebp-124h]
    float *v240; // [esp+694h] [ebp-120h]
    float *lastTerrainMappingOrigin; // [esp+698h] [ebp-11Ch]
    bool doLerping; // [esp+69Fh] [ebp-115h]
    BOOL v243; // [esp+6A0h] [ebp-114h]
    float originChange[5]; // [esp+6A4h] [ebp-110h]
    float footTraceEndOffsetZ; // [esp+6B8h] [ebp-FCh]
    bool fastLerp; // [esp+6BFh] [ebp-F5h]
    float frametimeScalar; // [esp+6C0h] [ebp-F4h]
    float maxFootRaiseTurret; // [esp+6C4h] [ebp-F0h]
    float maxFootRaiseCrouching; // [esp+6C8h] [ebp-ECh]
    float maxFootRaiseStanding; // [esp+6CCh] [ebp-E8h]
    float maxFootDropMoving; // [esp+6D0h] [ebp-E4h]
    float maxFootDropIdleCr; // [esp+6D4h] [ebp-E0h]
    float maxFootDropIdle; // [esp+6D8h] [ebp-DCh]
    float maxFootSpan; // [esp+6DCh] [ebp-D8h]
    float minPelvisHeightTurret; // [esp+6E0h] [ebp-D4h]
    float minPelvisHeightCrouching; // [esp+6E4h] [ebp-D0h]
    float minPelvisHeightNormal; // [esp+6E8h] [ebp-CCh]
    float footInAirLiftReductionMaxInAir; // [esp+6ECh] [ebp-C8h]
    float maxPelvisReachAdjust; // [esp+6F0h] [ebp-C4h]
    float footTraceEndOffsetZIdle[2]; // [esp+6F4h] [ebp-C0h]
    float footTraceEndOffsetZMoving; // [esp+6FCh] [ebp-B8h]
    float entityBoundsMinZ; // [esp+700h] [ebp-B4h]
    float footTraceLiftTurret; // [esp+704h] [ebp-B0h]
    float footTraceLiftNormal; // [esp+708h] [ebp-ACh]
    float footTraceOffsetFromAnkle[2]; // [esp+70Ch] [ebp-A8h]
    float IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MIN; // [esp+714h] [ebp-A0h]
    float IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX; // [esp+718h] [ebp-9Ch]
    float IK_TERRAIN_MAPPING_VELOCITY_SCALAR[2]; // [esp+71Ch] [ebp-98h]
    float IK_TERRAIN_MAPPING_VELOCITY_MAX; // [esp+724h] [ebp-90h]
    float IK_TERRAIN_MAPPING_CLAMP; // [esp+728h] [ebp-8Ch]
    float IK_TERRAIN_MAPPING_LERP_SCALE; // [esp+72Ch] [ebp-88h]
    float IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE; // [esp+730h] [ebp-84h]
    float IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE; // [esp+734h] [ebp-80h]
    float frametimeBase; // [esp+738h] [ebp-7Ch]
    float frametime; // [esp+73Ch] [ebp-78h]
    int v278; // [esp+74Ch] [ebp-68h]
    float v279; // [esp+750h] [ebp-64h]
    bool bIsClient; // [esp+757h] [ebp-5Dh]
    float pelvisMat[4][4]; // [esp+758h] [ebp-5Ch] BYREF
    float layerLerp; // [esp+79Ch] [ebp-18h]
    IKLayerNames layerName; // [esp+7A0h] [ebp-14h]
    //_UNKNOWN *v286; // [esp+7A8h] [ebp-Ch]
    //IKState *ikStatea; // [esp+7ACh] [ebp-8h]
    //int vars0; // [esp+7B4h] [ebp+0h]
    //
    //v286 = a1;
    //ikStatea = (IKState *)vars0;
    layerName = IKLAYER_TERRAIN_MAPPING;
    layerLerp = IKImport_GetLayerLerp(ikState, IKLAYER_TERRAIN_MAPPING, 0.1f);
    if (layerLerp > 0.0)
    {
        bIsClient = ikState->entityNum < 32;
        if (ikState->bJointVarsValid)
        {
            if (IKImport_GetVar_IK_Foot_Test())
            {
                if (ikState->ikBoneToObjBone[18] != 161
                    && ikState->ikBoneToObjBone[21] != 161
                    && ikState->lastFrameMS[9] != ikState->timeMS)
                {
                    ikState->lastFrameMS[9] = ikState->timeMS;
                    if (IK_IsCalcBone(ikState, 18))
                    {
                        v279 = (float)(ikState->lastFrameMS[9] % 2000) - 1000.0;
                        v278 = fabs(v279);
                        ikState->footMats[0][3][2] = (float)((float)(6.0 * (fabs(v279))) / 1000.0)
                            + ikState->footMats[0][3][2];
                    }
                    if (IK_IsCalcBone(ikState, 21))
                        ikState->footMats[1][3][2] = (float)((float)(1.0
                            - (float)((fabs((float)(ikState->lastFrameMS[9] % 2000) - 1000.0))
                                / 1000.0))
                            * 6.0)
                        + ikState->footMats[1][3][2];
                }
            }
            else if (ikState->ikBoneToObjBone[18] != 161
                && ikState->ikBoneToObjBone[21] != 161
                && (IK_IsCalcBone(ikState, 18) || IK_IsCalcBone(ikState, 21)))
            {
                frametime = (float)(ikState->timeMS - ikState->lastFrameMS[9]) * 0.001;
                if (frametime <= 0.0)
                {
                    if (Demo_IsPlaying())
                        frametime = 0.1f;
                    else
                        frametime = 0.0f;
                }
                ikState->lastFrameMS[9] = ikState->timeMS;
                frametimeBase = frametime;
                IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE = 30.0f;
                IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE = 30.0f;
                IK_TERRAIN_MAPPING_LERP_SCALE = 6.0f;
                IK_TERRAIN_MAPPING_CLAMP = 32.0f;
                IK_TERRAIN_MAPPING_VELOCITY_MAX = 300.0f;
                IK_TERRAIN_MAPPING_VELOCITY_SCALAR[0] = 7.0f;
                IK_TERRAIN_MAPPING_VELOCITY_SCALAR[1] = 7.0f;
                IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX = 8.0f;
                IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MIN = 1.0f;
                footTraceOffsetFromAnkle[0] = 0.0f;
                footTraceOffsetFromAnkle[1] = 8.0f;
                footTraceLiftNormal = 24.0f;
                footTraceLiftTurret = 30.0f;
                entityBoundsMinZ = 0.0f;
                footTraceEndOffsetZMoving = 40.0f;
                footTraceEndOffsetZIdle[0] = 20.0f;
                footTraceEndOffsetZIdle[1] = 20.0f;
                maxPelvisReachAdjust = -0.5f;
                footInAirLiftReductionMaxInAir = 16.0f;
                minPelvisHeightNormal = 6.0f;
                minPelvisHeightCrouching = 13.0f;
                minPelvisHeightTurret = 16.0f;
                maxFootSpan = 18.0f;
                maxFootDropIdle = 12.0f;
                maxFootDropIdleCr = 15.0f;
                maxFootDropMoving = 30.0f;
                maxFootRaiseStanding = 17.0f;
                maxFootRaiseCrouching = 17.0f;
                maxFootRaiseTurret = 27.0f;
                frametimeScalar = 1.0f;
                fastLerp = 0;
                footTraceEndOffsetZ = footTraceEndOffsetZIdle[bIsClient];
                ikCalcBoneModelMatrix(ikState, 1, (*ikState->matArrayPostIK)[0], pelvisMat);
                //LODWORD(originChange[4]) = ikState->lastTerrainMappingOrigin;
                //LODWORD(originChange[3]) = ikState->entityXform[3];
                originChange[0] = ikState->entityXform[3][0] - ikState->lastTerrainMappingOrigin[0];
                originChange[1] = ikState->entityXform[3][1] - ikState->lastTerrainMappingOrigin[1];
                originChange[2] = ikState->entityXform[3][2] - ikState->lastTerrainMappingOrigin[2];
                v243 = (float)((float)((float)(originChange[0] * originChange[0]) + (float)(originChange[1] * originChange[1]))
                    + (float)(originChange[2] * originChange[2])) < 4096.0;
                doLerping = (float)((float)((float)(originChange[0] * originChange[0])
                    + (float)(originChange[1] * originChange[1]))
                    + (float)(originChange[2] * originChange[2])) < 4096.0;
                if (frametime > frametimeCap)
                {
                    if (frametime > frametimeResetCap)
                    {
                        lastTerrainMappingOrigin = ikState->lastTerrainMappingOrigin;
                        v240 = ikState->entityXform[3];
                        ikState->lastTerrainMappingOrigin[0] = ikState->entityXform[3][0];
                        lastTerrainMappingOrigin[1] = v240[1];
                        lastTerrainMappingOrigin[2] = v240[2];
                        ikState->lastTerrainMappingFootOffsetZ[0] = 0.0f;
                        ikState->lastTerrainMappingFootOffsetZ[1] = 0.0f;
                        ikState->lastTerrainMappingPelvisZ = pelvisMat[3][2] + ikState->entityXform[3][2];
                    }
                    frametime = frametimeCapTime;
                    frametimeBase = frametimeCapTime;
                    doLerping = 0;
                }
                velSpeed = Abs(ikState->velocity);
                v238 = velSpeed / IK_TERRAIN_MAPPING_VELOCITY_MAX;
                if ((float)(1.0 - (float)(velSpeed / IK_TERRAIN_MAPPING_VELOCITY_MAX)) < 0.0)
                    v237 = 1.0f;
                else
                    v237 = v238;
                velSpeedScale = v237;
                velTimeScalar = 1.0f;
                velTimeScalar = (float)((float)(IK_TERRAIN_MAPPING_VELOCITY_SCALAR[bIsClient]
                    - IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MIN)
                    * v237)
                    + IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MIN;
                footTraceEndOffsetZ = (float)((float)(1.0 - v237) * footTraceEndOffsetZIdle[bIsClient])
                    + (float)(footTraceEndOffsetZMoving * v237);
                v234 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
                maxFootDrop = (float)((float)(1.0 - velSpeedScale)
                    * (float)((float)((float)(1.0 - v234) * maxFootDropIdle) + (float)(maxFootDropIdleCr * v234)))
                    + (float)(maxFootDropMoving * velSpeedScale);
                v232 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
                v230 = IKImport_GetLayerLerp(ikState, IKLAYER_TURRET_OFFSET);
                maxFootRaise = (float)((float)(1.0 - v230)
                    * (float)((float)((float)(1.0 - v232) * maxFootRaiseStanding)
                        + (float)(maxFootRaiseCrouching * v232)))
                    + (float)(maxFootRaiseTurret * v230);
                v229 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
                v228 = IKImport_GetLayerLerp(ikState, IKLAYER_TURRET_OFFSET);
                minPelvisHeight = (float)((float)((float)((float)(1.0 - v229) * minPelvisHeightNormal)
                    + (float)(minPelvisHeightCrouching * v229))
                    * (float)(1.0 - v228))
                    + (float)(minPelvisHeightTurret * v228);
                v226 = IKImport_GetLayerLerp(ikState, IKLAYER_TURRET_OFFSET);
                footTraceLift = (float)((float)(1.0 - v226) * footTraceLiftNormal) + (float)(footTraceLiftTurret * v226);
                v224 = ikState->lastTerrainMappingOrigin;
                v223 = ikState->entityXform[3];
                ikState->lastTerrainMappingOrigin[0] = ikState->entityXform[3][0];
                v224[1] = v223[1];
                v224[2] = v223[2];
                for (i = 0; i < 2; ++i)
                {
                    v221 = ikState->footMats[i][0];
                    v220 = v221;
                    v212 = *(_QWORD *)v221;
                    v213 = *(_QWORD *)&ikState->footMats[i][0][2];
                    v211 = ikState->footMats[i][1];
                    v214 = *(_QWORD *)v211;
                    v215 = *(_QWORD *)&ikState->footMats[i][1][2];
                    v210 = ikState->footMats[i][2];
                    v216 = *(_QWORD *)v210;
                    v217 = *(_QWORD *)&ikState->footMats[i][2][2];
                    v209 = ikState->footMats[i][3];
                    v218 = *(_QWORD *)v209;
                    v219 = *(_QWORD *)&ikState->footMats[i][3][2];
                    v207 = tmpMat[0];
                    *(_QWORD *)&tmpMat[0][0] = v212;
                    *(_QWORD *)&tmpMat[0][2] = v213;
                    v206 = tmpMat[1];
                    *(_QWORD *)&tmpMat[1][0] = v214;
                    *(_QWORD *)&tmpMat[1][2] = v215;
                    v205 = tmpMat[2];
                    *(_QWORD *)&tmpMat[2][0] = v216;
                    *(_QWORD *)&tmpMat[2][2] = v217;
                    v204 = tmpMat[3];
                    *(_QWORD *)&tmpMat[3][0] = v218;
                    *(_QWORD *)&tmpMat[3][2] = v219;
                    v202 = worldFootMats[i][0];
                    v201 = ikState->entityLegsXform[0];
                    v200 = tmpMat[0];
                    v192 = v212;
                    v193 = v213;
                    v191 = tmpMat[1];
                    v194 = v214;
                    v195 = v215;
                    v190 = tmpMat[2];
                    v196 = v216;
                    v197 = v217;
                    v189 = tmpMat[3];
                    v198 = v218;
                    v199 = v219;
                    v188 = ikState->entityLegsXform[0];
                    v177 = ikState->entityLegsXform[0][0];
                    v178 = *(_QWORD *)&ikState->entityLegsXform[0][1];
                    v179 = ikState->entityLegsXform[0][3];
                    v176 = ikState->entityLegsXform[1];
                    v180 = ikState->entityLegsXform[1][0];
                    v181 = *(_QWORD *)&ikState->entityLegsXform[1][1];
                    v182 = ikState->entityLegsXform[1][3];
                    v175 = ikState->entityLegsXform[2];
                    v183 = ikState->entityLegsXform[2][0];
                    v184 = *(_QWORD *)&ikState->entityLegsXform[2][1];
                    v185 = ikState->entityLegsXform[2][3];
                    v174 = ikState->entityLegsXform[3];
                    v186 = *(_QWORD *)&ikState->entityLegsXform[3][0];
                    v187 = *(_QWORD *)&ikState->entityLegsXform[3][2];
                    v158 = (float)((float)((float)(*(float *)&v212 * v177) + (float)(*((float *)&v212 + 1) * v180))
                        + (float)(*(float *)&v213 * v183))
                        + (float)(*((float *)&v213 + 1) * *(float *)&v186);
                    v159 = (float)((float)((float)(*(float *)&v212 * *(float *)&v178)
                        + (float)(*((float *)&v212 + 1) * *(float *)&v181))
                        + (float)(*(float *)&v213 * *(float *)&v184))
                        + (float)(*((float *)&v213 + 1) * *((float *)&v186 + 1));
                    v160 = (float)((float)((float)(*(float *)&v212 * *((float *)&v178 + 1))
                        + (float)(*((float *)&v212 + 1) * *((float *)&v181 + 1)))
                        + (float)(*(float *)&v213 * *((float *)&v184 + 1)))
                        + (float)(*((float *)&v213 + 1) * *(float *)&v187);
                    v161 = (float)((float)((float)(*(float *)&v212 * v179) + (float)(*((float *)&v212 + 1) * v182))
                        + (float)(*(float *)&v213 * v185))
                        + (float)(*((float *)&v213 + 1) * *((float *)&v187 + 1));
                    v162 = (float)((float)((float)(*(float *)&v214 * v177) + (float)(*((float *)&v214 + 1) * v180))
                        + (float)(*(float *)&v215 * v183))
                        + (float)(*((float *)&v215 + 1) * *(float *)&v186);
                    v163 = (float)((float)((float)(*(float *)&v214 * *(float *)&v178)
                        + (float)(*((float *)&v214 + 1) * *(float *)&v181))
                        + (float)(*(float *)&v215 * *(float *)&v184))
                        + (float)(*((float *)&v215 + 1) * *((float *)&v186 + 1));
                    v164 = (float)((float)((float)(*(float *)&v214 * *((float *)&v178 + 1))
                        + (float)(*((float *)&v214 + 1) * *((float *)&v181 + 1)))
                        + (float)(*(float *)&v215 * *((float *)&v184 + 1)))
                        + (float)(*((float *)&v215 + 1) * *(float *)&v187);
                    v165 = (float)((float)((float)(*(float *)&v214 * v179) + (float)(*((float *)&v214 + 1) * v182))
                        + (float)(*(float *)&v215 * v185))
                        + (float)(*((float *)&v215 + 1) * *((float *)&v187 + 1));
                    v166 = (float)((float)((float)(*(float *)&v216 * v177) + (float)(*((float *)&v216 + 1) * v180))
                        + (float)(*(float *)&v217 * v183))
                        + (float)(*((float *)&v217 + 1) * *(float *)&v186);
                    v167 = (float)((float)((float)(*(float *)&v216 * *(float *)&v178)
                        + (float)(*((float *)&v216 + 1) * *(float *)&v181))
                        + (float)(*(float *)&v217 * *(float *)&v184))
                        + (float)(*((float *)&v217 + 1) * *((float *)&v186 + 1));
                    v168 = (float)((float)((float)(*(float *)&v216 * *((float *)&v178 + 1))
                        + (float)(*((float *)&v216 + 1) * *((float *)&v181 + 1)))
                        + (float)(*(float *)&v217 * *((float *)&v184 + 1)))
                        + (float)(*((float *)&v217 + 1) * *(float *)&v187);
                    v169 = (float)((float)((float)(*(float *)&v216 * v179) + (float)(*((float *)&v216 + 1) * v182))
                        + (float)(*(float *)&v217 * v185))
                        + (float)(*((float *)&v217 + 1) * *((float *)&v187 + 1));
                    v170 = (float)((float)((float)(*(float *)&v218 * v177) + (float)(*((float *)&v218 + 1) * v180))
                        + (float)(*(float *)&v219 * v183))
                        + (float)(*((float *)&v219 + 1) * *(float *)&v186);
                    v171 = (float)((float)((float)(*(float *)&v218 * *(float *)&v178)
                        + (float)(*((float *)&v218 + 1) * *(float *)&v181))
                        + (float)(*(float *)&v219 * *(float *)&v184))
                        + (float)(*((float *)&v219 + 1) * *((float *)&v186 + 1));
                    v172 = (float)((float)((float)(*(float *)&v218 * *((float *)&v178 + 1))
                        + (float)(*((float *)&v218 + 1) * *((float *)&v181 + 1)))
                        + (float)(*(float *)&v219 * *((float *)&v184 + 1)))
                        + (float)(*((float *)&v219 + 1) * *(float *)&v187);
                    v173 = (float)((float)((float)(*(float *)&v218 * v179) + (float)(*((float *)&v218 + 1) * v182))
                        + (float)(*(float *)&v219 * v185))
                        + (float)(*((float *)&v219 + 1) * *((float *)&v187 + 1));
                    v157 = v202;
                    v2 = v202;
                    *v202 = v158;
                    v2[1] = v159;
                    v2[2] = v160;
                    v2[3] = v161;
                    v156 = v202 + 4;
                    v3 = v202 + 4;
                    v202[4] = v162;
                    v3[1] = v163;
                    v3[2] = v164;
                    v3[3] = v165;
                    v155 = v202 + 8;
                    v4 = v202 + 8;
                    v202[8] = v166;
                    v4[1] = v167;
                    v4[2] = v168;
                    v4[3] = v169;
                    v154 = v202 + 12;
                    v5 = v202 + 12;
                    v202[12] = v170;
                    v5[1] = v171;
                    v5[2] = v172;
                    v5[3] = v173;
                }
                lowestGroundZ = 9999999.0f;
                highestGroundZ = -9999999.0f;
                maxPelvisReach = (float)((float)(ikState->jointVars[2].LowerLength + ikState->jointVars[2].UpperLength)
                    + ikState->footHeightFromGround)
                    + maxPelvisReachAdjust;
                for (i = 0; i < 2; ++i)
                {
                    frametime = frametimeBase;
                    terrainZ = 0.0f;
                    groundOffsets[i] = 0.0f;
                    frametimeScalar = 1.0f;
                    modifiedFeet[i] = 0;
                    footTraceBoxRadius = 3.0f;
                    if (IKImport_GetVar_IK_Debug() == 1)
                        IKImport_DrawLine(startPos, endPos, i);
                    v144 = Abs(ikState->lastFootTraceGoalNormals[i]) - 1.0;
                    if ((fabs(v144)) >= 0.1)
                    {
                        v140 = footTraceNormals[i];
                        *v140 = 0.0f;
                        v140[1] = 0.0f;
                        v140[2] = 1.0f;
                        v139 = ikState->lastFootTraceGoalNormals[i];
                        v138 = footTraceNormals[i];
                        *v139 = *v138;
                        v139[1] = v138[1];
                        v139[2] = v138[2];
                    }
                    else
                    {
                        v142 = footTraceNormals[i];
                        v141 = ikState->lastFootTraceGoalNormals[i];
                        *v142 = *v141;
                        v142[1] = v141[1];
                        v142[2] = v141[2];
                    }
                    footTraceCacheDistThreshold = 0.2f;
                    v136 = worldFootMats[i][3];
                    v135 = ikState->lastTerrainMappingTraceFootPos[i];
                    v134[0] = *v136 - *v135;
                    v134[1] = v136[1] - ikState->lastTerrainMappingTraceFootPos[i][1];
                    v6 = Vec2Length(v134);
                    if (v6 <= footTraceCacheDistThreshold)
                    {
                        v102 = worldFootMats[i][3];
                        v101 = ikState->lastTerrainMappingTraceFootPos[i];
                        v100[0] = *v102 - *v101;
                        v100[1] = v102[1] - ikState->lastTerrainMappingTraceFootPos[i][1];
                        v10 = Vec2Length(v100);
                        if (footTraceCacheDistThreshold < v10)
                            v11 = ikState->lastTerrainMappingFootOffsetZ[i];
                        else
                            v11 = ikState->lastTerrainMappingTraceZ[i];
                        terrainZ = v11;
                    }
                    else
                    {
                        traceHitSomething = 0;
                        maxTraceZ = -99999.0f;
                        for (n = 0; n < 2; ++n)
                        {
                            v130 = worldFootMats[i][1];
                            v129 = (float)(2 * (i == 0) - 1) * footTraceOffsetFromAnkle[n];
                            v128 = worldFootMats[i][3];
                            startPos[0] = (float)(v129 * *v130) + *v128;
                            startPos[1] = (float)(v129 * v130[1]) + v128[1];
                            startPos[2] = (float)(v129 * v130[2]) + v128[2];
                            startPos[2] = ikState->entityXform[3][2] + footTraceLift;
                            endPos[0] = startPos[0];
                            endPos[1] = startPos[1];
                            endPos[2] = startPos[2];
                            endPos[2] = (float)((float)(ikState->entityXform[3][2] - entityBoundsMinZ) - ikState->footHeightFromGround)
                                - footTraceEndOffsetZ;
                            if (IKImport_TraceBox(
                                ikState,
                                startPos,
                                endPos,
                                footTraceBoxRadius,
                                thisTracePos,
                                thisNormal,
                                n + 2 * i,
                                0,
                                1))
                            {
                                if (thisTracePos[2] > maxTraceZ)
                                {
                                    tracePos[0] = thisTracePos[0];
                                    tracePos[1] = thisTracePos[1];
                                    tracePos[2] = thisTracePos[2];
                                    v124 = Abs(thisNormal) - 1.0;
                                    if ((fabs(v124)) < 0.1)
                                    {
                                        v123 = footTraceNormals[i];
                                        *v123 = thisNormal[0];
                                        v123[1] = thisNormal[1];
                                        v123[2] = thisNormal[2];
                                        v122 = ikState->lastFootTraceGoalNormals[i];
                                        *v122 = thisNormal[0];
                                        v122[1] = thisNormal[1];
                                        v122[2] = thisNormal[2];
                                    }
                                    maxTraceZ = thisTracePos[2];
                                }
                                traceHitSomething = 1;
                            }
                        }
                        if (traceHitSomething)
                        {
                            traceFootAdjust = 0.0f;
                            terrainZ = (float)(tracePos[2] - footTraceBoxRadius) + 0.0;
                            lowerFootTerrainSteepnessScale = -1.0f;
                            lowerFootTerrainDotScale = 5.0f;
                            lowerFootTerrainDotClampMin = 0.0f;
                            lowerFootTerrainDotClampMax = 1.0f;
                            v116 = worldFootMats[i][1];
                            v115 = ikState->lastFootTraceNormals[i];
                            (footDirectionToNormalDot) = -((float)(*v115 * *v116) + (float)(ikState->lastFootTraceNormals[i][1] * v116[1]));
                            if (i == 1)
                                footDirectionToNormalDot = -footDirectionToNormalDot;
                            if ((float)(footDirectionToNormalDot - lowerFootTerrainDotClampMax) < 0.0)
                                v7 = footDirectionToNormalDot;
                            else
                                v7 = lowerFootTerrainDotClampMax;
                            v113 = v7;
                            if ((float)(lowerFootTerrainDotClampMin - footDirectionToNormalDot) < 0.0)
                                v8 = v113;
                            else
                                v8 = lowerFootTerrainDotClampMin;
                            v112 = v8;
                            footDirectionToNormalDot = v8;
                            v9 = Vec2Length(ikState->lastFootTraceNormals[i]);
                            terrainZ = terrainZ - v9 * lowerFootTerrainSteepnessScale;
                            terrainZ = terrainZ - (float)(footDirectionToNormalDot * lowerFootTerrainDotScale);
                            if (ikState->footMats[i][3][2] > ikState->footHeightFromGround && terrainZ > ikState->entityXform[3][2])
                            {
                                v111 = (float)(terrainZ - ikState->entityXform[3][2]) / footInAirLiftReductionMaxInAir;
                                if (v111 <= 1.0)
                                    v110 = v111;
                                else
                                    v110 = 1.0f;
                                terrainZ = terrainZ
                                    - (float)((float)(ikState->footMats[i][3][2] - ikState->footHeightFromGround) * v110);
                            }
                            ikState->lastTerrainMappingTraceZ[i] = terrainZ;
                            if (IKImport_GetVar_IK_Debug() == 1)
                            {
                                start[2] = tracePos[2];
                                end[2] = tracePos[2];
                                start[0] = tracePos[0] + 4.0;
                                start[1] = tracePos[1] + 4.0;
                                end[0] = tracePos[0] - 4.0;
                                end[1] = tracePos[1] - 4.0;
                                IKImport_DrawLine(start, end, 3);
                                start[2] = tracePos[2];
                                end[2] = tracePos[2];
                                start[0] = tracePos[0] - 4.0;
                                start[1] = tracePos[1] + 4.0;
                                end[0] = tracePos[0] + 4.0;
                                end[1] = tracePos[1] - 4.0;
                                IKImport_DrawLine(start, end, 3);
                            }
                            if (ikState->lerpInfo[3].goalState)
                            {
                                v107 = ikState->lastTerrainMappingTraceFootPos[i];
                                v106 = worldFootMats[i][3];
                                *v107 = *v106;
                                v107[1] = v106[1];
                            }
                        }
                        if (!traceHitSomething)
                        {
                            terrainZ = ikState->entityXform[3][2];
                            ikState->lastTerrainMappingTraceZ[i] = terrainZ;
                            v105 = footTraceNormals[i];
                            *v105 = 0.0f;
                            v105[1] = 0.0f;
                            v105[2] = 1.0f;
                            v104 = ikState->lastFootTraceGoalNormals[i];
                            v103 = footTraceNormals[i];
                            *v104 = *v103;
                            v104[1] = v103[1];
                            v104[2] = v103[2];
                        }
                    }
                    if ((float)(ikState->entityXform[3][2] - maxFootDrop) > terrainZ)
                        terrainZ = ikState->entityXform[3][2] - maxFootDrop;
                    if (terrainZ > (float)(ikState->entityXform[3][2] + maxFootRaise))
                        terrainZ = ikState->entityXform[3][2] + maxFootRaise;
                    if (doLerping)
                    {
                        realZ = terrainZ;
                        terrainDiff = terrainZ - ikState->lastTerrainMappingFootOffsetZ[i];
                        terrainDiffNoLerpingThreshold = 128.0f;
                        if ((fabs(terrainDiff)) < 128.0)
                        {
                            frametimeScalar = velTimeScalar;
                            if (fastLerp)
                            {
                                fastLerpScalar[0] = 5.0f;
                                fastLerpScalar[1] = 10.0f;
                                frametimeScalar = frametimeScalar + fastLerpScalar[bIsClient];
                            }
                            else if ((fabs(realZ - terrainZ)) > IK_TERRAIN_MAPPING_CLAMP)
                            {
                                v95 = (float)((float)((fabs(realZ - terrainZ))
                                    / IK_TERRAIN_MAPPING_CLAMP)
                                    * 3.0)
                                    + 1.0;
                                if (frametimeScalar <= v95)
                                    v12 = v95;
                                else
                                    v12 = frametimeScalar;
                                v94 = v12;
                                frametimeScalar = frametimeScalar + v12;
                            }
                            if (!fastLerp && frametimeScalar > IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX)
                                frametimeScalar = IK_TERRAIN_MAPPING_FRAMETIME_SCALAR_MAX;
                            frametime = frametime * frametimeScalar;
                            terrainDiff = (float)(IK_TERRAIN_MAPPING_LERP_SCALE * frametime) * terrainDiff;
                            if (terrainDiff <= 0.0)
                            {
                                if (terrainDiff > (float)((-(IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE)) * frametimeBase))
                                    terrainDiff = (-(IK_TERRAIN_MAPPING_LERP_DOWN_MIN_RATE)) * frametimeBase;
                                v91 = ikState->lastTerrainMappingFootOffsetZ[i] + terrainDiff;
                                if (realZ <= v91)
                                    v14 = v91;
                                else
                                    v14 = realZ;
                                v90 = v14;
                                terrainZ = v14;
                            }
                            else
                            {
                                if ((float)(IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE * frametimeBase) > terrainDiff)
                                    terrainDiff = IK_TERRAIN_MAPPING_LERP_UP_MIN_RATE * frametimeBase;
                                v93 = ikState->lastTerrainMappingFootOffsetZ[i] + terrainDiff;
                                if (v93 <= realZ)
                                    v13 = v93;
                                else
                                    v13 = realZ;
                                v92 = v13;
                                terrainZ = v13;
                            }
                        }
                    }
                    footOffsetFromStandard = terrainZ - ikState->entityXform[3][2];
                    footOffsetAdjusted = footOffsetFromStandard;
                    if (footOffsetFromStandard < 0.0)
                    {
                        minScale = 1.0f;
                        v15 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
                        v86 = v15 + 1.0 - velSpeedScale;
                        if ((float)(v86 - 1.0) < 0.0)
                            v85 = v86;
                        else
                            v85 = 1.0f;
                        if ((float)(minScale - v86) < 0.0)
                            v16 = v85;
                        else
                            v16 = minScale;
                        v84 = v16;
                        footOffsetAdjusted = footOffsetAdjusted * v16;
                    }
                    ikState->footMats[i][3][2] = (float)(layerLerp * footOffsetAdjusted) + ikState->footMats[i][3][2];
                    if (lowestGroundZ > terrainZ)
                        lowestGroundZ = terrainZ;
                    if (terrainZ > highestGroundZ)
                        highestGroundZ = terrainZ;
                    ikState->lastTerrainMappingFootOffsetZ[i] = terrainZ;
                }
                v83 = ikState->lastTerrainMappingFootOffsetZ[0] - ikState->lastTerrainMappingFootOffsetZ[1];
                if ((fabs(v83)) > maxFootSpan)
                {
                    lowerFoot = ikState->lastTerrainMappingFootOffsetZ[0] > ikState->lastTerrainMappingFootOffsetZ[1];
                    lowerAdjust = (float)((float)(ikState->lastTerrainMappingFootOffsetZ[lowerFoot == 0]
                        - ikState->lastTerrainMappingFootOffsetZ[lowerFoot])
                        - maxFootSpan)
                        * layerLerp;
                    ikState->footMats[lowerFoot][3][2] = ikState->footMats[lowerFoot][3][2] + lowerAdjust;
                    ikState->lastTerrainMappingFootOffsetZ[lowerFoot] = ikState->lastTerrainMappingFootOffsetZ[lowerFoot]
                        + lowerAdjust;
                    lowestGroundZ = lowestGroundZ + lowerAdjust;
                }
                pelvisMoveScale = 0.0f;
                pelvisForceZAdjust = 0.0f;
                v78 = 0.75f;
                v77 = IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING);
                if ((float)(v77 - 1.0) < 0.0)
                    v76 = v77;
                else
                    v76 = 1.0f;
                if ((float)(v78 - v77) < 0.0)
                    v17 = v76;
                else
                    v17 = v78;
                v75 = v17;
                pelvisMoveScale = v17;
                pelvisMovingZAdjust = 0.0f;
                pelvisForceZAdjust = (1.0 - IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING)) * 0.0;
                if (IK_IsCalcBone(ikState, 1))
                {
                    zOfs = 0.0f;
                    PELVIS_ABS_SCALAR = 8.0f;
                    if (pelvisMat[3][2] <= (float)((float)(lowestGroundZ + maxPelvisReach) - ikState->entityXform[3][2]))
                    {
                        if ((float)((float)(highestGroundZ + minPelvisHeight) - ikState->entityXform[3][2]) <= pelvisMat[3][2])
                            v18 = pelvisMat[3][2] + ikState->entityXform[3][2];
                        else
                            v18 = highestGroundZ + minPelvisHeight;
                        realPelvisZ = v18;
                    }
                    else
                    {
                        realPelvisZ = lowestGroundZ + maxPelvisReach;
                    }
                    changeZ = realPelvisZ - ikState->lastTerrainMappingPelvisZ;
                    if (doLerping)
                    {
                        v69 = (float)((float)(frametimeBase * velTimeScalar) * pelvisMoveScale) * PELVIS_ABS_SCALAR;
                        if ((float)(1.0 - v69) < 0.0)
                            v68 = 1.0f;
                        else
                            v68 = v69;
                        changeZ = changeZ * v68;
                    }
                    zOfs = (float)(changeZ + ikState->lastTerrainMappingPelvisZ)
                        - (float)(pelvisMat[3][2] + ikState->entityXform[3][2]);
                    ikState->lastTerrainMappingPelvisZ = ikState->lastTerrainMappingPelvisZ + changeZ;
                    zOfs = zOfs * layerLerp;
                    (*ikState->matArrayPostIK)[7][2] = (*ikState->matArrayPostIK)[7][2] + zOfs;
                    if (!IK_GetStatus(ikState, IKSTATUS_MOUNTED_TURRET))
                    {
                        ikState->handMats[1][3][2] = ikState->handMats[1][3][2] + zOfs;
                        ikState->handMats[0][3][2] = ikState->handMats[0][3][2] + zOfs;
                    }
                    ikState->bHasActiveLayers = 1;
                    if (ikState->cacheActive)
                    {
                        memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                        ikState->cacheActive = 0;
                    }
                    ikState->modifiedIKBones |= 2u;
                }
                terrainLegExtendAdjust = 0.5f;
                if (!ikState->lerpInfo[1].goalState)
                {
                    for (i = 0; i < 2; ++i)
                    {
                        ikCalcBoneModelMatrix(
                            ikState,
                            ikState->localIkSystem->joints[i + 2].parentBone,
                            (*ikState->matArrayPostIK)[0],
                            parentMat);
                        v65 = ikState->footMats[i][3];
                        v64 = parentMat[3];
                        v62 = *v65 - parentMat[3][0];
                        v63 = ikState->footMats[i][3][1] - parentMat[3][1];
                        aSq = (float)(v62 * v62) + (float)(v63 * v63);
                        cSq = Vec3DistanceSq(parentMat[3], v65);
                        v59 = parentMat[3][2] - ikState->footMats[i][3][2];
                        v58 = fabs(v59);
                        if ((float)((fabs(v59)) * (fabs(v59))) > aSq)
                        {
                            v57 = (float)(ikState->jointVars[i + 2].LowerLength + ikState->jointVars[i + 2].UpperLength)
                                - terrainLegExtendAdjust;
                            cMaxSq = v57 * v57;
                            if (cSq > (float)(v57 * v57))
                            {
                                bSq = cMaxSq - aSq;
                                if ((float)(cMaxSq - aSq) > 0.1)
                                {
                                    v54 = sqrtf(bSq);
                                    ikState->footMats[i][3][2] = parentMat[3][2] - v54;
                                }
                            }
                        }
                    }
                }
                terrainNormalLerpTime = 0.2f;
                terrainNormalRangeCap = 0.40000001f;
                terrainNormalMinZ = 0.2f;
                for (i = 0; i < 2; ++i)
                {
                    v_up[0] = 0.0f;
                    v_up[1] = 0.0f;
                    v_up[2] = 1.0f;
                    angularMappingHeightOffsetMax = 4.0f;
                    v48 = (float)(ikState->footMats[i][3][2] - ikState->footHeightFromGround) / 4.0;
                    if ((float)(v48 - 1.0) < 0.0)
                        v47 = v48;
                    else
                        v47 = 1.0f;
                    if ((float)(0.0 - v48) < 0.0)
                        v46 = v47;
                    else
                        v46 = 0.0f;
                    footOffsetAngularScale = 1.0 - v46;
                    if (ikState->lerpInfo[3].goalState)
                        v44 = 1.0f;
                    else
                        v44 = footOffsetAngularScale;
                    angularWeight = v44;
                    Vec3Lerp(v_up, footTraceNormals[i], v44, footTraceNormals[i]);
                    if (terrainNormalMinZ > footTraceNormals[i][2])
                    {
                        v42 = footTraceNormals[i];
                        *v42 = v_up[0];
                        v42[1] = v_up[1];
                        v42[2] = v_up[2];
                    }
                    if (!doLerping
                        || ((v41 = ikState->lastFootTraceNormals[i], *v41 != 0.0) || v41[1] != 0.0 || v41[2] != 0.0
                            ? (v40 = 0)
                            : (v40 = 1),
                            v40
                            || (v39 = Abs(ikState->lastFootTraceNormals[i]) - 1.0, (fabs(v39)) >= 0.1)))
                    {
                        Vec3NormalizeTo(footTraceNormals[i], ikState->lastFootTraceNormals[i]);
                    }
                    else
                    {
                        v38 = frametime / terrainNormalLerpTime;
                        if ((float)((float)(frametime / terrainNormalLerpTime) - 1.0) < 0.0)
                            v37 = v38;
                        else
                            v37 = 1.0f;
                        if ((float)(0.0 - v38) < 0.0)
                            v36 = v37;
                        else
                            v36 = 0.0f;
                        Vec3Lerp(ikState->lastFootTraceNormals[i], footTraceNormals[i], v36, ikState->lastFootTraceNormals[i]);
                        Vec3Normalize(ikState->lastFootTraceNormals[i]);
                    }
                    XYdist = Vec2Length(ikState->lastFootTraceNormals[i]);
                    if (XYdist > terrainNormalRangeCap)
                    {
                        v34 = ikState->lastFootTraceNormals[i];
                        v33 = terrainNormalRangeCap / XYdist;
                        v32 = v34;
                        *v34 = (float)(terrainNormalRangeCap / XYdist) * *v34;
                        v34[1] = v33 * v32[1];
                        ikState->lastFootTraceNormals[i][2] = sqrtf(1.0 - (float)(terrainNormalRangeCap * terrainNormalRangeCap));
                        Vec3Normalize(ikState->lastFootTraceNormals[i]);
                    }
                    if (layerLerp >= 1.0)
                    {
                        v29 = ikState->lastFootTraceNormals[i];
                        v30[0] = *v29;
                        v30[1] = ikState->lastFootTraceNormals[i][1];
                        v30[2] = ikState->lastFootTraceNormals[i][2];
                    }
                    else
                    {
                        v31[0] = 0.0f;
                        v31[1] = 0.0f;
                        v31[2] = 1.0f;
                        Vec3Lerp(v31, ikState->lastFootTraceNormals[i], layerLerp, v30);
                        Vec3Normalize(v30);
                    }
                    ikMatrixIdentity44(terrainAxis);
                    v27 = ikState->entityLegsXform[0];
                    if (ikState->entityLegsXform == invXform
                        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 125, 0, "%s", "in != out"))
                    {
                        __debugbreak();
                    }
                    invXform[0][0] = *v27;
                    invXform[0][1] = v27[4];
                    invXform[0][2] = v27[8];
                    invXform[0][3] = v27[12];
                    invXform[1][0] = v27[1];
                    invXform[1][1] = v27[5];
                    invXform[1][2] = v27[9];
                    invXform[1][3] = v27[13];
                    invXform[2][0] = v27[2];
                    invXform[2][1] = v27[6];
                    invXform[2][2] = v27[10];
                    invXform[2][3] = v27[14];
                    invXform[3][0] = v27[3];
                    invXform[3][1] = v27[7];
                    invXform[3][2] = v27[11];
                    invXform[3][3] = v27[15];
                    ikMatrixTransformVector34(v30, invXform, terrainAxis[2]);
                    *(_QWORD *)&terrainAxis[0][0] = LODWORD(ikIdentityMatrix44[0][0]);
                    terrainAxis[0][2] = 0.0;
                    Vec3Cross(terrainAxis[2], terrainAxis[0], terrainAxis[1]);
                    Vec3Normalize(terrainAxis[1]);
                    Vec3Cross(terrainAxis[1], terrainAxis[2], terrainAxis[0]);
                    Vec3Normalize(terrainAxis[0]);
                    curFoot = *(_QWORD *)&ikState->footMats[i][0][0];
                    curFoot_8 = *(_QWORD *)&ikState->footMats[i][0][2];
                    curFoot_16 = *(_QWORD *)&ikState->footMats[i][1][0];
                    curFoot_24 = *(_QWORD *)&ikState->footMats[i][1][2];
                    curFoot_32 = *(_QWORD *)&ikState->footMats[i][2][0];
                    curFoot_40 = *(_QWORD *)&ikState->footMats[i][2][2];
                    v19 = ikState->footMats[i][0];
                    *v19 = (float)((float)(*(float *)&curFoot * terrainAxis[0][0])
                        + (float)(*((float *)&curFoot + 1) * terrainAxis[1][0]))
                        + (float)(*(float *)&curFoot_8 * terrainAxis[2][0]);
                    v19[1] = (float)((float)(*(float *)&curFoot * terrainAxis[0][1])
                        + (float)(*((float *)&curFoot + 1) * terrainAxis[1][1]))
                        + (float)(*(float *)&curFoot_8 * terrainAxis[2][1]);
                    v19[2] = (float)((float)(*(float *)&curFoot * terrainAxis[0][2])
                        + (float)(*((float *)&curFoot + 1) * terrainAxis[1][2]))
                        + (float)(*(float *)&curFoot_8 * terrainAxis[2][2]);
                    v19[3] = 0.0f;
                    v19[4] = (float)((float)(*(float *)&curFoot_16 * terrainAxis[0][0])
                        + (float)(*((float *)&curFoot_16 + 1) * terrainAxis[1][0]))
                        + (float)(*(float *)&curFoot_24 * terrainAxis[2][0]);
                    v19[5] = (float)((float)(*(float *)&curFoot_16 * terrainAxis[0][1])
                        + (float)(*((float *)&curFoot_16 + 1) * terrainAxis[1][1]))
                        + (float)(*(float *)&curFoot_24 * terrainAxis[2][1]);
                    v19[6] = (float)((float)(*(float *)&curFoot_16 * terrainAxis[0][2])
                        + (float)(*((float *)&curFoot_16 + 1) * terrainAxis[1][2]))
                        + (float)(*(float *)&curFoot_24 * terrainAxis[2][2]);
                    v19[7] = 0.0f;
                    v19[8] = (float)((float)(*(float *)&curFoot_32 * terrainAxis[0][0])
                        + (float)(*((float *)&curFoot_32 + 1) * terrainAxis[1][0]))
                        + (float)(*(float *)&curFoot_40 * terrainAxis[2][0]);
                    v19[9] = (float)((float)(*(float *)&curFoot_32 * terrainAxis[0][1])
                        + (float)(*((float *)&curFoot_32 + 1) * terrainAxis[1][1]))
                        + (float)(*(float *)&curFoot_40 * terrainAxis[2][1]);
                    v19[10] = (float)((float)(*(float *)&curFoot_32 * terrainAxis[0][2])
                        + (float)(*((float *)&curFoot_32 + 1) * terrainAxis[1][2]))
                        + (float)(*(float *)&curFoot_40 * terrainAxis[2][2]);
                    v19[11] = 0.0f;
                }
            }
        }
    }
    else
    {
        ikCalcBoneModelMatrix(ikState, 1, (*ikState->matArrayPostIK)[0], pelvisMat);
        ikState->lastTerrainMappingPelvisZ = pelvisMat[3][2] + ikState->entityXform[3][2];
    }
}

// local variable allocation has failed, the output may be wrong!
void    IK_Layer_ApplyFootIK(
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
    float v41; // [esp-Ch] [ebp-DCh] BYREF
    float v42; // [esp-8h] [ebp-D8h]
    float v43; // [esp-4h] [ebp-D4h]
    float baseMat_4; // [esp+4h] [ebp-CCh]
    float v45; // [esp+8h] [ebp-C8h]
    float v46; // [esp+Ch] [ebp-C4h]
    float v47; // [esp+14h] [ebp-BCh]
    float v48; // [esp+18h] [ebp-B8h]
    float v49; // [esp+1Ch] [ebp-B4h]
    float v50[4]; // [esp+24h] [ebp-ACh] BYREF
    float parentIKMat[4][4]; // [esp+34h] [ebp-9Ch] BYREF
    float jointIKMat[4][4]; // [esp+74h] [ebp-5Ch] BYREF
    bool flipHack; // [esp+C3h] [ebp-Dh]
    //_UNKNOWN *v55[2]; // [esp+C4h] [ebp-Ch] BYREF
    //IKJointVars *jointVarsa; // [esp+D0h] [ebp+0h]
    //
    //v55[0] = a1;
    //v55[1] = jointVarsa;
    flipHack = jointBones->parentBone == IKBONE_RTHIGH;
    if (ikState->bJointVarsValid)
    {
        if (ikState->ikBoneToObjBone[jointBones->childBone] != 161)
        {
            if (IK_IsCalcBone(ikState, jointBones->childBone))
            {
                if (flipHack)
                    IK_FlipHack(childMat);
                ikSolveLegJoint(ikState, jointBones, jointVars, &(*childMat)[12], parentIKMat, jointIKMat, flipHack);
                ikCalcBoneModelMatrix(ikState, jointBones->baseBone, (*ikState->matArrayPostIK)[0], (float (*)[4]) & v41);
                v40 = ikState->matArrayPostIK[jointBones->parentBone][0];
                if (&v41 == v31
                    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
                {
                    __debugbreak();
                }
                v31[0] = v41;
                v31[1] = baseMat_4;
                v31[2] = v47;
                v31[3] = 0.0f;
                v32 = v42;
                v33 = v45;
                v34 = v48;
                v35 = 0.0f;
                v36 = v43;
                v37 = v46;
                v38 = v49;
                v39 = 0.0f;
                *v40 = (float)((float)(parentIKMat[0][0] * v41) + (float)(parentIKMat[0][1] * v42))
                    + (float)(parentIKMat[0][2] * v43);
                v40[1] = (float)((float)(parentIKMat[0][0] * v31[1]) + (float)(parentIKMat[0][1] * v33))
                    + (float)(parentIKMat[0][2] * v37);
                v40[2] = (float)((float)(parentIKMat[0][0] * v31[2]) + (float)(parentIKMat[0][1] * v34))
                    + (float)(parentIKMat[0][2] * v38);
                v40[3] = 0.0f;
                v40[4] = (float)((float)(parentIKMat[1][0] * v31[0]) + (float)(parentIKMat[1][1] * v32))
                    + (float)(parentIKMat[1][2] * v36);
                v40[5] = (float)((float)(parentIKMat[1][0] * v31[1]) + (float)(parentIKMat[1][1] * v33))
                    + (float)(parentIKMat[1][2] * v37);
                v40[6] = (float)((float)(parentIKMat[1][0] * v31[2]) + (float)(parentIKMat[1][1] * v34))
                    + (float)(parentIKMat[1][2] * v38);
                v40[7] = 0.0f;
                v40[8] = (float)((float)(parentIKMat[2][0] * v31[0]) + (float)(parentIKMat[2][1] * v32))
                    + (float)(parentIKMat[2][2] * v36);
                v40[9] = (float)((float)(parentIKMat[2][0] * v31[1]) + (float)(parentIKMat[2][1] * v33))
                    + (float)(parentIKMat[2][2] * v37);
                v40[10] = (float)((float)(parentIKMat[2][0] * v31[2]) + (float)(parentIKMat[2][1] * v34))
                    + (float)(parentIKMat[2][2] * v38);
                v40[11] = 0.0f;
                //LODWORD(v30[4]) = v50;
                //LODWORD(v30[3]) = parentIKMat[3];
                v30[0] = parentIKMat[3][0] - v50[0];
                v30[1] = parentIKMat[3][1] - v50[1];
                v30[2] = parentIKMat[3][2] - v50[2];
                ikMatrixTransformVector34(v30, (const float (*)[4])v31, v40 + 12);
                v40[15] = 1.0f;
                ikNormalizedMatrixAssert_func(ikState->matArrayPostIK[jointBones->parentBone]);
                parentBone = jointBones->parentBone;
                ikState->bHasActiveLayers = 1;
                if (ikState->cacheActive)
                {
                    memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                    ikState->cacheActive = 0;
                }
                if (parentBone != IKBONE_NONE)
                    ikState->modifiedIKBones |= 1 << parentBone;
                v28 = ikState->matArrayPostIK[jointBones->jointBone][0];
                if (parentIKMat == (float (*)[4])v19
                    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
                {
                    __debugbreak();
                }
                v19[0] = parentIKMat[0][0];
                v19[1] = parentIKMat[1][0];
                v19[2] = parentIKMat[2][0];
                v19[3] = 0.0f;
                v20 = parentIKMat[0][1];
                v21 = parentIKMat[1][1];
                v22 = parentIKMat[2][1];
                v23 = 0.0f;
                v24 = parentIKMat[0][2];
                v25 = parentIKMat[1][2];
                v26 = parentIKMat[2][2];
                v27 = 0.0f;
                *v28 = (float)((float)(jointIKMat[0][0] * parentIKMat[0][0]) + (float)(jointIKMat[0][1] * parentIKMat[0][1]))
                    + (float)(jointIKMat[0][2] * parentIKMat[0][2]);
                v28[1] = (float)((float)(jointIKMat[0][0] * v19[1]) + (float)(jointIKMat[0][1] * v21))
                    + (float)(jointIKMat[0][2] * v25);
                v28[2] = (float)((float)(jointIKMat[0][0] * v19[2]) + (float)(jointIKMat[0][1] * v22))
                    + (float)(jointIKMat[0][2] * v26);
                v28[3] = 0.0f;
                v28[4] = (float)((float)(jointIKMat[1][0] * v19[0]) + (float)(jointIKMat[1][1] * v20))
                    + (float)(jointIKMat[1][2] * v24);
                v28[5] = (float)((float)(jointIKMat[1][0] * v19[1]) + (float)(jointIKMat[1][1] * v21))
                    + (float)(jointIKMat[1][2] * v25);
                v28[6] = (float)((float)(jointIKMat[1][0] * v19[2]) + (float)(jointIKMat[1][1] * v22))
                    + (float)(jointIKMat[1][2] * v26);
                v28[7] = 0.0f;
                v28[8] = (float)((float)(jointIKMat[2][0] * v19[0]) + (float)(jointIKMat[2][1] * v20))
                    + (float)(jointIKMat[2][2] * v24);
                v28[9] = (float)((float)(jointIKMat[2][0] * v19[1]) + (float)(jointIKMat[2][1] * v21))
                    + (float)(jointIKMat[2][2] * v25);
                v28[10] = (float)((float)(jointIKMat[2][0] * v19[2]) + (float)(jointIKMat[2][1] * v22))
                    + (float)(jointIKMat[2][2] * v26);
                v28[11] = 0.0f;
                //LODWORD(v18[4]) = parentIKMat[3];
                //LODWORD(v18[3]) = jointIKMat[3];
                v18[0] = jointIKMat[3][0] - parentIKMat[3][0];
                v18[1] = jointIKMat[3][1] - parentIKMat[3][1];
                v18[2] = jointIKMat[3][2] - parentIKMat[3][2];
                ikMatrixTransformVector34(v18, (const float (*)[4])v19, v28 + 12);
                v28[15] = 1.0f;
                ikNormalizedMatrixAssert_func(ikState->matArrayPostIK[jointBones->jointBone]);
                jointBone = jointBones->jointBone;
                ikState->bHasActiveLayers = 1;
                if (ikState->cacheActive)
                {
                    memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                    ikState->cacheActive = 0;
                }
                if (jointBone != IKBONE_NONE)
                    ikState->modifiedIKBones |= 1 << jointBone;
                if (jointVars->UpperLength + 1.0 <= Vec3Distance(parentIKMat[3], jointIKMat[3])
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                        728,
                        0,
                        "%s",
                        "Vec3Distance( parentIKMat[3], jointIKMat[3] ) < jointVars->UpperLength + 1.f"))
                {
                    __debugbreak();
                }
                if (jointVars->LowerLength + 5.0 <= Vec3Distance(&(*childMat)[12], jointIKMat[3])
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                        729,
                        0,
                        "%s",
                        "Vec3Distance( childMat[3], jointIKMat[3] ) < jointVars->LowerLength + 5.f"))
                {
                    __debugbreak();
                }
                if (flipHack)
                    IK_FlipHack(childMat);
                v16 = ikState->matArrayPostIK[jointBones->childBone][0];
                if (jointIKMat == (float (*)[4])v7
                    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
                {
                    __debugbreak();
                }
                v7[0] = jointIKMat[0][0];
                v7[1] = jointIKMat[1][0];
                v7[2] = jointIKMat[2][0];
                v7[3] = 0.0f;
                v8 = jointIKMat[0][1];
                v9 = jointIKMat[1][1];
                v10 = jointIKMat[2][1];
                v11 = 0.0f;
                v12 = jointIKMat[0][2];
                v13 = jointIKMat[1][2];
                v14 = jointIKMat[2][2];
                v15 = 0.0f;
                *v16 = (float)((float)((*childMat)[0] * jointIKMat[0][0]) + (float)((*childMat)[1] * jointIKMat[0][1]))
                    + (float)((*childMat)[2] * jointIKMat[0][2]);
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
                //LODWORD(v6[4]) = jointIKMat[3];
                //LODWORD(v6[3]) = &(*childMat)[12];
                v6[0] = (*childMat)[12] - jointIKMat[3][0];
                v6[1] = (*childMat)[13] - jointIKMat[3][1];
                v6[2] = (*childMat)[14] - jointIKMat[3][2];
                ikMatrixTransformVector34(v6, (const float (*)[4])v7, v16 + 12);
                v16[15] = 1.0f;
                ikNormalizedMatrixAssert_func(ikState->matArrayPostIK[jointBones->childBone]);
                childBone = jointBones->childBone;
                ikState->bHasActiveLayers = 1;
                if (ikState->cacheActive)
                {
                    memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                    ikState->cacheActive = 0;
                }
                if (childBone != IKBONE_NONE)
                    ikState->modifiedIKBones |= 1 << childBone;
            }
        }
    }
}

// local variable allocation has failed, the output may be wrong!
void    IK_Layer_ApplyHandIK(
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
    float out[4]; // [esp-5Ch] [ebp-12Ch] BYREF
    float v32; // [esp-4Ch] [ebp-11Ch]
    float v33; // [esp-48h] [ebp-118h]
    float v34; // [esp-44h] [ebp-114h]
    int v35; // [esp-40h] [ebp-110h]
    float v36; // [esp-3Ch] [ebp-10Ch]
    float v37; // [esp-38h] [ebp-108h]
    float v38; // [esp-34h] [ebp-104h]
    int v39; // [esp-30h] [ebp-100h]
    float *v40; // [esp-10h] [ebp-E0h]
    float in[4][4]; // [esp-Ch] [ebp-DCh] OVERLAPPED BYREF
    float parentIKMat[4][4]; // [esp+34h] [ebp-9Ch] BYREF
    float jointIKMat[4][4]; // [esp+74h] [ebp-5Ch] BYREF
    bool flipHack; // [esp+C3h] [ebp-Dh]
    //_UNKNOWN *v46[2]; // [esp+C4h] [ebp-Ch] BYREF
    //IKJointVars *jointVarsa; // [esp+D0h] [ebp+0h]
    //
    //v46[0] = a1;
    //v46[1] = jointVarsa;
    flipHack = jointBones->childBone == IKBONE_RHAND;
    if (ikState->bJointVarsValid)
    {
        if (ikState->ikBoneToObjBone[jointBones->childBone] != 161)
        {
            if (IK_IsCalcBone(ikState, jointBones->childBone))
            {
                if (flipHack)
                    IK_FlipHack(childMat);
                ikSolveArmJoint(ikState, jointBones, jointVars, &(*childMat)[12], parentIKMat, jointIKMat, flipHack);
                ikCalcBoneModelMatrix(ikState, jointBones->baseBone, (*ikState->matArrayPostIK)[0], in);
                v40 = ikState->matArrayPostIK[jointBones->parentBone][0];
                if (in == (float (*)[4])out
                    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
                {
                    __debugbreak();
                }
                out[0] = in[0][0];
                out[1] = in[1][0];
                out[2] = in[2][0];
                out[3] = 0.0f;
                v32 = in[0][1];
                v33 = in[1][1];
                v34 = in[2][1];
                v35 = 0.0f;
                v36 = in[0][2];
                v37 = in[1][2];
                v38 = in[2][2];
                v39 = 0.0f;
                *v40 = (float)((float)(parentIKMat[0][0] * in[0][0]) + (float)(parentIKMat[0][1] * in[0][1]))
                    + (float)(parentIKMat[0][2] * in[0][2]);
                v40[1] = (float)((float)(parentIKMat[0][0] * out[1]) + (float)(parentIKMat[0][1] * v33))
                    + (float)(parentIKMat[0][2] * v37);
                v40[2] = (float)((float)(parentIKMat[0][0] * out[2]) + (float)(parentIKMat[0][1] * v34))
                    + (float)(parentIKMat[0][2] * v38);
                v40[3] = 0.0f;
                v40[4] = (float)((float)(parentIKMat[1][0] * out[0]) + (float)(parentIKMat[1][1] * v32))
                    + (float)(parentIKMat[1][2] * v36);
                v40[5] = (float)((float)(parentIKMat[1][0] * out[1]) + (float)(parentIKMat[1][1] * v33))
                    + (float)(parentIKMat[1][2] * v37);
                v40[6] = (float)((float)(parentIKMat[1][0] * out[2]) + (float)(parentIKMat[1][1] * v34))
                    + (float)(parentIKMat[1][2] * v38);
                v40[7] = 0.0f;
                v40[8] = (float)((float)(parentIKMat[2][0] * out[0]) + (float)(parentIKMat[2][1] * v32))
                    + (float)(parentIKMat[2][2] * v36);
                v40[9] = (float)((float)(parentIKMat[2][0] * out[1]) + (float)(parentIKMat[2][1] * v33))
                    + (float)(parentIKMat[2][2] * v37);
                v40[10] = (float)((float)(parentIKMat[2][0] * out[2]) + (float)(parentIKMat[2][1] * v34))
                    + (float)(parentIKMat[2][2] * v38);
                v40[11] = 0.0f;
                //LODWORD(v30[4]) = in[3];
                //LODWORD(v30[3]) = parentIKMat[3];
                v30[0] = parentIKMat[3][0] - in[3][0];
                v30[1] = parentIKMat[3][1] - in[3][1];
                v30[2] = parentIKMat[3][2] - in[3][2];
                ikMatrixTransformVector34(v30, (const float (*)[4])out, v40 + 12);
                v40[15] = 1.0f;
                parentBone = jointBones->parentBone;
                ikState->bHasActiveLayers = 1;
                if (ikState->cacheActive)
                {
                    memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                    ikState->cacheActive = 0;
                }
                if (parentBone != IKBONE_NONE)
                    ikState->modifiedIKBones |= 1 << parentBone;
                v28 = ikState->matArrayPostIK[jointBones->jointBone][0];
                if (parentIKMat == (float (*)[4])v19
                    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
                {
                    __debugbreak();
                }
                v19[0] = parentIKMat[0][0];
                v19[1] = parentIKMat[1][0];
                v19[2] = parentIKMat[2][0];
                v19[3] = 0.0f;
                v20 = parentIKMat[0][1];
                v21 = parentIKMat[1][1];
                v22 = parentIKMat[2][1];
                v23 = 0.0f;
                v24 = parentIKMat[0][2];
                v25 = parentIKMat[1][2];
                v26 = parentIKMat[2][2];
                v27 = 0.0f;
                *v28 = (float)((float)(jointIKMat[0][0] * parentIKMat[0][0]) + (float)(jointIKMat[0][1] * parentIKMat[0][1]))
                    + (float)(jointIKMat[0][2] * parentIKMat[0][2]);
                v28[1] = (float)((float)(jointIKMat[0][0] * v19[1]) + (float)(jointIKMat[0][1] * v21))
                    + (float)(jointIKMat[0][2] * v25);
                v28[2] = (float)((float)(jointIKMat[0][0] * v19[2]) + (float)(jointIKMat[0][1] * v22))
                    + (float)(jointIKMat[0][2] * v26);
                v28[3] = 0.0f;
                v28[4] = (float)((float)(jointIKMat[1][0] * v19[0]) + (float)(jointIKMat[1][1] * v20))
                    + (float)(jointIKMat[1][2] * v24);
                v28[5] = (float)((float)(jointIKMat[1][0] * v19[1]) + (float)(jointIKMat[1][1] * v21))
                    + (float)(jointIKMat[1][2] * v25);
                v28[6] = (float)((float)(jointIKMat[1][0] * v19[2]) + (float)(jointIKMat[1][1] * v22))
                    + (float)(jointIKMat[1][2] * v26);
                v28[7] = 0.0f;
                v28[8] = (float)((float)(jointIKMat[2][0] * v19[0]) + (float)(jointIKMat[2][1] * v20))
                    + (float)(jointIKMat[2][2] * v24);
                v28[9] = (float)((float)(jointIKMat[2][0] * v19[1]) + (float)(jointIKMat[2][1] * v21))
                    + (float)(jointIKMat[2][2] * v25);
                v28[10] = (float)((float)(jointIKMat[2][0] * v19[2]) + (float)(jointIKMat[2][1] * v22))
                    + (float)(jointIKMat[2][2] * v26);
                v28[11] = 0.0f;
                //LODWORD(v18[4]) = parentIKMat[3];
                //LODWORD(v18[3]) = jointIKMat[3];
                v18[0] = jointIKMat[3][0] - parentIKMat[3][0];
                v18[1] = jointIKMat[3][1] - parentIKMat[3][1];
                v18[2] = jointIKMat[3][2] - parentIKMat[3][2];
                ikMatrixTransformVector34(v18, (const float (*)[4])v19, v28 + 12);
                v28[15] = 1.0f;
                jointBone = jointBones->jointBone;
                ikState->bHasActiveLayers = 1;
                if (ikState->cacheActive)
                {
                    memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                    ikState->cacheActive = 0;
                }
                if (jointBone != IKBONE_NONE)
                    ikState->modifiedIKBones |= 1 << jointBone;
                if (flipHack)
                    IK_FlipHack(childMat);
                v16 = ikState->matArrayPostIK[jointBones->childBone][0];
                if (jointIKMat == (float (*)[4])v7
                    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
                {
                    __debugbreak();
                }
                v7[0] = jointIKMat[0][0];
                v7[1] = jointIKMat[1][0];
                v7[2] = jointIKMat[2][0];
                v7[3] = 0.0f;
                v8 = jointIKMat[0][1];
                v9 = jointIKMat[1][1];
                v10 = jointIKMat[2][1];
                v11 = 0.0f;
                v12 = jointIKMat[0][2];
                v13 = jointIKMat[1][2];
                v14 = jointIKMat[2][2];
                v15 = 0.0f;
                *v16 = (float)((float)((*childMat)[0] * jointIKMat[0][0]) + (float)((*childMat)[1] * jointIKMat[0][1]))
                    + (float)((*childMat)[2] * jointIKMat[0][2]);
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
                //LODWORD(v6[4]) = jointIKMat[3];
                //LODWORD(v6[3]) = &(*childMat)[12];
                v6[0] = (*childMat)[12] - jointIKMat[3][0];
                v6[1] = (*childMat)[13] - jointIKMat[3][1];
                v6[2] = (*childMat)[14] - jointIKMat[3][2];
                ikMatrixTransformVector34(v6, (const float (*)[4])v7, v16 + 12);
                v16[15] = 1.0f;
                childBone = jointBones->childBone;
                ikState->bHasActiveLayers = 1;
                if (ikState->cacheActive)
                {
                    memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                    ikState->cacheActive = 0;
                }
                if (childBone != IKBONE_NONE)
                    ikState->modifiedIKBones |= 1 << childBone;
            }
        }
    }
}

// local variable allocation has failed, the output may be wrong!
void    IK_Layer_LeftHandOnGun(IKState *ikState)
{
    float *v2; // eax
    float *v3; // edx
    float *v4; // ecx
    float twistRot[3]; // [esp+14h] [ebp-600h] BYREF
    float twistScale; // [esp+20h] [ebp-5F4h]
    float v7[5]; // [esp+24h] [ebp-5F0h] BYREF
    float v8[4]; // [esp+38h] [ebp-5DCh] BYREF
    float v9; // [esp+48h] [ebp-5CCh]
    float v10; // [esp+4Ch] [ebp-5C8h]
    float v11; // [esp+50h] [ebp-5C4h]
    int v12; // [esp+54h] [ebp-5C0h]
    float v13; // [esp+58h] [ebp-5BCh]
    float v14; // [esp+5Ch] [ebp-5B8h]
    float v15; // [esp+60h] [ebp-5B4h]
    int v16; // [esp+64h] [ebp-5B0h]
    float *v17; // [esp+7Ch] [ebp-598h]
    float *v18; // [esp+80h] [ebp-594h]
    float *v19; // [esp+84h] [ebp-590h]
    float *v20; // [esp+88h] [ebp-58Ch]
    float *v21; // [esp+8Ch] [ebp-588h]
    float *v22; // [esp+90h] [ebp-584h]
    float *v23; // [esp+94h] [ebp-580h]
    float goalMat[4][4]; // [esp+98h] [ebp-57Ch] BYREF
    float v25; // [esp+E0h] [ebp-534h]
    float v26; // [esp+E4h] [ebp-530h]
    float v27; // [esp+E8h] [ebp-52Ch]
    float v28; // [esp+ECh] [ebp-528h]
    float v29; // [esp+F0h] [ebp-524h]
    float v30; // [esp+F4h] [ebp-520h]
    float v31; // [esp+F8h] [ebp-51Ch]
    float v32; // [esp+FCh] [ebp-518h]
    float v33; // [esp+100h] [ebp-514h]
    float v34; // [esp+104h] [ebp-510h]
    float v35; // [esp+108h] [ebp-50Ch]
    float v36; // [esp+10Ch] [ebp-508h]
    float v37; // [esp+110h] [ebp-504h]
    float v38; // [esp+114h] [ebp-500h]
    float v39; // [esp+118h] [ebp-4FCh]
    float v40; // [esp+11Ch] [ebp-4F8h]
    float *v41; // [esp+124h] [ebp-4F0h]
    float *v42; // [esp+128h] [ebp-4ECh]
    float *v43; // [esp+12Ch] [ebp-4E8h]
    __int64 v44; // [esp+130h] [ebp-4E4h]
    __int64 v45; // [esp+138h] [ebp-4DCh]
    __int64 v46; // [esp+140h] [ebp-4D4h]
    __int64 v47; // [esp+148h] [ebp-4CCh]
    __int64 v48; // [esp+150h] [ebp-4C4h]
    __int64 v49; // [esp+158h] [ebp-4BCh]
    __int64 v50; // [esp+160h] [ebp-4B4h]
    __int64 v51; // [esp+168h] [ebp-4ACh]
    float *v52; // [esp+170h] [ebp-4A4h]
    float *v53; // [esp+174h] [ebp-4A0h]
    float *v54; // [esp+178h] [ebp-49Ch]
    float *v55; // [esp+17Ch] [ebp-498h]
    __int64 v56; // [esp+180h] [ebp-494h]
    __int64 v57; // [esp+188h] [ebp-48Ch]
    __int64 v58; // [esp+190h] [ebp-484h]
    __int64 v59; // [esp+198h] [ebp-47Ch]
    __int64 v60; // [esp+1A0h] [ebp-474h]
    __int64 v61; // [esp+1A8h] [ebp-46Ch]
    __int64 v62; // [esp+1B0h] [ebp-464h]
    __int64 v63; // [esp+1B8h] [ebp-45Ch]
    float *v64; // [esp+1C4h] [ebp-450h]
    float *v65; // [esp+1C8h] [ebp-44Ch]
    float *v66; // [esp+1CCh] [ebp-448h]
    float *v67; // [esp+1D0h] [ebp-444h]
    float *v68; // [esp+1D4h] [ebp-440h]
    float handMat[4][4]; // [esp+1D8h] [ebp-43Ch] BYREF
    float *v70; // [esp+21Ch] [ebp-3F8h]
    float *v71; // [esp+220h] [ebp-3F4h]
    float *v72; // [esp+224h] [ebp-3F0h]
    __int64 v73; // [esp+228h] [ebp-3ECh]
    __int64 v74; // [esp+230h] [ebp-3E4h]
    __int64 v75; // [esp+238h] [ebp-3DCh]
    __int64 v76; // [esp+240h] [ebp-3D4h]
    __int64 v77; // [esp+248h] [ebp-3CCh]
    __int64 v78; // [esp+250h] [ebp-3C4h]
    __int64 v79; // [esp+258h] [ebp-3BCh]
    __int64 v80; // [esp+260h] [ebp-3B4h]
    float *v81; // [esp+268h] [ebp-3ACh]
    float *v82; // [esp+26Ch] [ebp-3A8h]
    float *v83; // [esp+270h] [ebp-3A4h]
    float *v84; // [esp+274h] [ebp-3A0h]
    float *v85; // [esp+278h] [ebp-39Ch]
    _DWORD v86[3]; // [esp+27Ch] [ebp-398h]
    __int64 v87; // [esp+288h] [ebp-38Ch]
    float v88; // [esp+290h] [ebp-384h]
    float v89; // [esp+294h] [ebp-380h]
    float v90; // [esp+298h] [ebp-37Ch]
    float v91; // [esp+29Ch] [ebp-378h]
    float v92; // [esp+2A0h] [ebp-374h]
    float v93; // [esp+2A4h] [ebp-370h]
    float v94; // [esp+2A8h] [ebp-36Ch]
    float v95; // [esp+2ACh] [ebp-368h]
    float v96; // [esp+2B0h] [ebp-364h]
    float v97; // [esp+2B4h] [ebp-360h]
    float v98; // [esp+2B8h] [ebp-35Ch]
    float v99; // [esp+2BCh] [ebp-358h]
    float *v100; // [esp+2C4h] [ebp-350h]
    float *v101; // [esp+2C8h] [ebp-34Ch]
    float *v102; // [esp+2CCh] [ebp-348h]
    __int64 v103; // [esp+2D0h] [ebp-344h]
    __int64 v104; // [esp+2D8h] [ebp-33Ch]
    __int64 v105; // [esp+2E0h] [ebp-334h]
    __int64 v106; // [esp+2E8h] [ebp-32Ch]
    __int64 v107; // [esp+2F0h] [ebp-324h]
    __int64 v108; // [esp+2F8h] [ebp-31Ch]
    __int64 v109; // [esp+300h] [ebp-314h]
    __int64 v110; // [esp+308h] [ebp-30Ch]
    float *v111; // [esp+310h] [ebp-304h]
    float *v112; // [esp+314h] [ebp-300h]
    float *v113; // [esp+318h] [ebp-2FCh]
    float *v114; // [esp+31Ch] [ebp-2F8h]
    __int64 v115; // [esp+320h] [ebp-2F4h]
    __int64 v116; // [esp+328h] [ebp-2ECh]
    __int64 v117; // [esp+330h] [ebp-2E4h]
    __int64 v118; // [esp+338h] [ebp-2DCh]
    __int64 v119; // [esp+340h] [ebp-2D4h]
    __int64 v120; // [esp+348h] [ebp-2CCh]
    __int64 v121; // [esp+350h] [ebp-2C4h]
    __int64 v122; // [esp+358h] [ebp-2BCh]
    float *v123; // [esp+360h] [ebp-2B4h]
    float *v124; // [esp+364h] [ebp-2B0h]
    float *v125; // [esp+368h] [ebp-2ACh]
    float *v126; // [esp+36Ch] [ebp-2A8h]
    float *v127; // [esp+370h] [ebp-2A4h]
    float *v128; // [esp+374h] [ebp-2A0h]
    float workMat[4][4]; // [esp+378h] [ebp-29Ch] BYREF
    float *v130; // [esp+3C4h] [ebp-250h]
    float *v131; // [esp+3C8h] [ebp-24Ch]
    float *v132; // [esp+3CCh] [ebp-248h]
    __int64 v133; // [esp+3D0h] [ebp-244h]
    __int64 v134; // [esp+3D8h] [ebp-23Ch]
    __int64 v135; // [esp+3E0h] [ebp-234h]
    __int64 v136; // [esp+3E8h] [ebp-22Ch]
    __int64 v137; // [esp+3F0h] [ebp-224h]
    __int64 v138; // [esp+3F8h] [ebp-21Ch]
    __int64 v139; // [esp+400h] [ebp-214h]
    __int64 v140; // [esp+408h] [ebp-20Ch]
    float *v141; // [esp+414h] [ebp-200h]
    float rotMat[4][4]; // [esp+418h] [ebp-1FCh] BYREF
    float axis[3][3]; // [esp+464h] [ebp-1B0h] BYREF
    float *v144; // [esp+488h] [ebp-18Ch]
    float *v145; // [esp+48Ch] [ebp-188h]
    float leftLerp; // [esp+490h] [ebp-184h]
    float gunPlaneOffset; // [esp+494h] [ebp-180h]
    float maxRange; // [esp+498h] [ebp-17Ch]
    float minRange; // [esp+49Ch] [ebp-178h]
    float gripPos[4]; // [esp+4A0h] [ebp-174h] BYREF
    float gripPosCenterEquipment[3]; // [esp+4B0h] [ebp-164h] BYREF
    float gripPosCenterShoulder[3]; // [esp+4BCh] [ebp-158h] BYREF
    float gripPosCenter[3]; // [esp+4C8h] [ebp-14Ch] BYREF
    float gripOffsetFromHandBone; // [esp+4D4h] [ebp-140h]
    float rangeShotgunAdd; // [esp+4D8h] [ebp-13Ch]
    float maxRangeBase; // [esp+4DCh] [ebp-138h]
    float minRangeBase; // [esp+4E0h] [ebp-134h]
    float v158[5]; // [esp+4E4h] [ebp-130h] BYREF
    float leftHandIKMat[4][4]; // [esp+4F8h] [ebp-11Ch] BYREF
    float v160[15]; // [esp+538h] [ebp-DCh] BYREF
    float *v161; // [esp+580h] [ebp-94h]
    float *leftHandRotation; // [esp+584h] [ebp-90h]
    float *leftHandOffset; // [esp+588h] [ebp-8Ch]
    float lerp; // [esp+58Ch] [ebp-88h]
    float fraction; // [esp+590h] [ebp-84h]
    float v166; // [esp+594h] [ebp-80h]
    float v167; // [esp+598h] [ebp-7Ch]
    int leftHandLerpTime; // [esp+59Ch] [ebp-78h]
    float leftHandRotationLocal[3]; // [esp+5A0h] [ebp-74h] BYREF
    float leftHandOffsetLocal[3]; // [esp+5ACh] [ebp-68h] BYREF
    float weapMat[4][4]; // [esp+5B8h] [ebp-5Ch] BYREF
    float layerLerp; // [esp+5FCh] [ebp-18h]
    int ikLayerName; // [esp+600h] [ebp-14h]
    //_UNKNOWN *v176[2]; // [esp+608h] [ebp-Ch] BYREF
    //int vars0; // [esp+614h] [ebp+0h]
    //
    //*(float *)v176 = a1;
    //v176[1] = (_UNKNOWN *)vars0;
    ikLayerName = 11;
    layerLerp = IKImport_GetLayerLerp(ikState, IKLAYER_LEFT_HAND_ON_GUN, 0.15f);
    ikState->leftHandLastProximityLerp = 0.0f;
    if (layerLerp > 0.0
        && ikState->ikBoneToObjBone[10] != 161
        && ikState->ikBoneToObjBone[22] != 161
        && ikState->bJointVarsValid
        && IK_IsCalcBone(ikState, 10))
    {
        ikCalcBoneModelMatrix(ikState, 22, (*ikState->matArrayPostIK)[0], weapMat);

        static bool bGetMat = true;
        if (bGetMat)
        {
            leftHandOffsetStatic[0] = 0.0f;
            leftHandOffsetStatic[1] = 0.0f;
            leftHandOffsetStatic[2] = 0.0f;
            bGetMat = 0;
        }

        static bool bHackUseDevGuiVars;
        if (IKImport_GetVar_IK_Hand_Tuning() || bHackUseDevGuiVars)
        {
            IKImport_GetVar_IK_Hand_Offset_Vec(leftHandOffsetStatic);
            IKImport_GetVar_IK_Hand_Rotation_Vec(leftHandRotationStatic);
            leftHandOffsetLocal[0] = leftHandOffsetStatic[0];
            leftHandOffsetLocal[1] = leftHandOffsetStatic[1];
            leftHandOffsetLocal[2] = leftHandOffsetStatic[2];
            leftHandRotationLocal[0] = leftHandRotationStatic[0];
            leftHandRotationLocal[1] = leftHandRotationStatic[1];
            leftHandRotationLocal[2] = leftHandRotationStatic[2];
        }
        else
        {
            leftHandLerpTime = 100;
            if (ikState->leftHandChangeTime <= ikState->timeMS - 100)
            {
                leftHandOffset = ikState->leftHandOffset;
                leftHandOffsetLocal[0] = ikState->leftHandOffset[0];
                leftHandOffsetLocal[1] = ikState->leftHandOffset[1];
                leftHandOffsetLocal[2] = ikState->leftHandOffset[2];
                leftHandRotation = ikState->leftHandRotation;
                leftHandRotationLocal[0] = ikState->leftHandRotation[0];
                leftHandRotationLocal[1] = ikState->leftHandRotation[1];
                leftHandRotationLocal[2] = ikState->leftHandRotation[2];
            }
            else
            {
                v167 = (float)(ikState->timeMS - ikState->leftHandChangeTime) / 100.0;
                if ((float)(v167 - 1.0) < 0.0)
                    v166 = v167;
                else
                    v166 = 1.0f;
                if ((float)(0.0 - v167) < 0.0)
                    fraction = v166;
                else
                    fraction = 0.0f;
                lerp = fraction;
                Vec3Lerp(ikState->leftHandOffsetPrev, ikState->leftHandOffset, fraction, leftHandOffsetLocal);
                Vec3Lerp(ikState->leftHandRotationPrev, ikState->leftHandRotation, fraction, leftHandRotationLocal);
            }
            leftHandOffsetStatic[0] = leftHandOffsetLocal[0];
            leftHandOffsetStatic[1] = leftHandOffsetLocal[1];
            leftHandOffsetStatic[2] = leftHandOffsetLocal[2];
            leftHandRotationStatic[0] = leftHandRotationLocal[0];
            leftHandRotationStatic[1] = leftHandRotationLocal[1];
            leftHandRotationStatic[2] = leftHandRotationLocal[2];
        }
        v161 = ikState->handMats[0][0];
        if (weapMat == (float (*)[4])v160
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
        {
            __debugbreak();
        }
        v160[0] = weapMat[0][0];
        v160[1] = weapMat[1][0];
        v160[2] = weapMat[2][0];
        v160[3] = 0.0f;
        v160[4] = weapMat[0][1];
        v160[5] = weapMat[1][1];
        v160[6] = weapMat[2][1];
        v160[7] = 0.0f;
        v160[8] = weapMat[0][2];
        v160[9] = weapMat[1][2];
        v160[10] = weapMat[2][2];
        v160[11] = 0.0f;
        leftHandIKMat[0][0] = (float)((float)(*v161 * weapMat[0][0]) + (float)(v161[1] * weapMat[0][1]))
            + (float)(v161[2] * weapMat[0][2]);
        leftHandIKMat[0][1] = (float)((float)(*v161 * weapMat[1][0]) + (float)(v161[1] * weapMat[1][1]))
            + (float)(v161[2] * weapMat[1][2]);
        leftHandIKMat[0][2] = (float)((float)(*v161 * weapMat[2][0]) + (float)(v161[1] * weapMat[2][1]))
            + (float)(v161[2] * weapMat[2][2]);
        leftHandIKMat[0][3] = 0.0f;
        leftHandIKMat[1][0] = (float)((float)(v161[4] * weapMat[0][0]) + (float)(v161[5] * weapMat[0][1]))
            + (float)(v161[6] * weapMat[0][2]);
        leftHandIKMat[1][1] = (float)((float)(v161[4] * weapMat[1][0]) + (float)(v161[5] * weapMat[1][1]))
            + (float)(v161[6] * weapMat[1][2]);
        leftHandIKMat[1][2] = (float)((float)(v161[4] * weapMat[2][0]) + (float)(v161[5] * weapMat[2][1]))
            + (float)(v161[6] * weapMat[2][2]);
        leftHandIKMat[1][3] = 0.0f;
        leftHandIKMat[2][0] = (float)((float)(v161[8] * weapMat[0][0]) + (float)(v161[9] * weapMat[0][1]))
            + (float)(v161[10] * weapMat[0][2]);
        leftHandIKMat[2][1] = (float)((float)(v161[8] * weapMat[1][0]) + (float)(v161[9] * weapMat[1][1]))
            + (float)(v161[10] * weapMat[1][2]);
        leftHandIKMat[2][2] = (float)((float)(v161[8] * weapMat[2][0]) + (float)(v161[9] * weapMat[2][1]))
            + (float)(v161[10] * weapMat[2][2]);
        leftHandIKMat[2][3] = 0.0f;
        //LODWORD(v158[4]) = weapMat[3];
        //LODWORD(v158[3]) = v161 + 12;
        v158[0] = v161[12] - weapMat[3][0];
        v158[1] = v161[13] - weapMat[3][1];
        v158[2] = v161[14] - weapMat[3][2];
        ikMatrixTransformVector34(v158, (const float (*)[4])v160, leftHandIKMat[3]);
        leftHandIKMat[3][3] = 1.0f;
        minRangeBase = 3.0f;
        maxRangeBase = 8.0f;
        rangeShotgunAdd = 5.0f;
        gripOffsetFromHandBone = 2.3f;
        gripPosCenter[0] = -4.0f;
        gripPosCenter[1] = 0.1f;
        gripPosCenter[2] = -3.4f;
        gripPosCenterShoulder[0] = -18.0f;
        gripPosCenterShoulder[1] = 1.6f;
        gripPosCenterShoulder[2] = 0.0f;
        gripPosCenterEquipment[0] = 1.0f;
        gripPosCenterEquipment[1] = 7.0f;
        gripPosCenterEquipment[2] = 0.4f;
        //LODWORD(gripPos[3]) = leftHandIKMat[3];
        gripPos[0] = (float)(2.3 * leftHandIKMat[0][0]) + leftHandIKMat[3][0];
        gripPos[1] = (float)(2.3 * leftHandIKMat[0][1]) + leftHandIKMat[3][1];
        gripPos[2] = (float)(2.3 * leftHandIKMat[0][2]) + leftHandIKMat[3][2];
        minRange = IKImport_GetLayerLerp(ikState, IKLAYER_SHOTGUN_ACTIVE) * 5.0 + 3.0;
        maxRange = IKImport_GetLayerLerp(ikState, IKLAYER_SHOTGUN_ACTIVE) * 5.0 + 8.0;
        if (ikState->weaponOffsetType == IKWEAPOFS_SHOULDER)
        {
            gunPlaneOffset = Vec3Distance(gripPos, gripPosCenterShoulder);
        }
        else if (ikState->weaponOffsetType == IKWEAPOFS_GRENADE)
        {
            gunPlaneOffset = Vec3Distance(gripPos, gripPosCenterEquipment);
        }
        else
        {
            gunPlaneOffset = Vec3Distance(gripPos, gripPosCenter);
        }
        if (minRange <= gunPlaneOffset)
        {
            if (maxRange <= gunPlaneOffset)
                leftLerp = 0.0f;
            else
                leftLerp = 1.0 - (float)((float)(gunPlaneOffset - minRange) / (float)(maxRange - minRange));
        }
        else
        {
            leftLerp = 1.0f;
        }
        if (!ikSystem.inViewer)
            layerLerp = layerLerp * leftLerp;
        ikState->leftHandLastProximityLerp = leftLerp;
        if (layerLerp >= 0.0099999998)
        {
            v145 = leftHandIKMat[3];
            v144 = leftHandIKMat[3];
            leftHandIKMat[3][0] = leftHandIKMat[3][0] + leftHandOffsetLocal[0];
            leftHandIKMat[3][1] = leftHandIKMat[3][1] + leftHandOffsetLocal[1];
            leftHandIKMat[3][2] = leftHandIKMat[3][2] + leftHandOffsetLocal[2];
            AnglesToAxis(leftHandRotationLocal, axis);
            ikMatrixSet44(rotMat, vec3_origin, axis, 1.0);
            v141 = leftHandIKMat[0];
            v133 = *(_QWORD *)&leftHandIKMat[0][0];
            v134 = *(_QWORD *)&leftHandIKMat[0][2];
            v132 = leftHandIKMat[1];
            v135 = *(_QWORD *)&leftHandIKMat[1][0];
            v136 = *(_QWORD *)&leftHandIKMat[1][2];
            v131 = leftHandIKMat[2];
            v137 = *(_QWORD *)&leftHandIKMat[2][0];
            v138 = *(_QWORD *)&leftHandIKMat[2][2];
            v130 = leftHandIKMat[3];
            v139 = *(_QWORD *)&leftHandIKMat[3][0];
            v140 = *(_QWORD *)&leftHandIKMat[3][2];
            v128 = workMat[0];
            *(_QWORD *)&workMat[0][0] = *(_QWORD *)&leftHandIKMat[0][0];
            *(_QWORD *)&workMat[0][2] = *(_QWORD *)&leftHandIKMat[0][2];
            v127 = workMat[1];
            *(_QWORD *)&workMat[1][0] = *(_QWORD *)&leftHandIKMat[1][0];
            *(_QWORD *)&workMat[1][2] = *(_QWORD *)&leftHandIKMat[1][2];
            v126 = workMat[2];
            *(_QWORD *)&workMat[2][0] = *(_QWORD *)&leftHandIKMat[2][0];
            *(_QWORD *)&workMat[2][2] = *(_QWORD *)&leftHandIKMat[2][2];
            v125 = workMat[3];
            *(_QWORD *)&workMat[3][0] = *(_QWORD *)&leftHandIKMat[3][0];
            *(_QWORD *)&workMat[3][2] = *(_QWORD *)&leftHandIKMat[3][2];
            leftHandIKMat[0][0] = (float)((float)(leftHandIKMat[0][0] * rotMat[0][0])
                + (float)(leftHandIKMat[0][1] * rotMat[1][0]))
                + (float)(leftHandIKMat[0][2] * rotMat[2][0]);
            leftHandIKMat[0][1] = (float)((float)(workMat[0][0] * rotMat[0][1]) + (float)(leftHandIKMat[0][1] * rotMat[1][1]))
                + (float)(leftHandIKMat[0][2] * rotMat[2][1]);
            leftHandIKMat[0][2] = (float)((float)(workMat[0][0] * rotMat[0][2]) + (float)(workMat[0][1] * rotMat[1][2]))
                + (float)(leftHandIKMat[0][2] * rotMat[2][2]);
            leftHandIKMat[0][3] = 0.0f;
            leftHandIKMat[1][0] = (float)((float)(leftHandIKMat[1][0] * rotMat[0][0])
                + (float)(leftHandIKMat[1][1] * rotMat[1][0]))
                + (float)(leftHandIKMat[1][2] * rotMat[2][0]);
            leftHandIKMat[1][1] = (float)((float)(workMat[1][0] * rotMat[0][1]) + (float)(leftHandIKMat[1][1] * rotMat[1][1]))
                + (float)(leftHandIKMat[1][2] * rotMat[2][1]);
            leftHandIKMat[1][2] = (float)((float)(workMat[1][0] * rotMat[0][2]) + (float)(workMat[1][1] * rotMat[1][2]))
                + (float)(leftHandIKMat[1][2] * rotMat[2][2]);
            leftHandIKMat[1][3] = 0.0f;
            leftHandIKMat[2][0] = (float)((float)(leftHandIKMat[2][0] * rotMat[0][0])
                + (float)(leftHandIKMat[2][1] * rotMat[1][0]))
                + (float)(leftHandIKMat[2][2] * rotMat[2][0]);
            leftHandIKMat[2][1] = (float)((float)(workMat[2][0] * rotMat[0][1]) + (float)(leftHandIKMat[2][1] * rotMat[1][1]))
                + (float)(leftHandIKMat[2][2] * rotMat[2][1]);
            leftHandIKMat[2][2] = (float)((float)(workMat[2][0] * rotMat[0][2]) + (float)(workMat[2][1] * rotMat[1][2]))
                + (float)(leftHandIKMat[2][2] * rotMat[2][2]);
            leftHandIKMat[2][3] = 0.0f;
            if (layerLerp == 1.0)
            {
                v124 = ikState->handMats[0][0];
                v123 = leftHandIKMat[0];
                v115 = *(_QWORD *)&leftHandIKMat[0][0];
                v116 = *(_QWORD *)&leftHandIKMat[0][2];
                v114 = leftHandIKMat[1];
                v117 = *(_QWORD *)&leftHandIKMat[1][0];
                v118 = *(_QWORD *)&leftHandIKMat[1][2];
                v113 = leftHandIKMat[2];
                v119 = *(_QWORD *)&leftHandIKMat[2][0];
                v120 = *(_QWORD *)&leftHandIKMat[2][2];
                v112 = leftHandIKMat[3];
                v121 = *(_QWORD *)&leftHandIKMat[3][0];
                v122 = *(_QWORD *)&leftHandIKMat[3][2];
                v111 = weapMat[0];
                v103 = *(_QWORD *)&weapMat[0][0];
                v104 = *(_QWORD *)&weapMat[0][2];
                v102 = weapMat[1];
                v105 = *(_QWORD *)&weapMat[1][0];
                v106 = *(_QWORD *)&weapMat[1][2];
                v101 = weapMat[2];
                v107 = *(_QWORD *)&weapMat[2][0];
                v108 = *(_QWORD *)&weapMat[2][2];
                v100 = weapMat[3];
                v109 = *(_QWORD *)&weapMat[3][0];
                v110 = *(_QWORD *)&weapMat[3][2];
                *(float *)&v86[1] = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][0])
                    + (float)(leftHandIKMat[0][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][0]);
                *(float *)&v86[2] = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][1])
                    + (float)(leftHandIKMat[0][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][1]);
                *(float *)&v87 = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][2])
                    + (float)(leftHandIKMat[0][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][2]);
                *((float *)&v87 + 1) = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][3])
                    + (float)(leftHandIKMat[0][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][3]);
                v88 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][0]) + (float)(leftHandIKMat[1][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][0]);
                v89 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][1]) + (float)(leftHandIKMat[1][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][1]);
                v90 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][2]) + (float)(leftHandIKMat[1][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][2]);
                v91 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][3]) + (float)(leftHandIKMat[1][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][3]);
                v92 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][0]) + (float)(leftHandIKMat[2][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][0]);
                v93 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][1]) + (float)(leftHandIKMat[2][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][1]);
                v94 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][2]) + (float)(leftHandIKMat[2][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][2]);
                v95 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][3]) + (float)(leftHandIKMat[2][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][3]);
                v96 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][0]) + (float)(leftHandIKMat[3][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][0]);
                v97 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][1]) + (float)(leftHandIKMat[3][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][1]);
                v98 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][2]) + (float)(leftHandIKMat[3][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][2]);
                v99 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][3]) + (float)(leftHandIKMat[3][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][3]);
                //v86[0] = ikState->handMats;
                *(_QWORD *)&ikState->handMats[0][0][0] = *(_QWORD *)&v86[1];
                *(_QWORD *)&ikState->handMats[0][0][2] = v87;
                v85 = v124 + 4;
                v2 = v124 + 4;
                v124[4] = v88;
                v2[1] = v89;
                v2[2] = v90;
                v2[3] = v91;
                v84 = v124 + 8;
                v3 = v124 + 8;
                v124[8] = v92;
                v3[1] = v93;
                v3[2] = v94;
                v3[3] = v95;
                v83 = v124 + 12;
                v4 = v124 + 12;
                v124[12] = v96;
                v4[1] = v97;
                v4[2] = v98;
                v4[3] = v99;
            }
            else
            {
                v82 = ikState->handMats[0][0];
                v81 = ikState->handMats[0][0];
                v73 = *(_QWORD *)&ikState->handMats[0][0][0];
                v74 = *(_QWORD *)&ikState->handMats[0][0][2];
                v72 = ikState->handMats[0][1];
                v75 = *(_QWORD *)&ikState->handMats[0][1][0];
                v76 = *(_QWORD *)&ikState->handMats[0][1][2];
                v71 = ikState->handMats[0][2];
                v77 = *(_QWORD *)&ikState->handMats[0][2][0];
                v78 = *(_QWORD *)&ikState->handMats[0][2][2];
                v70 = ikState->handMats[0][3];
                v79 = *(_QWORD *)&ikState->handMats[0][3][0];
                v80 = *(_QWORD *)&ikState->handMats[0][3][2];
                v68 = handMat[0];
                *(_QWORD *)&handMat[0][0] = v73;
                *(_QWORD *)&handMat[0][2] = v74;
                v67 = handMat[1];
                *(_QWORD *)&handMat[1][0] = v75;
                *(_QWORD *)&handMat[1][2] = v76;
                v66 = handMat[2];
                *(_QWORD *)&handMat[2][0] = v77;
                *(_QWORD *)&handMat[2][2] = v78;
                v65 = handMat[3];
                *(_QWORD *)&handMat[3][0] = v79;
                *(_QWORD *)&handMat[3][2] = v80;
                v64 = leftHandIKMat[0];
                v56 = *(_QWORD *)&leftHandIKMat[0][0];
                v57 = *(_QWORD *)&leftHandIKMat[0][2];
                v55 = leftHandIKMat[1];
                v58 = *(_QWORD *)&leftHandIKMat[1][0];
                v59 = *(_QWORD *)&leftHandIKMat[1][2];
                v54 = leftHandIKMat[2];
                v60 = *(_QWORD *)&leftHandIKMat[2][0];
                v61 = *(_QWORD *)&leftHandIKMat[2][2];
                v53 = leftHandIKMat[3];
                v62 = *(_QWORD *)&leftHandIKMat[3][0];
                v63 = *(_QWORD *)&leftHandIKMat[3][2];
                v52 = weapMat[0];
                v44 = *(_QWORD *)&weapMat[0][0];
                v45 = *(_QWORD *)&weapMat[0][2];
                v43 = weapMat[1];
                v46 = *(_QWORD *)&weapMat[1][0];
                v47 = *(_QWORD *)&weapMat[1][2];
                v42 = weapMat[2];
                v48 = *(_QWORD *)&weapMat[2][0];
                v49 = *(_QWORD *)&weapMat[2][2];
                v41 = weapMat[3];
                v50 = *(_QWORD *)&weapMat[3][0];
                v51 = *(_QWORD *)&weapMat[3][2];
                v25 = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][0]) + (float)(leftHandIKMat[0][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][0]);
                v26 = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][1]) + (float)(leftHandIKMat[0][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][1]);
                v27 = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][2]) + (float)(leftHandIKMat[0][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][2]);
                v28 = (float)((float)((float)(leftHandIKMat[0][0] * weapMat[0][3]) + (float)(leftHandIKMat[0][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[0][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[0][3] * weapMat[3][3]);
                v29 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][0]) + (float)(leftHandIKMat[1][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][0]);
                v30 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][1]) + (float)(leftHandIKMat[1][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][1]);
                v31 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][2]) + (float)(leftHandIKMat[1][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][2]);
                v32 = (float)((float)((float)(leftHandIKMat[1][0] * weapMat[0][3]) + (float)(leftHandIKMat[1][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[1][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[1][3] * weapMat[3][3]);
                v33 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][0]) + (float)(leftHandIKMat[2][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][0]);
                v34 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][1]) + (float)(leftHandIKMat[2][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][1]);
                v35 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][2]) + (float)(leftHandIKMat[2][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][2]);
                v36 = (float)((float)((float)(leftHandIKMat[2][0] * weapMat[0][3]) + (float)(leftHandIKMat[2][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[2][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[2][3] * weapMat[3][3]);
                v37 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][0]) + (float)(leftHandIKMat[3][1] * weapMat[1][0]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][0]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][0]);
                v38 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][1]) + (float)(leftHandIKMat[3][1] * weapMat[1][1]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][1]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][1]);
                v39 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][2]) + (float)(leftHandIKMat[3][1] * weapMat[1][2]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][2]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][2]);
                v40 = (float)((float)((float)(leftHandIKMat[3][0] * weapMat[0][3]) + (float)(leftHandIKMat[3][1] * weapMat[1][3]))
                    + (float)(leftHandIKMat[3][2] * weapMat[2][3]))
                    + (float)(leftHandIKMat[3][3] * weapMat[3][3]);
                v23 = goalMat[0];
                goalMat[0][0] = v25;
                goalMat[0][1] = v26;
                goalMat[0][2] = v27;
                goalMat[0][3] = v28;
                v22 = goalMat[1];
                goalMat[1][0] = v29;
                goalMat[1][1] = v30;
                goalMat[1][2] = v31;
                goalMat[1][3] = v32;
                v21 = goalMat[2];
                goalMat[2][0] = v33;
                goalMat[2][1] = v34;
                goalMat[2][2] = v35;
                goalMat[2][3] = v36;
                v20 = goalMat[3];
                goalMat[3][0] = v37;
                goalMat[3][1] = v38;
                goalMat[3][2] = v39;
                goalMat[3][3] = v40;
                ikMatrixLerp44(handMat, goalMat, layerLerp, ikState->handMats[0]);
            }
            v19 = ikState->leftHandFromRightHandMat[0];
            v18 = ikState->handMats[1][0];
            v17 = ikState->handMats[0][0];
            if ((float *)ikState->handMats[1] == v8
                && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
            {
                __debugbreak();
            }
            v8[0] = *v18;
            v8[1] = v18[4];
            v8[2] = v18[8];
            v8[3] = 0.0f;
            v9 = v18[1];
            v10 = v18[5];
            v11 = v18[9];
            v12 = 0.0f;
            v13 = v18[2];
            v14 = v18[6];
            v15 = v18[10];
            v16 = 0.0f;
            *v19 = (float)((float)(*v17 * v8[0]) + (float)(v17[1] * v9)) + (float)(v17[2] * v13);
            v19[1] = (float)((float)(*v17 * v8[1]) + (float)(v17[1] * v10)) + (float)(v17[2] * v14);
            v19[2] = (float)((float)(*v17 * v8[2]) + (float)(v17[1] * v11)) + (float)(v17[2] * v15);
            v19[3] = 0.0f;
            v19[4] = (float)((float)(v17[4] * v8[0]) + (float)(v17[5] * v9)) + (float)(v17[6] * v13);
            v19[5] = (float)((float)(v17[4] * v8[1]) + (float)(v17[5] * v10)) + (float)(v17[6] * v14);
            v19[6] = (float)((float)(v17[4] * v8[2]) + (float)(v17[5] * v11)) + (float)(v17[6] * v15);
            v19[7] = 0.0f;
            v19[8] = (float)((float)(v17[8] * v8[0]) + (float)(v17[9] * v9)) + (float)(v17[10] * v13);
            v19[9] = (float)((float)(v17[8] * v8[1]) + (float)(v17[9] * v10)) + (float)(v17[10] * v14);
            v19[10] = (float)((float)(v17[8] * v8[2]) + (float)(v17[9] * v11)) + (float)(v17[10] * v15);
            v19[11] = 0.0f;
            //LODWORD(v7[4]) = v18 + 12;
            //LODWORD(v7[3]) = v17 + 12;
            v7[0] = v17[12] - v18[12];
            v7[1] = v17[13] - v18[13];
            v7[2] = v17[14] - v18[14];
            ikMatrixTransformVector34(v7, (const float (*)[4])v8, v19 + 12);
            v19[15] = 1.0f;
            twistScale = -0.89999998;
            twistRot[0] = 0.0f;
            twistRot[1] = 0.0f;
            twistRot[2] = (float)(layerLerp * -0.89999998) * leftHandRotationLocal[0];
            ikRotateBone(ikState, IKBONE_LWRIST_TWIST, twistRot, 1);
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
    layerLerp = IKImport_GetLayerLerp(ikState, IKLAYER_PLAYER_CONTROLLERS, 0.0f);
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
                ikRotateBone(ikState, ikControllerBones[i], rot, 0);
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
                    ikRotateBone(ikState, IKBONE_SPINE, rot_s, 0);
                    if ( IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE) < 1.0 )
                    {
                        v5 = 1.0 - IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
                        rot_s[0] = v5 * rot[0];
                        rot_s[1] = v5 * rot[1];
                        rot_s[2] = v5 * rot[2];
                        ikRotateBone(ikState, IKBONE_PELVIS, rot_s, 0);
                    }
                }
                else
                {
                    ikRotateBone(ikState, ikControllerBones[j], rot, 0);
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
            ikRotateBone(ikState, IKBONE_LFOOT, rot, 0);
            v3 = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
            rot[1] = (float)((float)((float)(1.0 - v3) * pelvisYawFeetScaleRight[0]) + (float)(pelvisYawFeetScaleRight[1] * v3))
                         * ikState->controllers[5][1];
            ikRotateBone(ikState, IKBONE_RFOOT, rot, 0);
        }
    }
}

// local variable allocation has failed, the output may be wrong!
void    IK_Layer_PlayerPitch(IKState *ikState, bool preControllers)
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
    float *out; // [esp+74h] [ebp-480h]
    float *v33; // [esp+78h] [ebp-47Ch]
    float *v34; // [esp+7Ch] [ebp-478h]
    float *v35; // [esp+80h] [ebp-474h]
    float *v36; // [esp+84h] [ebp-470h]
    float *v37; // [esp+88h] [ebp-46Ch]
    float *v38; // [esp+8Ch] [ebp-468h]
    float *v39; // [esp+90h] [ebp-464h]
    _DWORD v40[3]; // [esp+94h] [ebp-460h]
    __int64 v41; // [esp+A0h] [ebp-454h]
    float v42; // [esp+A8h] [ebp-44Ch]
    float v43; // [esp+ACh] [ebp-448h]
    float v44; // [esp+B0h] [ebp-444h]
    float v45; // [esp+B4h] [ebp-440h]
    float v46; // [esp+B8h] [ebp-43Ch]
    float v47; // [esp+BCh] [ebp-438h]
    float v48; // [esp+C0h] [ebp-434h]
    float v49; // [esp+C4h] [ebp-430h]
    float v50; // [esp+C8h] [ebp-42Ch]
    float v51; // [esp+CCh] [ebp-428h]
    float v52; // [esp+D0h] [ebp-424h]
    float v53; // [esp+D4h] [ebp-420h]
    float *v54; // [esp+DCh] [ebp-418h]
    float *v55; // [esp+E0h] [ebp-414h]
    float *v56; // [esp+E4h] [ebp-410h]
    float v57; // [esp+E8h] [ebp-40Ch]
    __int64 v58; // [esp+ECh] [ebp-408h]
    float v59; // [esp+F4h] [ebp-400h]
    float v60; // [esp+F8h] [ebp-3FCh]
    __int64 v61; // [esp+FCh] [ebp-3F8h]
    float v62; // [esp+104h] [ebp-3F0h]
    float v63; // [esp+108h] [ebp-3ECh]
    __int64 v64; // [esp+10Ch] [ebp-3E8h]
    float v65; // [esp+114h] [ebp-3E0h]
    __int64 v66; // [esp+118h] [ebp-3DCh]
    __int64 v67; // [esp+120h] [ebp-3D4h]
    float *v68; // [esp+128h] [ebp-3CCh]
    float *v69; // [esp+12Ch] [ebp-3C8h]
    float *v70; // [esp+130h] [ebp-3C4h]
    float *v71; // [esp+134h] [ebp-3C0h]
    float v72; // [esp+138h] [ebp-3BCh]
    __int64 v73; // [esp+13Ch] [ebp-3B8h]
    float v74; // [esp+144h] [ebp-3B0h]
    float v75; // [esp+148h] [ebp-3ACh]
    __int64 v76; // [esp+14Ch] [ebp-3A8h]
    float v77; // [esp+154h] [ebp-3A0h]
    float v78; // [esp+158h] [ebp-39Ch]
    __int64 v79; // [esp+15Ch] [ebp-398h]
    float v80; // [esp+164h] [ebp-390h]
    __int64 v81; // [esp+168h] [ebp-38Ch]
    __int64 v82; // [esp+170h] [ebp-384h]
    float *v83; // [esp+178h] [ebp-37Ch]
    float *v84; // [esp+17Ch] [ebp-378h]
    float *v85; // [esp+180h] [ebp-374h]
    float *v86; // [esp+184h] [ebp-370h]
    float *v87; // [esp+188h] [ebp-36Ch]
    float *v88; // [esp+18Ch] [ebp-368h]
    float *v89; // [esp+190h] [ebp-364h]
    _DWORD v90[3]; // [esp+194h] [ebp-360h]
    __int64 v91; // [esp+1A0h] [ebp-354h]
    float v92; // [esp+1A8h] [ebp-34Ch]
    float v93; // [esp+1ACh] [ebp-348h]
    float v94; // [esp+1B0h] [ebp-344h]
    float v95; // [esp+1B4h] [ebp-340h]
    float v96; // [esp+1B8h] [ebp-33Ch]
    float v97; // [esp+1BCh] [ebp-338h]
    float v98; // [esp+1C0h] [ebp-334h]
    float v99; // [esp+1C4h] [ebp-330h]
    float v100; // [esp+1C8h] [ebp-32Ch]
    float v101; // [esp+1CCh] [ebp-328h]
    float v102; // [esp+1D0h] [ebp-324h]
    float v103; // [esp+1D4h] [ebp-320h]
    float *v104; // [esp+1DCh] [ebp-318h]
    float *v105; // [esp+1E0h] [ebp-314h]
    float *v106; // [esp+1E4h] [ebp-310h]
    __int64 v107; // [esp+1E8h] [ebp-30Ch]
    __int64 v108; // [esp+1F0h] [ebp-304h]
    __int64 v109; // [esp+1F8h] [ebp-2FCh]
    __int64 v110; // [esp+200h] [ebp-2F4h]
    __int64 v111; // [esp+208h] [ebp-2ECh]
    __int64 v112; // [esp+210h] [ebp-2E4h]
    __int64 v113; // [esp+218h] [ebp-2DCh]
    __int64 v114; // [esp+220h] [ebp-2D4h]
    float *v115; // [esp+228h] [ebp-2CCh]
    float *v116; // [esp+22Ch] [ebp-2C8h]
    __int64 *v117; // [esp+230h] [ebp-2C4h]
    __int64 *v118; // [esp+234h] [ebp-2C0h]
    __int64 v119; // [esp+238h] [ebp-2BCh]
    __int64 v120; // [esp+240h] [ebp-2B4h]
    __int64 v121; // [esp+248h] [ebp-2ACh]
    __int64 v122; // [esp+250h] [ebp-2A4h]
    __int64 v123; // [esp+258h] [ebp-29Ch]
    __int64 v124; // [esp+260h] [ebp-294h]
    float v125; // [esp+268h] [ebp-28Ch]
    float v126; // [esp+26Ch] [ebp-288h]
    float v127; // [esp+270h] [ebp-284h]
    float v128; // [esp+274h] [ebp-280h]
    float *v129; // [esp+278h] [ebp-27Ch]
    float *v130; // [esp+27Ch] [ebp-278h]
    float v131; // [esp+280h] [ebp-274h]
    float headPitchFrac; // [esp+284h] [ebp-270h]
    float v133; // [esp+288h] [ebp-26Ch]
    float neckPitchFrac; // [esp+28Ch] [ebp-268h]
    float headAndNeckPitchScale; // [esp+290h] [ebp-264h]
    float v136[5]; // [esp+294h] [ebp-260h] BYREF
    float rightHandFromHead[2]; // [esp+2A8h] [ebp-24Ch] BYREF
    __int64 v138; // [esp+2B0h] [ebp-244h]
    __int64 v139; // [esp+2B8h] [ebp-23Ch] BYREF
    __int64 v140; // [esp+2C0h] [ebp-234h]
    __int64 v141; // [esp+2C8h] [ebp-22Ch] BYREF
    __int64 v142; // [esp+2D0h] [ebp-224h]
    float v143; // [esp+2D8h] [ebp-21Ch] BYREF
    float v144; // [esp+2DCh] [ebp-218h]
    float v145; // [esp+2E0h] [ebp-214h]
    float v146; // [esp+2E4h] [ebp-210h]
    float v147[16]; // [esp+2E8h] [ebp-20Ch] BYREF
    float *v148; // [esp+334h] [ebp-1C0h]
    float headMat[4][4]; // [esp+338h] [ebp-1BCh] BYREF
    float *v150; // [esp+384h] [ebp-170h]
    float *v151; // [esp+388h] [ebp-16Ch]
    float *v152; // [esp+38Ch] [ebp-168h]
    float *v153; // [esp+390h] [ebp-164h]
    float weapOfs[3]; // [esp+394h] [ebp-160h] BYREF
    bool bIsPitchDirectionUp; // [esp+3A3h] [ebp-151h]
    BOOL v156; // [esp+3A4h] [ebp-150h]
    float v157; // [esp+3A8h] [ebp-14Ch]
    int v158; // [esp+3ACh] [ebp-148h]
    float v159; // [esp+3B0h] [ebp-144h]
    float fPitchPow; // [esp+3B4h] [ebp-140h]
    float rot[4]; // [esp+3B8h] [ebp-13Ch] BYREF
    float pelvisPitchScale; // [esp+3C8h] [ebp-12Ch]
    float v163; // [esp+3CCh] [ebp-128h]
    float pelvisPitchScaleDn; // [esp+3D0h] [ebp-124h]
    float pelvisPitchScaleUp; // [esp+3D4h] [ebp-120h]
    float *v166; // [esp+3D8h] [ebp-11Ch]
    float *v167; // [esp+3DCh] [ebp-118h]
    float *v168; // [esp+3E0h] [ebp-114h]
    float *v169; // [esp+3E4h] [ebp-110h]
    float footVec[3]; // [esp+3E8h] [ebp-10Ch]
    float *v171; // [esp+3F4h] [ebp-100h]
    float *v172; // [esp+3F8h] [ebp-FCh]
    float ofsTwistVec[5]; // [esp+3FCh] [ebp-F8h] BYREF
    float v174; // [esp+410h] [ebp-E4h]
    float twistScaleCap; // [esp+414h] [ebp-E0h]
    float maxTwist; // [esp+418h] [ebp-DCh]
    float ofsTwistCrouch[3]; // [esp+41Ch] [ebp-D8h] BYREF
    float ofsTwist[3]; // [esp+428h] [ebp-CCh] BYREF
    float ofsDnCrouch[3]; // [esp+434h] [ebp-C0h] BYREF
    float ofsDn[3]; // [esp+440h] [ebp-B4h] BYREF
    float v181; // [esp+44Ch] [ebp-A8h]
    float ofsUpCrouch[3]; // [esp+450h] [ebp-A4h] BYREF
    float ofsUp[3]; // [esp+45Ch] [ebp-98h] BYREF
    float fScale[3]; // [esp+468h] [ebp-8Ch]
    float axis[3][3]; // [esp+474h] [ebp-80h] BYREF
    float ofsVec[3]; // [esp+498h] [ebp-5Ch] BYREF
    float notMovingLerp; // [esp+4A4h] [ebp-50h]
    float v188; // [esp+4A8h] [ebp-4Ch]
    float v189; // [esp+4ACh] [ebp-48h]
    float v190; // [esp+4B0h] [ebp-44h]
    float movingLerp; // [esp+4B4h] [ebp-40h]
    float v192; // [esp+4B8h] [ebp-3Ch]
    float v193; // [esp+4BCh] [ebp-38h]
    float v194; // [esp+4C0h] [ebp-34h]
    float xySpeed; // [esp+4C4h] [ebp-30h]
    float crouchLerp; // [esp+4C8h] [ebp-2Ch]
    float playerAngles[4]; // [esp+4CCh] [ebp-28h] BYREF
    float layerLerp; // [esp+4DCh] [ebp-18h]
    int ikLayerName; // [esp+4E0h] [ebp-14h]
    //_UNKNOWN *v200[2]; // [esp+4E8h] [ebp-Ch] BYREF
    //int vars0; // [esp+4F4h] [ebp+0h]
    //
    //*(float *)v200 = a1;
    //v200[1] = (_UNKNOWN *)vars0;
    ikLayerName = 13;
    layerLerp = IKImport_GetLayerLerp(ikState, IKLAYER_PLAYER_PITCH, 0.3f);
    if (layerLerp > 0.0)
    {
        //LODWORD(playerAngles[3]) = ikState->angles;
        playerAngles[0] = ikState->angles[0];
        playerAngles[1] = 0.0f;
        playerAngles[2] = 0.0f;
        crouchLerp = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
        xySpeed = Vec2Length(ikState->velocity);
        v194 = xySpeed / velIdleMax;
        if ((float)((float)(xySpeed / velIdleMax) - 1.0) < 0.0)
            v193 = v194;
        else
            v193 = 1.0f;
        if ((float)(0.0 - v194) < 0.0)
            v192 = v193;
        else
            v192 = 0.0f;
        movingLerp = v192;
        v190 = IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING);
        v189 = IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING_FAST);
        if ((float)(v190 - v189) < 0.0)
            v3 = v189;
        else
            v3 = v190;
        v188 = v3;
        notMovingLerp = v3;
        memset(ofsVec, 0, sizeof(ofsVec));
        AnglesToAxis(playerAngles, axis);
        if (!preControllers)
        {
            fScale[2] = playerAngles[0];
            fScale[1] = fabs(playerAngles[0]);
            v4 = (float)((fabs(playerAngles[0])) / 90.0);
            //__libm_sse2_pow(v14, v16);
            //*(float *)&v4 = v4;
            v4 = pow(v4, 1.3f);
            fScale[0] = *(float *)&v4 * layerLerp;
            if (playerAngles[0] >= 0.0)
            {
                ofsDn[0] = -4.5f;
                ofsDn[1] = 0.0f;
                ofsDn[2] = -1.7;
                ofsDnCrouch[0] = -6.5f;
                ofsDnCrouch[1] = 0.0f;
                ofsDnCrouch[2] = 8.5f;
                Vec3Lerp(ofsDn, ofsDnCrouch, crouchLerp, ofsVec);
            }
            else
            {
                ofsUp[0] = 5.1f;
                ofsUp[1] = 0.0f;
                ofsUp[2] = -2.5f;
                ofsUpCrouch[0] = 5.1f;
                ofsUpCrouch[1] = 0.0f;
                ofsUpCrouch[2] = 4.5f;
                Vec3Lerp(ofsUp, ofsUpCrouch, crouchLerp, ofsVec);
                v181 = 1.0 - IKImport_GetLayerLerp(ikState, IKLAYER_MINIGUN_ACTIVE);
                ofsVec[0] = v181 * ofsVec[0];
                ofsVec[1] = v181 * ofsVec[1];
                ofsVec[2] = v181 * ofsVec[2];
            }
            ofsVec[2] = ofsVec[2] * notMovingLerp;
            ofsVec[0] = fScale[0] * ofsVec[0];
            ofsVec[1] = fScale[0] * ofsVec[1];
            ofsVec[2] = fScale[0] * ofsVec[2];
            ofsTwist[0] = 6.0f;
            ofsTwist[1] = 0.0f;
            ofsTwist[2] = -1.0f;
            ofsTwistCrouch[0] = 4.0f;
            ofsTwistCrouch[1] = 0.0f;
            ofsTwistCrouch[2] = -1.0f;
            maxTwist = 90.0f;
            twistScaleCap = 1.0f;
            v174 = ikState->controller_origin_angles[1] / 90.0;
            //ofsTwistVec[4] = fabs(v174);
            //__libm_sse2_pow(v15, v16);
            fScale[0] = pow(fabs(v174), 1.3f);
            //fScale[0] = fabs(v174);
            if ((float)(1.0 - (fabs(v174))) < 0.0)
                v5 = twistScaleCap;
            else
                v5 = fScale[0];
            ofsTwistVec[3] = v5;
            fScale[0] = v5;
            Vec3Lerp(ofsTwist, ofsTwistCrouch, crouchLerp, ofsTwistVec);
            ofsVec[0] = (float)(fScale[0] * ofsTwistVec[0]) + ofsVec[0];
            ofsVec[1] = (float)(fScale[0] * ofsTwistVec[1]) + ofsVec[1];
            ofsVec[2] = (float)(fScale[0] * ofsTwistVec[2]) + ofsVec[2];
            v172 = (*ikState->matArrayPostIK)[7];
            v171 = (*ikState->matArrayPostIK)[7];
            *v172 = *v171 + ofsVec[0];
            v172[1] = v171[1] + ofsVec[1];
            v172[2] = v171[2] + ofsVec[2];
            ikState->bHasActiveLayers = 1;
            if (ikState->cacheActive)
            {
                memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                ikState->cacheActive = 0;
            }
            ikState->modifiedIKBones |= 2u;
            if (movingLerp > 0.0)
            {
                footVec[0] = ofsVec[0];
                footVec[1] = ofsVec[1];
                footVec[2] = 0.0f;
                v169 = ikState->footMats[0][3];
                v168 = ikState->footMats[0][3];
                ikState->footMats[0][3][0] = (float)(movingLerp * ofsVec[0]) + ikState->footMats[0][3][0];
                v169[1] = (float)(movingLerp * footVec[1]) + v168[1];
                v169[2] = (float)(movingLerp * footVec[2]) + v168[2];
                v167 = ikState->footMats[1][3];
                v166 = ikState->footMats[1][3];
                ikState->footMats[1][3][0] = (float)(movingLerp * footVec[0]) + ikState->footMats[1][3][0];
                v167[1] = (float)(movingLerp * footVec[1]) + v166[1];
                v167[2] = (float)(movingLerp * footVec[2]) + v166[2];
            }
        }
        if (preControllers)
        {
            pelvisPitchScaleUp = 0.0f;
            pelvisPitchScaleDn = 0.0f;
            if (playerAngles[0] > 0.0)
                v6 = pelvisPitchScaleDn;
            else
                v6 = pelvisPitchScaleUp;
            v163 = v6;
            pelvisPitchScale = (float)(1.0 - movingLerp) * v6;
            if (pelvisPitchScale > 0.0)
            {
                rot[3] = layerLerp * pelvisPitchScale;
                rot[0] = (float)(layerLerp * pelvisPitchScale) * playerAngles[0];
                rot[1] = (float)(layerLerp * pelvisPitchScale) * playerAngles[1];
                rot[2] = (float)(layerLerp * pelvisPitchScale) * playerAngles[2];
                ikRotateBone(ikState, IKBONE_PELVIS, rot, 0);
            }
            fPitchPow = 1.1f;
            v159 = playerAngles[0];
            v158 = fabs(playerAngles[0]);
            v7 = (float)((fabs(playerAngles[0])) / 90.0);
            //__libm_sse2_pow(v14, v16);

            v7 = pow(fabs(playerAngles[0]) / 90.0f, 1.1f);

            //*(float *)&v7 = v7;
            v157 = *(float *)&v7 * layerLerp;
            v156 = playerAngles[0] <= 0.0;
            bIsPitchDirectionUp = playerAngles[0] <= 0.0;
            IKImport_GetPitchWeaponOffset(ikState, v156, weapOfs);
            weapOfs[0] = v157 * weapOfs[0];
            weapOfs[1] = v157 * weapOfs[1];
            weapOfs[2] = v157 * weapOfs[2];
            v153 = ikState->handMats[0][3];
            v152 = ikState->handMats[0][3];
            ikState->handMats[0][3][0] = ikState->handMats[0][3][0] + weapOfs[0];
            v153[1] = v152[1] + weapOfs[1];
            v153[2] = v152[2] + weapOfs[2];
            v151 = ikState->handMats[1][3];
            v150 = ikState->handMats[1][3];
            ikState->handMats[1][3][0] = ikState->handMats[1][3][0] + weapOfs[0];
            v151[1] = v150[1] + weapOfs[1];
            v151[2] = v150[2] + weapOfs[2];
            if (playerAngles[0] < 0.0)
            {
                ikCalcBoneModelMatrix(ikState, 6, (*ikState->matArrayPostIK)[0], headMat);
                v148 = ikState->handMats[1][0];
                if (headMat == (float (*)[4])v147
                    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
                {
                    __debugbreak();
                }
                v147[0] = headMat[0][0];
                v147[1] = headMat[1][0];
                v147[2] = headMat[2][0];
                v147[3] = 0.0f;
                v147[4] = headMat[0][1];
                v147[5] = headMat[1][1];
                v147[6] = headMat[2][1];
                v147[7] = 0.0f;
                v147[8] = headMat[0][2];
                v147[9] = headMat[1][2];
                v147[10] = headMat[2][2];
                v147[11] = 0.0f;
                rightHandFromHead[0] = (float)((float)(*v148 * headMat[0][0]) + (float)(v148[1] * headMat[0][1]))
                    + (float)(v148[2] * headMat[0][2]);
                rightHandFromHead[1] = (float)((float)(*v148 * headMat[1][0]) + (float)(v148[1] * headMat[1][1]))
                    + (float)(v148[2] * headMat[1][2]);
                *(float *)&v138 = (float)((float)(*v148 * headMat[2][0]) + (float)(v148[1] * headMat[2][1]))
                    + (float)(v148[2] * headMat[2][2]);
                HIDWORD(v138) = 0.0f;
                *(float *)&v139 = (float)((float)(v148[4] * headMat[0][0]) + (float)(v148[5] * headMat[0][1]))
                    + (float)(v148[6] * headMat[0][2]);
                *((float *)&v139 + 1) = (float)((float)(v148[4] * headMat[1][0]) + (float)(v148[5] * headMat[1][1]))
                    + (float)(v148[6] * headMat[1][2]);
                *(float *)&v140 = (float)((float)(v148[4] * headMat[2][0]) + (float)(v148[5] * headMat[2][1]))
                    + (float)(v148[6] * headMat[2][2]);
                HIDWORD(v140) = 0.0f;
                *(float *)&v141 = (float)((float)(v148[8] * headMat[0][0]) + (float)(v148[9] * headMat[0][1]))
                    + (float)(v148[10] * headMat[0][2]);
                *((float *)&v141 + 1) = (float)((float)(v148[8] * headMat[1][0]) + (float)(v148[9] * headMat[1][1]))
                    + (float)(v148[10] * headMat[1][2]);
                *(float *)&v142 = (float)((float)(v148[8] * headMat[2][0]) + (float)(v148[9] * headMat[2][1]))
                    + (float)(v148[10] * headMat[2][2]);
                HIDWORD(v142) = 0.0f;
                //LODWORD(v136[4]) = headMat[3];
                //LODWORD(v136[3]) = v148 + 12;
                v136[0] = v148[12] - headMat[3][0];
                v136[1] = v148[13] - headMat[3][1];
                v136[2] = v148[14] - headMat[3][2];
                ikMatrixTransformVector34(v136, (const float (*)[4])v147, &v143);
                v146 = 1.0f;
                headAndNeckPitchScale = 0.3f;
                neckPitchFrac = 0.7f;
                v133 = (float)(layerLerp * 0.69999999) * 0.30000001;
                rot[0] = v133 * playerAngles[0];
                rot[1] = v133 * playerAngles[1];
                rot[2] = v133 * playerAngles[2];
                ikRotateBone(ikState, IKBONE_NECK, rot, 0);
                headPitchFrac = 0.3f;
                v131 = (float)(layerLerp * 0.30000001) * 0.30000001;
                rot[0] = v131 * playerAngles[0];
                rot[1] = v131 * playerAngles[1];
                rot[2] = v131 * playerAngles[2];
                ikRotateBone(ikState, IKBONE_HEAD, rot, 0);
                ikCalcBoneModelMatrix(ikState, 6, (*ikState->matArrayPostIK)[0], headMat);
                v130 = ikState->handMats[1][0];
                v129 = rightHandFromHead;
                v119 = *(_QWORD *)rightHandFromHead;
                v120 = v138;
                v118 = &v139;
                v121 = v139;
                v122 = v140;
                v117 = &v141;
                v123 = v141;
                v124 = v142;
                v116 = &v143;
                v125 = v143;
                v126 = v144;
                v127 = v145;
                v128 = v146;
                v115 = headMat[0];
                v107 = *(_QWORD *)&headMat[0][0];
                v108 = *(_QWORD *)&headMat[0][2];
                v106 = headMat[1];
                v109 = *(_QWORD *)&headMat[1][0];
                v110 = *(_QWORD *)&headMat[1][2];
                v105 = headMat[2];
                v111 = *(_QWORD *)&headMat[2][0];
                v112 = *(_QWORD *)&headMat[2][2];
                v104 = headMat[3];
                v113 = *(_QWORD *)&headMat[3][0];
                v114 = *(_QWORD *)&headMat[3][2];
                *(float *)&v90[1] = (float)((float)((float)(rightHandFromHead[0] * headMat[0][0])
                    + (float)(rightHandFromHead[1] * headMat[1][0]))
                    + (float)(*(float *)&v138 * headMat[2][0]))
                    + (float)(*((float *)&v138 + 1) * headMat[3][0]);
                *(float *)&v90[2] = (float)((float)((float)(rightHandFromHead[0] * headMat[0][1])
                    + (float)(rightHandFromHead[1] * headMat[1][1]))
                    + (float)(*(float *)&v138 * headMat[2][1]))
                    + (float)(*((float *)&v138 + 1) * headMat[3][1]);
                *(float *)&v91 = (float)((float)((float)(rightHandFromHead[0] * headMat[0][2])
                    + (float)(rightHandFromHead[1] * headMat[1][2]))
                    + (float)(*(float *)&v138 * headMat[2][2]))
                    + (float)(*((float *)&v138 + 1) * headMat[3][2]);
                *((float *)&v91 + 1) = (float)((float)((float)(rightHandFromHead[0] * headMat[0][3])
                    + (float)(rightHandFromHead[1] * headMat[1][3]))
                    + (float)(*(float *)&v138 * headMat[2][3]))
                    + (float)(*((float *)&v138 + 1) * headMat[3][3]);
                v92 = (float)((float)((float)(*(float *)&v139 * headMat[0][0]) + (float)(*((float *)&v139 + 1) * headMat[1][0]))
                    + (float)(*(float *)&v140 * headMat[2][0]))
                    + (float)(*((float *)&v140 + 1) * headMat[3][0]);
                v93 = (float)((float)((float)(*(float *)&v139 * headMat[0][1]) + (float)(*((float *)&v139 + 1) * headMat[1][1]))
                    + (float)(*(float *)&v140 * headMat[2][1]))
                    + (float)(*((float *)&v140 + 1) * headMat[3][1]);
                v94 = (float)((float)((float)(*(float *)&v139 * headMat[0][2]) + (float)(*((float *)&v139 + 1) * headMat[1][2]))
                    + (float)(*(float *)&v140 * headMat[2][2]))
                    + (float)(*((float *)&v140 + 1) * headMat[3][2]);
                v95 = (float)((float)((float)(*(float *)&v139 * headMat[0][3]) + (float)(*((float *)&v139 + 1) * headMat[1][3]))
                    + (float)(*(float *)&v140 * headMat[2][3]))
                    + (float)(*((float *)&v140 + 1) * headMat[3][3]);
                v96 = (float)((float)((float)(*(float *)&v141 * headMat[0][0]) + (float)(*((float *)&v141 + 1) * headMat[1][0]))
                    + (float)(*(float *)&v142 * headMat[2][0]))
                    + (float)(*((float *)&v142 + 1) * headMat[3][0]);
                v97 = (float)((float)((float)(*(float *)&v141 * headMat[0][1]) + (float)(*((float *)&v141 + 1) * headMat[1][1]))
                    + (float)(*(float *)&v142 * headMat[2][1]))
                    + (float)(*((float *)&v142 + 1) * headMat[3][1]);
                v98 = (float)((float)((float)(*(float *)&v141 * headMat[0][2]) + (float)(*((float *)&v141 + 1) * headMat[1][2]))
                    + (float)(*(float *)&v142 * headMat[2][2]))
                    + (float)(*((float *)&v142 + 1) * headMat[3][2]);
                v99 = (float)((float)((float)(*(float *)&v141 * headMat[0][3]) + (float)(*((float *)&v141 + 1) * headMat[1][3]))
                    + (float)(*(float *)&v142 * headMat[2][3]))
                    + (float)(*((float *)&v142 + 1) * headMat[3][3]);
                v100 = (float)((float)((float)(v143 * headMat[0][0]) + (float)(v144 * headMat[1][0]))
                    + (float)(v145 * headMat[2][0]))
                    + (float)(v146 * headMat[3][0]);
                v101 = (float)((float)((float)(v143 * headMat[0][1]) + (float)(v144 * headMat[1][1]))
                    + (float)(v145 * headMat[2][1]))
                    + (float)(v146 * headMat[3][1]);
                v102 = (float)((float)((float)(v143 * headMat[0][2]) + (float)(v144 * headMat[1][2]))
                    + (float)(v145 * headMat[2][2]))
                    + (float)(v146 * headMat[3][2]);
                v103 = (float)((float)((float)(v143 * headMat[0][3]) + (float)(v144 * headMat[1][3]))
                    + (float)(v145 * headMat[2][3]))
                    + (float)(v146 * headMat[3][3]);
                //v90[0] = ikState->handMats[1];
                *(_QWORD *)&ikState->handMats[1][0][0] = *(_QWORD *)&v90[1];
                *(_QWORD *)&ikState->handMats[1][0][2] = v91;
                v89 = v130 + 4;
                v8 = v130 + 4;
                v130[4] = v92;
                v8[1] = v93;
                v8[2] = v94;
                v8[3] = v95;
                v88 = v130 + 8;
                v9 = v130 + 8;
                v130[8] = v96;
                v9[1] = v97;
                v9[2] = v98;
                v9[3] = v99;
                v87 = v130 + 12;
                v10 = v130 + 12;
                v130[12] = v100;
                v10[1] = v101;
                v10[2] = v102;
                v10[3] = v103;
            }
            v86 = ikState->handMats[0][0];
            v85 = ikState->handMats[1][0];
            v84 = ikState->leftHandFromRightHandMat[0];
            v83 = ikState->leftHandFromRightHandMat[0];
            v72 = ikState->leftHandFromRightHandMat[0][0];
            v73 = *(_QWORD *)&ikState->leftHandFromRightHandMat[0][1];
            v74 = ikState->leftHandFromRightHandMat[0][3];
            v71 = ikState->leftHandFromRightHandMat[1];
            v75 = ikState->leftHandFromRightHandMat[1][0];
            v76 = *(_QWORD *)&ikState->leftHandFromRightHandMat[1][1];
            v77 = ikState->leftHandFromRightHandMat[1][3];
            v70 = ikState->leftHandFromRightHandMat[2];
            v78 = ikState->leftHandFromRightHandMat[2][0];
            v79 = *(_QWORD *)&ikState->leftHandFromRightHandMat[2][1];
            v80 = ikState->leftHandFromRightHandMat[2][3];
            v69 = ikState->leftHandFromRightHandMat[3];
            v81 = *(_QWORD *)&ikState->leftHandFromRightHandMat[3][0];
            v82 = *(_QWORD *)&ikState->leftHandFromRightHandMat[3][2];
            v68 = ikState->handMats[1][0];
            v57 = ikState->handMats[1][0][0];
            v58 = *(_QWORD *)&ikState->handMats[1][0][1];
            v59 = ikState->handMats[1][0][3];
            v56 = ikState->handMats[1][1];
            v60 = ikState->handMats[1][1][0];
            v61 = *(_QWORD *)&ikState->handMats[1][1][1];
            v62 = ikState->handMats[1][1][3];
            v55 = ikState->handMats[1][2];
            v63 = ikState->handMats[1][2][0];
            v64 = *(_QWORD *)&ikState->handMats[1][2][1];
            v65 = ikState->handMats[1][2][3];
            v54 = ikState->handMats[1][3];
            v66 = *(_QWORD *)&ikState->handMats[1][3][0];
            v67 = *(_QWORD *)&ikState->handMats[1][3][2];
            *(float *)&v40[1] = (float)((float)((float)(v72 * v57) + (float)(*(float *)&v73 * v60))
                + (float)(*((float *)&v73 + 1) * v63))
                + (float)(v74 * *(float *)&v66);
            *(float *)&v40[2] = (float)((float)((float)(v72 * *(float *)&v58) + (float)(*(float *)&v73 * *(float *)&v61))
                + (float)(*((float *)&v73 + 1) * *(float *)&v64))
                + (float)(v74 * *((float *)&v66 + 1));
            *(float *)&v41 = (float)((float)((float)(v72 * *((float *)&v58 + 1))
                + (float)(*(float *)&v73 * *((float *)&v61 + 1)))
                + (float)(*((float *)&v73 + 1) * *((float *)&v64 + 1)))
                + (float)(v74 * *(float *)&v67);
            *((float *)&v41 + 1) = (float)((float)((float)(v72 * v59) + (float)(*(float *)&v73 * v62))
                + (float)(*((float *)&v73 + 1) * v65))
                + (float)(v74 * *((float *)&v67 + 1));
            v42 = (float)((float)((float)(v75 * v57) + (float)(*(float *)&v76 * v60)) + (float)(*((float *)&v76 + 1) * v63))
                + (float)(v77 * *(float *)&v66);
            v43 = (float)((float)((float)(v75 * *(float *)&v58) + (float)(*(float *)&v76 * *(float *)&v61))
                + (float)(*((float *)&v76 + 1) * *(float *)&v64))
                + (float)(v77 * *((float *)&v66 + 1));
            v44 = (float)((float)((float)(v75 * *((float *)&v58 + 1)) + (float)(*(float *)&v76 * *((float *)&v61 + 1)))
                + (float)(*((float *)&v76 + 1) * *((float *)&v64 + 1)))
                + (float)(v77 * *(float *)&v67);
            v45 = (float)((float)((float)(v75 * v59) + (float)(*(float *)&v76 * v62)) + (float)(*((float *)&v76 + 1) * v65))
                + (float)(v77 * *((float *)&v67 + 1));
            v46 = (float)((float)((float)(v78 * v57) + (float)(*(float *)&v79 * v60)) + (float)(*((float *)&v79 + 1) * v63))
                + (float)(v80 * *(float *)&v66);
            v47 = (float)((float)((float)(v78 * *(float *)&v58) + (float)(*(float *)&v79 * *(float *)&v61))
                + (float)(*((float *)&v79 + 1) * *(float *)&v64))
                + (float)(v80 * *((float *)&v66 + 1));
            v48 = (float)((float)((float)(v78 * *((float *)&v58 + 1)) + (float)(*(float *)&v79 * *((float *)&v61 + 1)))
                + (float)(*((float *)&v79 + 1) * *((float *)&v64 + 1)))
                + (float)(v80 * *(float *)&v67);
            v49 = (float)((float)((float)(v78 * v59) + (float)(*(float *)&v79 * v62)) + (float)(*((float *)&v79 + 1) * v65))
                + (float)(v80 * *((float *)&v67 + 1));
            v50 = (float)((float)((float)(*(float *)&v81 * v57) + (float)(*((float *)&v81 + 1) * v60))
                + (float)(*(float *)&v82 * v63))
                + (float)(*((float *)&v82 + 1) * *(float *)&v66);
            v51 = (float)((float)((float)(*(float *)&v81 * *(float *)&v58) + (float)(*((float *)&v81 + 1) * *(float *)&v61))
                + (float)(*(float *)&v82 * *(float *)&v64))
                + (float)(*((float *)&v82 + 1) * *((float *)&v66 + 1));
            v52 = (float)((float)((float)(*(float *)&v81 * *((float *)&v58 + 1))
                + (float)(*((float *)&v81 + 1) * *((float *)&v61 + 1)))
                + (float)(*(float *)&v82 * *((float *)&v64 + 1)))
                + (float)(*((float *)&v82 + 1) * *(float *)&v67);
            v53 = (float)((float)((float)(*(float *)&v81 * v59) + (float)(*((float *)&v81 + 1) * v62))
                + (float)(*(float *)&v82 * v65))
                + (float)(*((float *)&v82 + 1) * *((float *)&v67 + 1));
            //v40[0] = ikState->handMats;
            *(_QWORD *)&ikState->handMats[0][0][0] = *(_QWORD *)&v40[1];
            *(_QWORD *)&ikState->handMats[0][0][2] = v41;
            v39 = v86 + 4;
            v11 = v86 + 4;
            v86[4] = v42;
            v11[1] = v43;
            v11[2] = v44;
            v11[3] = v45;
            v38 = v86 + 8;
            v12 = v86 + 8;
            v86[8] = v46;
            v12[1] = v47;
            v12[2] = v48;
            v12[3] = v49;
            v37 = v86 + 12;
            v13 = v86 + 12;
            v86[12] = v50;
            v13[1] = v51;
            v13[2] = v52;
            v13[3] = v53;
        }
        v36 = ikState->handMats[0][3];
        v35 = ikState->handMats[0][3];
        ikState->handMats[0][3][0] = ikState->handMats[0][3][0] + ofsVec[0];
        v36[1] = v35[1] + ofsVec[1];
        v36[2] = v35[2] + ofsVec[2];
        v34 = ikState->handMats[1][3];
        v33 = ikState->handMats[1][3];
        ikState->handMats[1][3][0] = ikState->handMats[1][3][0] + ofsVec[0];
        v34[1] = v33[1] + ofsVec[1];
        v34[2] = v33[2] + ofsVec[2];
        out = ikState->leftHandFromRightHandMat[0];
        v31 = ikState->handMats[1][0];
        v30 = ikState->handMats[0][0];
        if ((float *)ikState->handMats[1] == v21
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
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
        v25 = 0.0f;
        v26 = v31[2];
        v27 = v31[6];
        v28 = v31[10];
        v29 = 0.0f;
        *out = (float)((float)(*v30 * v21[0]) + (float)(v30[1] * v22)) + (float)(v30[2] * v26);
        out[1] = (float)((float)(*v30 * v21[1]) + (float)(v30[1] * v23)) + (float)(v30[2] * v27);
        out[2] = (float)((float)(*v30 * v21[2]) + (float)(v30[1] * v24)) + (float)(v30[2] * v28);
        out[3] = 0.0f;
        out[4] = (float)((float)(v30[4] * v21[0]) + (float)(v30[5] * v22)) + (float)(v30[6] * v26);
        out[5] = (float)((float)(v30[4] * v21[1]) + (float)(v30[5] * v23)) + (float)(v30[6] * v27);
        out[6] = (float)((float)(v30[4] * v21[2]) + (float)(v30[5] * v24)) + (float)(v30[6] * v28);
        out[7] = 0.0f;
        out[8] = (float)((float)(v30[8] * v21[0]) + (float)(v30[9] * v22)) + (float)(v30[10] * v26);
        out[9] = (float)((float)(v30[8] * v21[1]) + (float)(v30[9] * v23)) + (float)(v30[10] * v27);
        out[10] = (float)((float)(v30[8] * v21[2]) + (float)(v30[9] * v24)) + (float)(v30[10] * v28);
        out[11] = 0.0f;
        v20 = v31 + 12;
        v19 = v30 + 12;
        *((float *)&v16 + 1) = v30[12] - v31[12];
        v17 = v30[13] - v31[13];
        v18 = v30[14] - v31[14];
        ikMatrixTransformVector34((const float *)&v16 + 1, (const float (*)[4])v21, out + 12);
        out[15] = 1.0f;
    }
}

// local variable allocation has failed, the output may be wrong!
void    IK_Layer_PreventHeadClip(IKState *ikState)
{
    float v2; // [esp+1Ch] [ebp-2D8h]
    float v3; // [esp+28h] [ebp-2CCh]
    float v4; // [esp+34h] [ebp-2C0h]
    float v5; // [esp+40h] [ebp-2B4h]
    float v6; // [esp+4Ch] [ebp-2A8h]
    float *v7; // [esp+50h] [ebp-2A4h]
    float v8[5]; // [esp+54h] [ebp-2A0h] BYREF
    float torsoFromLegsMat[4][4]; // [esp+68h] [ebp-28Ch] BYREF
    float v10[4]; // [esp+A8h] [ebp-24Ch] BYREF
    float v11; // [esp+B8h] [ebp-23Ch]
    float v12; // [esp+BCh] [ebp-238h]
    float v13; // [esp+C0h] [ebp-234h]
    int v14; // [esp+C4h] [ebp-230h]
    float v15; // [esp+C8h] [ebp-22Ch]
    float v16; // [esp+CCh] [ebp-228h]
    float v17; // [esp+D0h] [ebp-224h]
    int v18; // [esp+D4h] [ebp-220h]
    float *v19; // [esp+E8h] [ebp-20Ch]
    float *v20; // [esp+ECh] [ebp-208h]
    float headClipMaxPelvisShift; // [esp+F0h] [ebp-204h]
    float traceHitPos[3]; // [esp+F4h] [ebp-200h] BYREF
    float traceHitNormal[3]; // [esp+100h] [ebp-1F4h] BYREF
    float end[3]; // [esp+10Ch] [ebp-1E8h] BYREF
    float start[3]; // [esp+118h] [ebp-1DCh] BYREF
    float fwd[3]; // [esp+124h] [ebp-1D0h]
    float traceHitFraction; // [esp+130h] [ebp-1C4h] BYREF
    float headCenterPointWorld[9]; // [esp+134h] [ebp-1C0h]
    float headMatWorld[4][4]; // [esp+158h] [ebp-19Ch] BYREF
    float v30; // [esp+198h] [ebp-15Ch]
    float v31; // [esp+19Ch] [ebp-158h]
    float v32; // [esp+1A0h] [ebp-154h]
    float v33; // [esp+1A4h] [ebp-150h]
    float v34; // [esp+1A8h] [ebp-14Ch]
    float v35; // [esp+1ACh] [ebp-148h]
    float v36; // [esp+1B0h] [ebp-144h]
    float v37; // [esp+1B4h] [ebp-140h]
    float v38; // [esp+1B8h] [ebp-13Ch]
    float v39; // [esp+1BCh] [ebp-138h]
    float v40; // [esp+1C0h] [ebp-134h]
    float v41; // [esp+1C4h] [ebp-130h]
    float v42; // [esp+1C8h] [ebp-12Ch]
    float v43; // [esp+1CCh] [ebp-128h]
    float v44; // [esp+1D0h] [ebp-124h]
    float v45; // [esp+1D4h] [ebp-120h]
    float *v46; // [esp+1DCh] [ebp-118h]
    float *v47; // [esp+1E0h] [ebp-114h]
    float *v48; // [esp+1E4h] [ebp-110h]
    float v49; // [esp+1E8h] [ebp-10Ch]
    __int64 v50; // [esp+1ECh] [ebp-108h]
    float v51; // [esp+1F4h] [ebp-100h]
    float v52; // [esp+1F8h] [ebp-FCh]
    __int64 v53; // [esp+1FCh] [ebp-F8h]
    float v54; // [esp+204h] [ebp-F0h]
    float v55; // [esp+208h] [ebp-ECh]
    __int64 v56; // [esp+20Ch] [ebp-E8h]
    float v57; // [esp+214h] [ebp-E0h]
    __int64 v58; // [esp+218h] [ebp-DCh]
    __int64 v59; // [esp+220h] [ebp-D4h]
    float *v60; // [esp+228h] [ebp-CCh]
    float *v61; // [esp+22Ch] [ebp-C8h]
    float *v62; // [esp+230h] [ebp-C4h]
    float *v63; // [esp+234h] [ebp-C0h]
    __int64 v64; // [esp+238h] [ebp-BCh]
    __int64 v65; // [esp+240h] [ebp-B4h]
    __int64 v66; // [esp+248h] [ebp-ACh]
    __int64 v67; // [esp+250h] [ebp-A4h]
    __int64 v68; // [esp+258h] [ebp-9Ch]
    __int64 v69; // [esp+260h] [ebp-94h]
    __int64 v70; // [esp+268h] [ebp-8Ch]
    __int64 v71; // [esp+270h] [ebp-84h]
    float *v72; // [esp+278h] [ebp-7Ch]
    float *v73; // [esp+27Ch] [ebp-78h]
    float *v74; // [esp+280h] [ebp-74h]
    float v75; // [esp+284h] [ebp-70h]
    float *v76; // [esp+288h] [ebp-6Ch]
    float headClipPlayerCollisionHullRadius; // [esp+28Ch] [ebp-68h]
    float headClipTraceDist; // [esp+290h] [ebp-64h]
    float headClipTraceRadius; // [esp+294h] [ebp-60h]
    float headMat[4][4]; // [esp+298h] [ebp-5Ch] BYREF
    float layerLerp; // [esp+2DCh] [ebp-18h]
    int ikLayerName; // [esp+2E0h] [ebp-14h]
    //_UNKNOWN *v83; // [esp+2E8h] [ebp-Ch]
    //IKState *ikStatea; // [esp+2ECh] [ebp-8h]
    //int vars0; // [esp+2F4h] [ebp+0h]
    //
    //v83 = a1;
    //ikStatea = (IKState *)vars0;
    ikLayerName = 20;
    layerLerp = IKImport_GetLayerLerp(ikState, IKLAYER_PREVENT_HEAD_CLIP, 0.3f);
    if (layerLerp > 0.0)
    {
        ikCalcBoneModelMatrix(ikState, 6, (*ikState->matArrayPostIK)[0], headMat);
        headClipTraceRadius = 0.0f;
        headClipTraceDist = 12.0f;
        headClipPlayerCollisionHullRadius = 14.0f;
        if (Vec2Length(headMat[3]) > 14.0 - 0.0)
        {
            Vec2Normalize(headMat[3]);
            v76 = headMat[3];
            v75 = headClipPlayerCollisionHullRadius - headClipTraceRadius;
            v74 = headMat[3];
            headMat[3][0] = (float)(headClipPlayerCollisionHullRadius - headClipTraceRadius) * headMat[3][0];
            headMat[3][1] = (float)(headClipPlayerCollisionHullRadius - headClipTraceRadius) * headMat[3][1];
        }
        v73 = ikState->entityLegsXform[0];
        v72 = headMat[0];
        v64 = *(_QWORD *)&headMat[0][0];
        v65 = *(_QWORD *)&headMat[0][2];
        v63 = headMat[1];
        v66 = *(_QWORD *)&headMat[1][0];
        v67 = *(_QWORD *)&headMat[1][2];
        v62 = headMat[2];
        v68 = *(_QWORD *)&headMat[2][0];
        v69 = *(_QWORD *)&headMat[2][2];
        v61 = headMat[3];
        v70 = *(_QWORD *)&headMat[3][0];
        v71 = *(_QWORD *)&headMat[3][2];
        v60 = ikState->entityLegsXform[0];
        v49 = ikState->entityLegsXform[0][0];
        v50 = *(_QWORD *)&ikState->entityLegsXform[0][1];
        v51 = ikState->entityLegsXform[0][3];
        v48 = ikState->entityLegsXform[1];
        v52 = ikState->entityLegsXform[1][0];
        v53 = *(_QWORD *)&ikState->entityLegsXform[1][1];
        v54 = ikState->entityLegsXform[1][3];
        v47 = ikState->entityLegsXform[2];
        v55 = ikState->entityLegsXform[2][0];
        v56 = *(_QWORD *)&ikState->entityLegsXform[2][1];
        v57 = ikState->entityLegsXform[2][3];
        v46 = ikState->entityLegsXform[3];
        v58 = *(_QWORD *)&ikState->entityLegsXform[3][0];
        v59 = *(_QWORD *)&ikState->entityLegsXform[3][2];
        v30 = (float)((float)((float)(headMat[0][0] * v49) + (float)(headMat[0][1] * v52)) + (float)(headMat[0][2] * v55))
            + (float)(headMat[0][3] * *(float *)&v58);
        v31 = (float)((float)((float)(headMat[0][0] * *(float *)&v50) + (float)(headMat[0][1] * *(float *)&v53))
            + (float)(headMat[0][2] * *(float *)&v56))
            + (float)(headMat[0][3] * *((float *)&v58 + 1));
        v32 = (float)((float)((float)(headMat[0][0] * *((float *)&v50 + 1)) + (float)(headMat[0][1] * *((float *)&v53 + 1)))
            + (float)(headMat[0][2] * *((float *)&v56 + 1)))
            + (float)(headMat[0][3] * *(float *)&v59);
        v33 = (float)((float)((float)(headMat[0][0] * v51) + (float)(headMat[0][1] * v54)) + (float)(headMat[0][2] * v57))
            + (float)(headMat[0][3] * *((float *)&v59 + 1));
        v34 = (float)((float)((float)(headMat[1][0] * v49) + (float)(headMat[1][1] * v52)) + (float)(headMat[1][2] * v55))
            + (float)(headMat[1][3] * *(float *)&v58);
        v35 = (float)((float)((float)(headMat[1][0] * *(float *)&v50) + (float)(headMat[1][1] * *(float *)&v53))
            + (float)(headMat[1][2] * *(float *)&v56))
            + (float)(headMat[1][3] * *((float *)&v58 + 1));
        v36 = (float)((float)((float)(headMat[1][0] * *((float *)&v50 + 1)) + (float)(headMat[1][1] * *((float *)&v53 + 1)))
            + (float)(headMat[1][2] * *((float *)&v56 + 1)))
            + (float)(headMat[1][3] * *(float *)&v59);
        v37 = (float)((float)((float)(headMat[1][0] * v51) + (float)(headMat[1][1] * v54)) + (float)(headMat[1][2] * v57))
            + (float)(headMat[1][3] * *((float *)&v59 + 1));
        v38 = (float)((float)((float)(headMat[2][0] * v49) + (float)(headMat[2][1] * v52)) + (float)(headMat[2][2] * v55))
            + (float)(headMat[2][3] * *(float *)&v58);
        v39 = (float)((float)((float)(headMat[2][0] * *(float *)&v50) + (float)(headMat[2][1] * *(float *)&v53))
            + (float)(headMat[2][2] * *(float *)&v56))
            + (float)(headMat[2][3] * *((float *)&v58 + 1));
        v40 = (float)((float)((float)(headMat[2][0] * *((float *)&v50 + 1)) + (float)(headMat[2][1] * *((float *)&v53 + 1)))
            + (float)(headMat[2][2] * *((float *)&v56 + 1)))
            + (float)(headMat[2][3] * *(float *)&v59);
        v41 = (float)((float)((float)(headMat[2][0] * v51) + (float)(headMat[2][1] * v54)) + (float)(headMat[2][2] * v57))
            + (float)(headMat[2][3] * *((float *)&v59 + 1));
        v42 = (float)((float)((float)(headMat[3][0] * v49) + (float)(headMat[3][1] * v52)) + (float)(headMat[3][2] * v55))
            + (float)(headMat[3][3] * *(float *)&v58);
        v43 = (float)((float)((float)(headMat[3][0] * *(float *)&v50) + (float)(headMat[3][1] * *(float *)&v53))
            + (float)(headMat[3][2] * *(float *)&v56))
            + (float)(headMat[3][3] * *((float *)&v58 + 1));
        v44 = (float)((float)((float)(headMat[3][0] * *((float *)&v50 + 1)) + (float)(headMat[3][1] * *((float *)&v53 + 1)))
            + (float)(headMat[3][2] * *((float *)&v56 + 1)))
            + (float)(headMat[3][3] * *(float *)&v59);
        v45 = (float)((float)((float)(headMat[3][0] * v51) + (float)(headMat[3][1] * v54)) + (float)(headMat[3][2] * v57))
            + (float)(headMat[3][3] * *((float *)&v59 + 1));
        //LODWORD(headCenterPointWorld[8]) = headMatWorld;
        headMatWorld[0][0] = v30;
        headMatWorld[0][1] = v31;
        headMatWorld[0][2] = v32;
        headMatWorld[0][3] = v33;
        //LODWORD(headCenterPointWorld[7]) = headMatWorld[1];
        headMatWorld[1][0] = v34;
        headMatWorld[1][1] = v35;
        headMatWorld[1][2] = v36;
        headMatWorld[1][3] = v37;
        //LODWORD(headCenterPointWorld[6]) = headMatWorld[2];
        headMatWorld[2][0] = v38;
        headMatWorld[2][1] = v39;
        headMatWorld[2][2] = v40;
        headMatWorld[2][3] = v41;
        //LODWORD(headCenterPointWorld[5]) = headMatWorld[3];
        headMatWorld[3][0] = v42;
        headMatWorld[3][1] = v43;
        headMatWorld[3][2] = v44;
        headMatWorld[3][3] = v45;
        //LODWORD(headCenterPointWorld[4]) = headMatWorld[3];
        headCenterPointWorld[0] = v42;
        headCenterPointWorld[1] = v43;
        headCenterPointWorld[2] = v44;
        traceHitFraction = 0.0f;
        fwd[0] = v30;
        fwd[1] = v31;
        fwd[2] = v32;
        start[0] = v42;
        start[1] = v43;
        start[2] = v44;
        end[0] = (float)(headClipTraceDist * v30) + v42;
        end[1] = (float)(headClipTraceDist * v31) + v43;
        end[2] = (float)(headClipTraceDist * v32) + v44;
        if (IKImport_TraceBox(
            ikState,
            start,
            end,
            headClipTraceRadius,
            traceHitPos,
            traceHitNormal,
            -1,
            &traceHitFraction,
            0))
        {
            headClipMaxPelvisShift = 12.0f;
            v20 = ikState->entityLegsXform[0];
            v19 = ikState->entityXform[0];
            if ((float *)ikState->entityLegsXform == v10
                && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
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
            v14 = 0.0f;
            v15 = v20[2];
            v16 = v20[6];
            v17 = v20[10];
            v18 = 0.0f;
            torsoFromLegsMat[0][0] = (float)((float)(*v19 * v10[0]) + (float)(v19[1] * v11)) + (float)(v19[2] * v15);
            torsoFromLegsMat[0][1] = (float)((float)(*v19 * v10[1]) + (float)(v19[1] * v12)) + (float)(v19[2] * v16);
            torsoFromLegsMat[0][2] = (float)((float)(*v19 * v10[2]) + (float)(v19[1] * v13)) + (float)(v19[2] * v17);
            torsoFromLegsMat[0][3] = 0.0f;
            torsoFromLegsMat[1][0] = (float)((float)(v19[4] * v10[0]) + (float)(v19[5] * v11)) + (float)(v19[6] * v15);
            torsoFromLegsMat[1][1] = (float)((float)(v19[4] * v10[1]) + (float)(v19[5] * v12)) + (float)(v19[6] * v16);
            torsoFromLegsMat[1][2] = (float)((float)(v19[4] * v10[2]) + (float)(v19[5] * v13)) + (float)(v19[6] * v17);
            torsoFromLegsMat[1][3] = 0.0f;
            torsoFromLegsMat[2][0] = (float)((float)(v19[8] * v10[0]) + (float)(v19[9] * v11)) + (float)(v19[10] * v15);
            torsoFromLegsMat[2][1] = (float)((float)(v19[8] * v10[1]) + (float)(v19[9] * v12)) + (float)(v19[10] * v16);
            torsoFromLegsMat[2][2] = (float)((float)(v19[8] * v10[2]) + (float)(v19[9] * v13)) + (float)(v19[10] * v17);
            torsoFromLegsMat[2][3] = 0.0f;
            //LODWORD(v8[4]) = v20 + 12;
            //LODWORD(v8[3]) = v19 + 12;
            v8[0] = v19[12] - v20[12];
            v8[1] = v19[13] - v20[13];
            v8[2] = v19[14] - v20[14];
            ikMatrixTransformVector34(v8, (const float (*)[4])v10, torsoFromLegsMat[3]);
            torsoFromLegsMat[3][3] = 1.0f;
            v7 = (*ikState->matArrayPostIK)[7];
            v6 = (-(1.0 - traceHitFraction)) * headClipMaxPelvisShift;
            *v7 = (float)(v6 * torsoFromLegsMat[0][0]) + *v7;
            v7[1] = (float)(v6 * torsoFromLegsMat[0][1]) + v7[1];
            v5 = (-(1.0 - traceHitFraction)) * headClipMaxPelvisShift;
            ikState->handMats[1][3][0] = (float)(v5 * torsoFromLegsMat[0][0]) + ikState->handMats[1][3][0];
            ikState->handMats[1][3][1] = (float)(v5 * torsoFromLegsMat[0][1]) + ikState->handMats[1][3][1];
            v4 = (-(1.0 - traceHitFraction)) * headClipMaxPelvisShift;
            ikState->handMats[0][3][0] = (float)(v4 * torsoFromLegsMat[0][0]) + ikState->handMats[0][3][0];
            ikState->handMats[0][3][1] = (float)(v4 * torsoFromLegsMat[0][1]) + ikState->handMats[0][3][1];
            v3 = (-(1.0 - traceHitFraction)) * headClipMaxPelvisShift;
            ikState->footMats[1][3][0] = (float)(v3 * torsoFromLegsMat[0][0]) + ikState->footMats[1][3][0];
            ikState->footMats[1][3][1] = (float)(v3 * torsoFromLegsMat[0][1]) + ikState->footMats[1][3][1];
            v2 = (-(1.0 - traceHitFraction)) * headClipMaxPelvisShift;
            ikState->footMats[0][3][0] = (float)(v2 * torsoFromLegsMat[0][0]) + ikState->footMats[0][3][0];
            ikState->footMats[0][3][1] = (float)(v2 * torsoFromLegsMat[0][1]) + ikState->footMats[0][3][1];
            ikState->bHasActiveLayers = 1;
            if (ikState->cacheActive)
            {
                memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
                ikState->cacheActive = 0;
            }
            ikState->modifiedIKBones |= 2u;
        }
    }
}

// local variable allocation has failed, the output may be wrong!
void    IK_ProcessLayers(IKState *ikState)
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
    float v18[4][4]; // [esp-108h] [ebp-258h]
    float v19[5]; // [esp-20h] [ebp-170h] BYREF
    float v20; // [esp-Ch] [ebp-15Ch]
    float v21; // [esp-8h] [ebp-158h]
    float v22; // [esp-4h] [ebp-154h]
    float rightHandFromSpineMat[4][4]; // [esp+0h] [ebp-150h] BYREF
    int v24; // [esp+40h] [ebp-110h]
    float v25; // [esp+44h] [ebp-10Ch]
    float v26; // [esp+48h] [ebp-108h]
    float v27; // [esp+4Ch] [ebp-104h]
    int v28; // [esp+50h] [ebp-100h]
    float v29; // [esp+54h] [ebp-FCh]
    float v30; // [esp+58h] [ebp-F8h]
    float v31; // [esp+5Ch] [ebp-F4h]
    int v32; // [esp+60h] [ebp-F0h]
    float *v33; // [esp+80h] [ebp-D0h]
    float spineMat[4][4]; // [esp+84h] [ebp-CCh] BYREF
    bool v35; // [esp+CEh] [ebp-82h]
    bool v36; // [esp+CFh] [ebp-81h]
    float v37[5]; // [esp+D0h] [ebp-80h] BYREF
    float v38[4][4]; // [esp+E4h] [ebp-6Ch] BYREF
    float *v39; // [esp+130h] [ebp-20h]
    float *v40; // [esp+134h] [ebp-1Ch]
    float *v41; // [esp+138h] [ebp-18h]
    bool v42[8]; // [esp+13Ch] [ebp-14h]
    //_UNKNOWN *v43[2]; // [esp+144h] [ebp-Ch] BYREF
    //int vars0; // [esp+150h] [ebp+0h]
    //
    //v43[0] = a1;
    //v43[1] = (_UNKNOWN *)vars0;
    if (ikState->bJointVarsValid)
    {
        if (ikSystem.inViewer)
        {
            ikState->bHasActiveLayers = 1;
            ikState->bHasArmSolverLayer = 1;
        }
        //BG_EvalVehicleName();
        v42[3] = ikState->ikBoneToObjBone[10] != 161;
        if (!v42[3]
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                1773,
                0,
                "%s",
                "IK_IsBoneAnimated( ikState, IKBONE_LHAND )"))
        {
            __debugbreak();
        }
        ikCalcBoneModelMatrix(ikState, 10, (*ikState->matArrayPostIK)[0], ikState->handMats[0]);
        v42[2] = ikState->ikBoneToObjBone[15] != 161;
        if (!v42[2]
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                1776,
                0,
                "%s",
                "IK_IsBoneAnimated( ikState, IKBONE_RHAND )"))
        {
            __debugbreak();
        }
        ikCalcBoneModelMatrix(ikState, 15, (*ikState->matArrayPostIK)[0], ikState->handMats[1]);
        v41 = ikState->leftHandFromRightHandMat[0];
        v40 = ikState->handMats[1][0];
        v39 = ikState->handMats[0][0];
        if (ikState->handMats[1] == v38
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
        {
            __debugbreak();
        }
        v38[0][0] = *v40;
        v38[0][1] = v40[4];
        v38[0][2] = v40[8];
        v38[0][3] = 0.0f;
        v38[1][0] = v40[1];
        v38[1][1] = v40[5];
        v38[1][2] = v40[9];
        v38[1][3] = 0.0f;
        v38[2][0] = v40[2];
        v38[2][1] = v40[6];
        v38[2][2] = v40[10];
        v38[2][3] = 0.0f;

        *v41 = (float)((float)(*v39 * v38[0][0]) + (float)(v39[1] * v38[1][0])) + (float)(v39[2] * v38[2][0]);
        v41[1] = (float)((float)(*v39 * v38[0][1]) + (float)(v39[1] * v38[1][1])) + (float)(v39[2] * v38[2][1]);
        v41[2] = (float)((float)(*v39 * v38[0][2]) + (float)(v39[1] * v38[1][2])) + (float)(v39[2] * v38[2][2]);
        v41[3] = 0.0f;
        v41[4] = (float)((float)(v39[4] * v38[0][0]) + (float)(v39[5] * v38[1][0])) + (float)(v39[6] * v38[2][0]);
        v41[5] = (float)((float)(v39[4] * v38[0][1]) + (float)(v39[5] * v38[1][1])) + (float)(v39[6] * v38[2][1]);
        v41[6] = (float)((float)(v39[4] * v38[0][2]) + (float)(v39[5] * v38[1][2])) + (float)(v39[6] * v38[2][2]);
        v41[7] = 0.0f;
        v41[8] = (float)((float)(v39[8] * v38[0][0]) + (float)(v39[9] * v38[1][0])) + (float)(v39[10] * v38[2][0]);
        v41[9] = (float)((float)(v39[8] * v38[0][1]) + (float)(v39[9] * v38[1][1])) + (float)(v39[10] * v38[2][1]);
        v41[10] = (float)((float)(v39[8] * v38[0][2]) + (float)(v39[9] * v38[1][2])) + (float)(v39[10] * v38[2][2]);
        v41[11] = 0.0f;
        //LODWORD(v37[4]) = v40 + 12;
        //LODWORD(v37[3]) = v39 + 12;
        v37[0] = v39[12] - v40[12];
        v37[1] = v39[13] - v40[13];
        v37[2] = v39[14] - v40[14];
        ikMatrixTransformVector34(v37, v38, v41 + 12);
        v41[15] = 1.0f;
        v36 = ikState->ikBoneToObjBone[18] != 161;
        if (!v36
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                1782,
                0,
                "%s",
                "IK_IsBoneAnimated( ikState, IKBONE_LFOOT )"))
        {
            __debugbreak();
        }
        ikCalcBoneModelMatrix(ikState, 18, (*ikState->matArrayPostIK)[0], ikState->footMats[0]);
        v35 = ikState->ikBoneToObjBone[21] != 161;
        if (!v35
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_layers.cpp",
                1785,
                0,
                "%s",
                "IK_IsBoneAnimated( ikState, IKBONE_RFOOT )"))
        {
            __debugbreak();
        }
        ikCalcBoneModelMatrix(ikState, 21, (*ikState->matArrayPostIK)[0], ikState->footMats[1]);
        IK_Layer_LeftHandOnGun(ikState);
        if (IKImport_IsClient(ikState))
        {
            IK_Layer_PlayerPitch(ikState, 1);
            ikCalcBoneModelMatrix(ikState, 4, (*ikState->matArrayPostIK)[0], spineMat);
            v33 = ikState->handMats[1][0];
            if (spineMat == (float (*)[4]) & rightHandFromSpineMat[3][1]
                && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out"))
            {
                __debugbreak();
            }
            *(_QWORD *)&rightHandFromSpineMat[3][1] = __PAIR64__(LODWORD(spineMat[1][0]), LODWORD(spineMat[0][0]));
            rightHandFromSpineMat[3][3] = spineMat[2][0];
            v24 = 0.0f;
            v25 = spineMat[0][1];
            v26 = spineMat[1][1];
            v27 = spineMat[2][1];
            v28 = 0.0f;
            v29 = spineMat[0][2];
            v30 = spineMat[1][2];
            v31 = spineMat[2][2];
            v32 = 0.0f;
            v20 = (float)((float)(*v33 * spineMat[0][0]) + (float)(v33[1] * spineMat[0][1]))
                + (float)(v33[2] * spineMat[0][2]);
            v21 = (float)((float)(*v33 * spineMat[1][0]) + (float)(v33[1] * spineMat[1][1]))
                + (float)(v33[2] * spineMat[1][2]);
            v22 = (float)((float)(*v33 * spineMat[2][0]) + (float)(v33[1] * spineMat[2][1]))
                + (float)(v33[2] * spineMat[2][2]);
            rightHandFromSpineMat[0][0] = 0.0f;
            rightHandFromSpineMat[0][1] = (float)((float)(v33[4] * spineMat[0][0]) + (float)(v33[5] * spineMat[0][1]))
                + (float)(v33[6] * spineMat[0][2]);
            rightHandFromSpineMat[0][2] = (float)((float)(v33[4] * spineMat[1][0]) + (float)(v33[5] * spineMat[1][1]))
                + (float)(v33[6] * spineMat[1][2]);
            rightHandFromSpineMat[0][3] = (float)((float)(v33[4] * spineMat[2][0]) + (float)(v33[5] * spineMat[2][1]))
                + (float)(v33[6] * spineMat[2][2]);
            rightHandFromSpineMat[1][0] = 0.0f;
            rightHandFromSpineMat[1][1] = (float)((float)(v33[8] * spineMat[0][0]) + (float)(v33[9] * spineMat[0][1]))
                + (float)(v33[10] * spineMat[0][2]);
            rightHandFromSpineMat[1][2] = (float)((float)(v33[8] * spineMat[1][0]) + (float)(v33[9] * spineMat[1][1]))
                + (float)(v33[10] * spineMat[1][2]);
            rightHandFromSpineMat[1][3] = (float)((float)(v33[8] * spineMat[2][0]) + (float)(v33[9] * spineMat[2][1]))
                + (float)(v33[10] * spineMat[2][2]);
            rightHandFromSpineMat[2][0] = 0.0f;
            //LODWORD(v19[4]) = spineMat[3];
            //LODWORD(v19[3]) = v33 + 12;
            v19[0] = v33[12] - spineMat[3][0];
            v19[1] = v33[13] - spineMat[3][1];
            v19[2] = v33[14] - spineMat[3][2];
            ikMatrixTransformVector34(v19, (const float (*)[4]) & rightHandFromSpineMat[3][1], &rightHandFromSpineMat[2][1]);
            rightHandFromSpineMat[3][0] = 1.0f;
            IK_Layer_PlayerControllers(ikState);
            ikCalcBoneModelMatrix(ikState, 4, (*ikState->matArrayPostIK)[0], spineMat);
            v18[0][0] = (float)((float)((float)(v20 * spineMat[0][1]) + (float)(v21 * spineMat[1][1]))
                + (float)(v22 * spineMat[2][1]))
                + (float)(rightHandFromSpineMat[0][0] * spineMat[3][1]);
            v18[0][1] = (float)((float)((float)(v20 * spineMat[0][2]) + (float)(v21 * spineMat[1][2]))
                + (float)(v22 * spineMat[2][2]))
                + (float)(rightHandFromSpineMat[0][0] * spineMat[3][2]);
            v18[0][2] = (float)((float)((float)(v20 * spineMat[0][3]) + (float)(v21 * spineMat[1][3]))
                + (float)(v22 * spineMat[2][3]))
                + (float)(rightHandFromSpineMat[0][0] * spineMat[3][3]);
            v18[0][3] = (float)((float)((float)(rightHandFromSpineMat[0][1] * spineMat[0][0])
                + (float)(rightHandFromSpineMat[0][2] * spineMat[1][0]))
                + (float)(rightHandFromSpineMat[0][3] * spineMat[2][0]))
                + (float)(rightHandFromSpineMat[1][0] * spineMat[3][0]);
            v18[1][0] = (float)((float)((float)(rightHandFromSpineMat[0][1] * spineMat[0][1])
                + (float)(rightHandFromSpineMat[0][2] * spineMat[1][1]))
                + (float)(rightHandFromSpineMat[0][3] * spineMat[2][1]))
                + (float)(rightHandFromSpineMat[1][0] * spineMat[3][1]);
            v18[1][1] = (float)((float)((float)(rightHandFromSpineMat[0][1] * spineMat[0][2])
                + (float)(rightHandFromSpineMat[0][2] * spineMat[1][2]))
                + (float)(rightHandFromSpineMat[0][3] * spineMat[2][2]))
                + (float)(rightHandFromSpineMat[1][0] * spineMat[3][2]);
            v18[1][2] = (float)((float)((float)(rightHandFromSpineMat[0][1] * spineMat[0][3])
                + (float)(rightHandFromSpineMat[0][2] * spineMat[1][3]))
                + (float)(rightHandFromSpineMat[0][3] * spineMat[2][3]))
                + (float)(rightHandFromSpineMat[1][0] * spineMat[3][3]);
            v18[1][3] = (float)((float)((float)(rightHandFromSpineMat[1][1] * spineMat[0][0])
                + (float)(rightHandFromSpineMat[1][2] * spineMat[1][0]))
                + (float)(rightHandFromSpineMat[1][3] * spineMat[2][0]))
                + (float)(rightHandFromSpineMat[2][0] * spineMat[3][0]);
            v18[2][0] = (float)((float)((float)(rightHandFromSpineMat[1][1] * spineMat[0][1])
                + (float)(rightHandFromSpineMat[1][2] * spineMat[1][1]))
                + (float)(rightHandFromSpineMat[1][3] * spineMat[2][1]))
                + (float)(rightHandFromSpineMat[2][0] * spineMat[3][1]);
            v18[2][1] = (float)((float)((float)(rightHandFromSpineMat[1][1] * spineMat[0][2])
                + (float)(rightHandFromSpineMat[1][2] * spineMat[1][2]))
                + (float)(rightHandFromSpineMat[1][3] * spineMat[2][2]))
                + (float)(rightHandFromSpineMat[2][0] * spineMat[3][2]);
            v18[2][2] = (float)((float)((float)(rightHandFromSpineMat[1][1] * spineMat[0][3])
                + (float)(rightHandFromSpineMat[1][2] * spineMat[1][3]))
                + (float)(rightHandFromSpineMat[1][3] * spineMat[2][3]))
                + (float)(rightHandFromSpineMat[2][0] * spineMat[3][3]);
            v18[2][3] = (float)((float)((float)(rightHandFromSpineMat[2][1] * spineMat[0][0])
                + (float)(rightHandFromSpineMat[2][2] * spineMat[1][0]))
                + (float)(rightHandFromSpineMat[2][3] * spineMat[2][0]))
                + (float)(rightHandFromSpineMat[3][0] * spineMat[3][0]);
            v18[3][0] = (float)((float)((float)(rightHandFromSpineMat[2][1] * spineMat[0][1])
                + (float)(rightHandFromSpineMat[2][2] * spineMat[1][1]))
                + (float)(rightHandFromSpineMat[2][3] * spineMat[2][1]))
                + (float)(rightHandFromSpineMat[3][0] * spineMat[3][1]);
            v18[3][1] = (float)((float)((float)(rightHandFromSpineMat[2][1] * spineMat[0][2])
                + (float)(rightHandFromSpineMat[2][2] * spineMat[1][2]))
                + (float)(rightHandFromSpineMat[2][3] * spineMat[2][2]))
                + (float)(rightHandFromSpineMat[3][0] * spineMat[3][2]);
            v18[3][2] = (float)((float)((float)(rightHandFromSpineMat[2][1] * spineMat[0][3])
                + (float)(rightHandFromSpineMat[2][2] * spineMat[1][3]))
                + (float)(rightHandFromSpineMat[2][3] * spineMat[2][3]))
                + (float)(rightHandFromSpineMat[3][0] * spineMat[3][3]);
            ikState->handMats[1][0][0] = (float)((float)((float)(v20 * spineMat[0][0]) + (float)(v21 * spineMat[1][0]))
                + (float)(v22 * spineMat[2][0]))
                + (float)(rightHandFromSpineMat[0][0] * spineMat[3][0]);
            *(_QWORD *)&ikState->handMats[1][0][1] = *(_QWORD *)&v18[0][0];
            *(_QWORD *)&ikState->handMats[1][0][3] = *(_QWORD *)&v18[0][2];
            *(_QWORD *)&ikState->handMats[1][1][1] = *(_QWORD *)&v18[1][0];
            *(_QWORD *)&ikState->handMats[1][1][3] = *(_QWORD *)&v18[1][2];
            *(_QWORD *)&ikState->handMats[1][2][1] = *(_QWORD *)&v18[2][0];
            *(_QWORD *)&ikState->handMats[1][2][3] = *(_QWORD *)&v18[2][2];
            *(_QWORD *)&ikState->handMats[1][3][1] = *(_QWORD *)&v18[3][0];
            ikState->handMats[1][3][3] = v18[3][2];
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
        if (IKImport_IsClient(ikState))
        {
            IK_Layer_PlayerPitch(ikState, 0);
            IK_Layer_PreventHeadClip( ikState);
        }
        IK_Layer_TerrainMapping(ikState);
        if (ikState->bHasActiveLayers)
        {
            if (ikState->bHasLegSolverLayer)
            {
                IK_Layer_ApplyFootIK(
                    ikState,
                    &ikState->localIkSystem->joints[2],
                    &ikState->jointVars[2],
                    ikState->footMats[0]);
                IK_Layer_ApplyFootIK(
                    ikState,
                    &ikState->localIkSystem->joints[3],
                    &ikState->jointVars[3],
                    ikState->footMats[1]);
            }
            if (ikState->bHasArmSolverLayer)
            {
                IK_Layer_ApplyHandIK(ikState, ikState->localIkSystem->joints, ikState->jointVars, ikState->handMats[0]);
                IK_Layer_ApplyHandIK(
                    ikState,
                    &ikState->localIkSystem->joints[1],
                    &ikState->jointVars[1],
                    ikState->handMats[1]);
            }
        }
    }
}

