#pragma once
#include <bgame/bg_local.h>
#include <cstdint>
#include <universal/assertive.h>
#include <universal/q_shared.h>

enum DemoType : __int32
{                                       // XREF: cg_s/r
                                        // ?CG_DrawActiveFrame@@YAHHHW4DemoType@@W4CubemapShot@@HH@Z/r ...
    DEMO_TYPE_NONE            = 0x0,
    DEMO_TYPE_CLIENT          = 0x1,
    DEMO_TYPE_SERVER          = 0x2,
    DEMO_TYPE_SERVER_SNAPSHOT = 0x3,
};

enum CubemapShot : __int32
{                                       // XREF: cg_s/r
                                        // ?CG_DrawActiveFrame@@YAHHHW4DemoType@@W4CubemapShot@@HH@Z/r ...
    CUBEMAPSHOT_NONE  = 0x0,
    CUBEMAPSHOT_RIGHT = 0x1,
    CUBEMAPSHOT_LEFT  = 0x2,
    CUBEMAPSHOT_BACK  = 0x3,
    CUBEMAPSHOT_FRONT = 0x4,
    CUBEMAPSHOT_UP    = 0x5,
    CUBEMAPSHOT_DOWN  = 0x6,
    CUBEMAPSHOT_COUNT = 0x7,
};

struct snapshot_s // sizeof=0x20200
{                                       // XREF: cg_s/r snapshot_t/r
    int snapFlags;
    int ping;
    int serverTime;
    int physicsTime;
    playerState_s ps;
    int numEntities;
    int numClients;
    entityState_s entities[512];
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    clientState_s clients[32];
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    MatchState matchState;
    int serverCommandSequence;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};


