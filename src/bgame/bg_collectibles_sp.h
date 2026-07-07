#pragma once



#include <universal/q_shared.h>



struct dvar_s;



// Decomp: CoDSP_rdBlackOps.map.c — bg_collectibles_coop.cpp



struct CollectibleGroupMap

{

    const char *groupName;

    int startIndex;

    int endIndex;

};



static const int kMaxCollectibleIndex = 45;

static const int kCollectibleStringLen = 50;

static const int kCollectibleGroupMapCount = 15;



extern const dvar_s *bg_collectibles;



int __cdecl BG_GetCollectibleGroupValue(unsigned __int8 *collectibleData);

bool __cdecl BG_HasCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, int a5, int a6, __int64 a7);

int __cdecl BG_SetCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, __int64 a5);

int __cdecl BG_UnsetCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, __int64 a5);

int __cdecl BG_GiveAllCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, __int64 a5);

void BG_ClearCollectibles();

char *BG_GetDefaulCollectiblesStr();

const dvar_s *BG_Collectibles_RegisterDvars();


