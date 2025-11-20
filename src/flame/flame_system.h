#pragma once

void __cdecl Flame_Init_FlameVars();
void __cdecl Flame_Reset_FlameVars();
void __cdecl Flame_Init_Classes();
void __cdecl Flame_Init_Sources();
void __cdecl Flame_Init_DVars();
char __cdecl Flame_TableInDevGui(const char *tableName);
void __cdecl Flame_InitDevGui();
void __cdecl Flame_Init();
void __cdecl Flame_Reset();
void __cdecl Flame_CMD_PrintDVarsToConsol();
void __cdecl Flame_CMD_Test_Toggle();
void __cdecl Flame_CMD_Use_Dvars_Toggle();
void __cdecl Flame_DVarsToFlameVars(flameTable *fTable);
flameTable *__cdecl Flame_FindFlameTable(const char *tableName);
double __cdecl Flame_SwayRand(float x, float y, int time);
flameSource_t *__cdecl SV_Flame_Source_Get(int entityNum);
flameSource_t *__cdecl Flame_Source_Get(int entityNum);
flameRender_s *__cdecl Flame_Get_FlameRender(const char *name);
void __cdecl Flame_Item_Init(flameGeneric_s *item, unsigned int itemSize);
void __cdecl Flame_List_Init(flameGeneric_s *list, int itemSize, int listSize);
void __cdecl Flame_List_Move_Global(flameGeneric_s *item, flameGeneric_s **fromList, flameGeneric_s **toList);
void __cdecl Flame_List_Add_Local(flameGeneric_s *item, flameGeneric_s **localList);
void __cdecl Flame_List_Sub_Local(flameGeneric_s *item, flameGeneric_s **localList);
void __cdecl CG_Flame_Age_All_Objects(int time);
void __cdecl SV_Flame_Age_All_Objects(int time);
flameSource_t *__cdecl Flame_Source_Alloc(int entityNum);
flameSource_t *__cdecl SV_Flame_Source_Alloc(int entityNum);
void __cdecl Flame_Render_Sprite(
        cg_s *clientGlobals,
        Material *material,
        flameGeneric_s *flameGeneric,
        float alpha,
        float frame,
        unsigned __int8 col);
void  Flame_Render_Sprites(
        float a1@<ebp>,
        cg_s *clientGlobals,
        Material *material,
        flameGeneric_s **flameGenericList,
        int numItems);
int __cdecl Flame_GetLocalClientFlameSource(int localClientNum, int EntNum);
unsigned __int8 __cdecl Flame_GetLocalClientSourceRange();
void __cdecl Flame_Source_Update(flameSource_t *source, flameWeaponConfig_t *weaponConfig);
void __cdecl SV_Flame_Update_Source_Internal(
        flameSource_t *source,
        int entityNum,
        flameWeaponConfig_t *weaponConfig,
        int time);
void __cdecl CG_Flame_Update_Source_Internal(
        flameSource_t *source,
        int entityNum,
        flameWeaponConfig_t *weaponConfig,
        int time,
        flameRender_s *flameRend);
void __cdecl SV_Flame_Update_Source(int entityNum, flameWeaponConfig_t *weaponConfig, int time);
void __cdecl CG_Flame_Update_Source(int localClientNum);
void __cdecl Flame_Generate_Verts(int localClientNum);
void __cdecl CG_Flame_Update_ViewModel(int localClientNum, centity_s *cent);
double __cdecl Flame_Random(bool is_server);
double __cdecl Flame_CRandom(bool is_server);
double __cdecl Flame_CalcTimeScale(int startTime, int endTime);
double __cdecl Flame_CalcInvStartSpeed(float invInitialSpeed, float speedScale);