struct __declspec(align(128)) cg_s // sizeof=0x71C80
{                                       // XREF: cg_t/r
    int clientNum;
    int localClientNum;
    DemoType demoType;
    CubemapShot cubemapShot;
    int cubemapSize;
    int cameraMode;
    int hiResShotMode;
    int renderScreen;
    int latestSnapshotNum;
    int latestSnapshotTime;
    snapshot_s *snap;
    snapshot_s *nextSnap;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    snapshot_s activeSnapshots[2];
    float frameInterpolation;
    int frametime;
    int time;
    int oldTime;
    int physicsTime;
    int mapRestart;
    thirdPersonType renderingThirdPerson;
    playerState_s predictedPlayerState;
    int lastPlayerStateOverride;
    centity_s predictedPlayerEntity;
    playerEntity_t playerEntity;
    int predictedErrorTime;
    float predictedError[3];
    float landChange;
    int landTime;
    float heightToCeiling;
    int heightToCeilingTS;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    refdef_s refdef;
    float refdefViewAngles[3];
    Camera cameraData;
    float lastPelvisCameraOffset;
    float baseGunAngles[3];
    float swayViewAngles[3];
    float swayAngles[3];
    float swayOffset[3];
    float recoilAngles[3];
    float recoilSpeed[3];
    float movieCameraOrigin[3];
    float movieCameraAngles[3];
    float movieCameraVelocity[3];
    int movieCameraLastInsideHeliPatch;
    float radiantCameraOrigin[3];
    float radiantCameraAngles[3];
    int radiantCamInUse;
    int followCameraClient;
    int iEntityLastType[1536];
    XModel *pEntityLastXModel[1536];
    float zoomSensitivity;
    bool isLoading;
    char objectiveText[1024];
    // padding byte
    // padding byte
    // padding byte
    int vehicleInitView;
    float prevVehicleInvAxis[3][3];
    bool vehicleViewLocked;
    // padding byte
    // padding byte
    // padding byte
    float vehicleViewLockedAngles[3];
    float storedRemoteCameraAngles[3];
    float storedRemoteCameraOrigin[3];
    char scriptMainMenu[256];
    int numScores;
    int teamScores[4];
    int teamPings[4];
    int teamPlayers[4];
    int scoreOrder[32];
    int scoreLimit;
    scoreboardColumnType_t scoreboardColumnTypes[4];
    int matchUIVisibilityFlags;
    unsigned int talkFlags;
    int bombTimer[2];
    int showScores;
    int scoreFadeTime;
    int scoresTop;
    int scoresOffBottom;
    int scoresBottom;
    int drawHud;
    int timeScaleTimeStart;
    int timeScaleTimeEnd;
    float timeScaleStart;
    float timeScaleEnd;
    int crosshairClientNum;
    int crosshairClientLastTime;
    int crosshairClientStartTime;
    int identifyClientNum;
    int cursorHintIcon;
    int cursorHintTime;
    int cursorHintFade;
    int cursorHintString;
    int vehicleHintTime;
    int lastVehicleSeat;
    int driverHintShown;
    int gunnerHintShown;
    int lastClipFlashTime;
    InvalidCmdHintType invalidCmdHintType;
    int invalidCmdHintTime;
    SuccessfulCmdHintType successfulCmdHintType;
    int successfulCmdHintTime;
    int lastHealthPulseTime;
    int lastHealthLerpDelay;
    int lastHealthClient;
    float lastHealth;
    float healthOverlayFromAlpha;
    float healthOverlayToAlpha;
    int healthOverlayPulseTime;
    int healthOverlayPulseDuration;
    int healthOverlayPulsePhase;
    bool healthOverlayHurt;
    // padding byte
    // padding byte
    // padding byte
    int healthOverlayLastHitTime;
    float healthOverlayOldHealth;
    int healthOverlayPulseIndex;
    int proneBlockedEndTime;
    int lastStance;
    int lastStanceChangeTime;
    int lastStanceFlashTime;
    int voiceTime;
    unsigned int weaponSelect;
    int weaponSelectTime;
    bool weaponForcedSelect;
    // padding byte
    // padding byte
    // padding byte
    unsigned int weaponLatestPrimaryIdx;
    unsigned __int16 lastWeaponForAlt[15];
    // padding byte
    // padding byte
    int prevViewmodelWeapon;
    int equippedOffHand;
    viewDirectionalHitIndicator_t directionalHitIndicator[4];
    viewDamage_t viewDamage[8];
    int damageTime;
    float damageX;
    float damageY;
    float damageValue;
    float viewFade;
    int waterDropCount;
    int waterDropStartTime;
    int waterDropStopTime;
    int weapIdleTime;
    int nomarks;
    int clearMarks;
    int newPlayerViewmodel;
    int v_dmg_time;
    float v_dmg_pitch;
    float v_dmg_roll;
    float fBobCycle;
    float xyspeed;
    float kickAVel[3];
    float kickAngles[3];
    float offsetAngles[3];
    float gunPitch;
    float gunYaw;
    float gunXOfs;
    float gunYOfs;
    float gunZOfs;
    float flamethrowerKickOffset[3];
    float minigunSpinAngle;
    float viewModelAxis[4][3];
    float rumbleScale;
    float compassNorthYaw;
    float compassNorth[2];
    Material *compassMapMaterial;
    float compassMapUpperLeft[2];
    float compassMapWorldSize[2];
    int compassLastTime;
    float compassYaw;
    float compassSpeed;
    int compassFadeTime;
    int healthFadeTime;
    int ammoFadeTime;
    int stanceFadeTime;
    int sprintFadeTime;
    int offhandFadeTime;
    int offhandFlashTime;
    int waypointFadeTime;
    int vehicleControlsFadeTime;
    shellshock_t shellshock;
    cg_s::<unnamed_type_testShock> testShock;
    int shellshockSoundActive;
    int holdBreathTime;
    int holdBreathInTime;
    int holdBreathDelay;
    float holdBreathFrac;
    float radarProgress;
    float satelliteProgress;
    int radarType;
    float selectedLocation[2];
    float selectedYaw[2];
    int typewriterStartTime;
    float selectedLocations[3][2];
    int lastTimeSelected;
    SprintState sprintStates;
    int adsViewErrorDone;
    int inKillCam;
    int wasInKillCam;
    bool killCamFirstFrameRan;
    // padding byte
    // padding byte
    // padding byte
    int killCamEntity;
    int invalidKillCamEntity;
    KillCamEntityType killCamEntityType;
    int killCamLastEntityNum;
    float killCamLastEntityOrg[3];
    float killCamLastEntityAngles[3];
    KillCamEntityRestState killCamEntityRestState;
    int killCamLookAtEntity;
    float killCamLookAt[3];
    float killCamHelicopterOffset[3];
    __int16 killCamLastVehicleInfoIndex;
    // padding byte
    // padding byte
    int killCamStoppedTime;
    float killCamStoppedDecelTime;
    float killCamStoppedPos[3];
    float killCamStoppedVel[3];
    float killCamPrevBombOrigin[3];
    int killCamLerpEndTime;
    float killCamOldViewAngles[3];
    float killCamOldViewOrg[3];
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    bgs_t bgs;
    cpose_t viewModelPose;
    visionSetVars_t visionSetPreLoaded[8];
    char visionSetPreLoadedName[8][64];
    visionSetVars_t visionSetFrom[6];
    visionSetVars_t visionSetTo[6];
    visionSetVars_t visionSetCurrent[6];
    visionSetLerpData_t visionSetLerpData[6];
    char visionNameNaked[64];
    char visionNameNight[64];
    char visionNameFlare[64];
    char visionNameInfrared[64];
    char visionNameTvguided[64];
    char visionNameExtraCam[64];
    bitarray<51> extraButton_bits;
    int lastActionSlotTime;
    bool playerTeleported;
    // padding byte
    // padding byte
    // padding byte
    float flareRatioLastFrame;
    int stepViewStart;
    float stepViewChange;
    cg_s::<unnamed_type_lastFrame> lastFrame;
    int oldCommandTime;
    float oldOrigin[3];
    float oldViewangles[3];
    transPlayerState_t oldTransPlayerState;
    hudElemSoundInfo_t hudElemSound[32];
    int nextRippleTime;
    unsigned int impactEffectsNext;
    unsigned int impactEffects[256];
    float warDesiredMomentumProgress;
    float warCurrentMomentumProgress;
    int warMomentumMultiplier;
    int warMomentumBlitzkriegTime;
    int warMomentumFlashTime;
    float gunnerPitchOffset;
    unsigned int centInNextSnapshot[32];
    centity_s *slideFakeEnt;
    int cameraLinkedEntitiesCount;
    int cameraLinkedEntities[4];
    link_type_e cameraLinkedEntitiesType[4];
    int groundTiltEntNum;
    int poisoned;
    Material *genericMaterialMap[64];
    bool genericFilterPassEnabled[3][16];
    Material *genericFilterPassMaterial[3][16];
    int genericFilterPassTarget[3][16];
    int genericFilterPassSampler0[3][16];
    int genericFilterPassSampler1[3][16];
    float genericFilterPassParam[3][16][16];
    int genericFilterPassQuads[3][16];
    float genericFilterPassFlareOcclusion[3][16];
    bool genericOverlayEnabled[3];
    // padding byte
    Material *genericOverlayMaterial[3];
    float genericOverlayParam[3][16];
    int genericOverlaySortIndex[3];
    float genericOverlayParamADS[8];
    bool genericOverlayEnableADS;
    // padding byte
    // padding byte
    // padding byte
    int swimming;
    int swimmingStartTime;
    int missileViewWobbleLastTime;
    float missileViewWobbleFreq[3];
    float missileViewWobbleAmp[3];
    int counterSpinTime;
    float counterSpinTarget;
    float counterSpinAngle;
    unsigned __int8 weaponTracerCount[1024];
    int extraCamEntity;
    int extraCamActive;
    int extraCamStatic;
    float extraCamOrigin[3];
    float extraCamAngles[3];
    int hasLocalRadar;
    float localRadarPos[2];
    float localRadarProgress;
    int globalScramblerActive;
    float scramblerEnemyAlpha;
    float scramblerFriendlyAlpha;
    int popUpSwayStartTime;
    float popupRotationAngle;
    float popUpAngleDelta;
    CompassScrambler compassScrambler[16];
    centity_s *closestEnemyScrambler;
    int alliesFlagAway;
    int axisFlagAway;
    int resetWeaponStateFlag;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};
static_assert(sizeof(cg_s) == 0x71C80);


centity_s *cg_entitiesArray[1];
fake_centity_s *cg_fakeEntitiesArray;

inline centity_s *__cdecl CG_GetEntity(int32_t localClientNum, uint32_t entityIndex)
{
    iassert(localClientNum == 0);
    bcassert(entityIndex, MAX_LOCAL_CENTITIES);

    if (entityIndex < 1024)
        return &cg_entitiesArray[localClientNum][entityIndex];
    else
        return &cg_fakeEntitiesArray[512 * localClientNum - 1024 + entityIndex].cent;
}

inline cg_s *CG_GetLocalClientGlobals(int32_t localClientNum)
{
    iassert(localClientNum == 0);

    return &cgArray[localClientNum];
}

inline cgs_t *CG_GetLocalClientStaticGlobals(int32_t localClientNum)
{
    iassert(localClientNum == 0);

    return &cgsArray[localClientNum];
}

inline weaponInfo_s *__cdecl CG_GetLocalClientWeaponInfo(int localClientNum, int weaponIndex)
{
    iassert(localClientNum == 0);

    return &cg_weaponsArray[localClientNum][weaponIndex];
}

