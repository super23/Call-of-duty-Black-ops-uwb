#include "cg_animtree_sp.h"
#include <clientscript/cscr_main.h>
#include <clientscript/cscr_memorytree.h>
#include <client_sp/cl_cgame_sp.h>
#include <clientscript/cscr_animtree.h>
#include <universal/com_memory.h>
#include <qcommon/common.h>
#include "cg_local_sp.h"
#include <qcommon/dobj_management.h>
#include <game/g_scr_main.h>

extern bgsAnim_s cg_bgsAnim;

static scr_animtree_t CGScr_FindAnimTree(const char *treeName, int bEnforceExists)
{
    scr_animtree_t tree;

    tree.anims = Scr_FindAnimTree(SCRIPTINSTANCE_CLIENT, treeName).anims;
    if ( !tree.anims && bEnforceExists )
        Com_Error(ERR_DROP, "Could not find animtree '%s'", treeName);
    return tree;
}

// Decomp: CoDSP_rdBlackOps.map.c (CGScr_FindAnimTrees @ 82332010)
void __cdecl CGScr_FindAnimTrees(bool loading_scripts)
{
    if ( cg_bgsAnim.generic_human.tree.anims )
        return;
    if ( com_sv_running->current.enabled )
        cg_bgsAnim.generic_human.tree = Scr_HACKGetGameGenericHumanTree();
    else if ( loading_scripts )
        cg_bgsAnim.generic_human.tree = CGScr_FindAnimTree("generic_human", 1);
}

void __cdecl CGScr_LoadAnimTrees()
{
    signed int i; // [esp+14h] [ebp-8h]
    char *string; // [esp+18h] [ebp-4h]

    Scr_BeginLoadAnimTrees(SCRIPTINSTANCE_SERVER, 0);
    for ( i = 3132; i < 3148; ++i )
    {
        string = CL_GetConfigString(i);
        if ( *string )
        {
            if ( strcmp("multiplayer", string) )
            {
                Scr_ClientUsingTree(SCRIPTINSTANCE_SERVER, string);
                Scr_PrecacheAnimTrees(SCRIPTINSTANCE_SERVER, (void *(__cdecl *)(int))Hunk_AllocXAnimCreate, 0, 1);
            }
        }
    }
}

unsigned __int8 *__cdecl Hunk_AllocXAnimCreate(unsigned int size)
{
    return Hunk_AllocLow(size, "XAnimCreateAnims", 13);
}

void __cdecl CG_FreeClientDObjInfo(int localClientNum)
{
    int i; // [esp+0h] [ebp-4h]

    for ( i = 0; i < com_maxclients->current.integer; ++i )
        CG_SafeDObjFree(localClientNum, i);
}

void __cdecl CG_SetDObjInfo(int localClientNum, int iEntNum, int iEntType, XModel *pXModel)
{
    cg_s *cgameGlob; // eax

    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    cgameGlob->iEntityLastType[iEntNum] = iEntType;
    cgameGlob->pEntityLastXModel[iEntNum] = pXModel;
}

bool __cdecl CG_CheckDObjInfoMatches(int localClientNum, int iEntNum, int iEntType, XModel *pXModel)
{
    const cg_s *cgameGlob; // [esp+0h] [ebp-4h]

    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    return cgameGlob->iEntityLastType[iEntNum] == iEntType && cgameGlob->pEntityLastXModel[iEntNum] == pXModel;
}

void __cdecl CG_SafeDObjFree(int localClientNum, int entIndex)
{
    centity_s *cent; // [esp+8h] [ebp-4h]

    Com_SafeClientDObjFree(entIndex, localClientNum);
    CG_SetDObjInfo(localClientNum, entIndex, 0, 0);
    cent = CG_GetEntity(localClientNum, entIndex);
    if ( cent->tree )
    {
        if ( entIndex >= 32 )
        {
            XAnimFreeTree(cent->tree, (void (__cdecl *)(void *, int, scriptInstance_t))MT_Free, SCRIPTINSTANCE_SERVER);
            cent->tree = 0;
        }
    }
}

void __cdecl CG_FreeEntityDObjInfo(int localClientNum)
{
    int i; // [esp+0h] [ebp-4h]

    for ( i = 32; i < 1024; ++i )
        CG_SafeDObjFree(localClientNum, i);
}

