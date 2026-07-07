#pragma once
#include <xanim/dobj.h>
#include <xanim/xanim.h>

#include <cgame/cg_weapons.h>
#include <client/client_limits.h>

struct ViewModelHand // sizeof=0x8
{                                       // XREF: ViewModelInfo/r
    int iPrevAnim;
    int iHandAnimIndex;
};

struct ViewModelInfo // sizeof=0x34
{
    DObj *viewModelDObj;
    int hasAnimTree;
    struct XAnimTree_s *tree;
    XAnim_s *anims;
    unsigned int partBits[5];
    ViewModelHand hand[2];
};

extern ViewModelInfo *cg_viewModelArray;
extern weaponInfo_s *cg_weaponsArray[MAX_LOCAL_CLIENTS];

inline ViewModelInfo *__cdecl CG_GetLocalClientViewModelInfo(int localClientNum)
{
    (void)localClientNum;
    return cg_viewModelArray;
}


inline weaponInfo_s *__cdecl CG_GetLocalClientWeaponInfo(int localClientNum, int weaponIndex)
{
    return &cg_weaponsArray[Com_ClampLocalClientNum(localClientNum)][weaponIndex];
}
