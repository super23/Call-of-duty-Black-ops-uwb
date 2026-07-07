#pragma once
#include "cl_main.h"
#include <client/client_limits.h>
#include <universal/assertive.h>
#ifdef KISAK_SP
#include <client_sp/cl_main_sp.h>
#else
#include <client_mp/cl_main_mp.h>
#endif


inline connstate_t __cdecl CL_GetLocalClientConnectionState(int localClientNum)
{
    return clientUIActives[Com_ClampLocalClientNum(localClientNum)].connectionState;
}

extern clientActive_t *clients;
inline clientActive_t *__cdecl CL_GetLocalClientGlobals(int localClientNum)
{
    return &clients[Com_ClampLocalClientNum(localClientNum)];
}