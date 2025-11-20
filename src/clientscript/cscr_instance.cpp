#include "cscr_instance.h"

void __cdecl Scr_ClearScrVarDebugPub(scriptInstance_t inst)
{
  gScrVarDebugPub[inst] = 0;
}

void __cdecl Scr_ResetScrVarDebugPub(scriptInstance_t inst)
{
  gScrVarDebugPub[inst] = &gScrVarDebugPubBuff[inst];
}

void __cdecl Scr_ClearScrStringDebugGlob(scriptInstance_t inst)
{
  gScrStringDebugGlob[inst] = 0;
}

void __cdecl Scr_ResetScrStringDebugGlob(scriptInstance_t inst)
{
  gScrStringDebugGlob[inst] = &gScrStringDebugGlobBuff[inst];
}

