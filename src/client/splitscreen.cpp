#include "splitscreen.h"
#include <universal/assertive.h>
#include <ui/ui_main.h>
#include <qcommon/com_clients.h>
#include "cl_main.h"
#include <client/client_limits.h>

int __cdecl CL_LocalClient_GetActiveCount()
{
    int activeCount;
    int localClientNum;

    activeCount = 0;
    for ( localClientNum = 0; localClientNum < MAX_LOCAL_CLIENTS; ++localClientNum )
    {
        if ( CL_LocalClient_IsActive(localClientNum) )
            ++activeCount;
    }
    return activeCount;
}

char __cdecl CL_LocalClient_IsActive(int localClientNum)
{
    if ( !Com_IsValidLocalClientNum(localClientNum) )
        return 0;
    return (clientUIActives[localClientNum].flags & 1) != 0;
}

char __cdecl CL_LocalClient_IsFirstActive(int localClientNum)
{
    if ( !Com_IsValidLocalClientNum(localClientNum) )
        return 0;
    return localClientNum == 0;
}

char __cdecl CL_AllLocalClientsDisconnected()
{
    int client;

    if ( UI_IsFullscreen(0) )
        return 1;
    for ( client = 0; client < MAX_LOCAL_CLIENTS; ++client )
    {
        if ( CL_LocalClient_IsActive(client) && clientUIActives[client].connectionState >= CA_CONNECTING )
            return 0;
    }
    return 1;
}

char __cdecl CL_AnyLocalClientStateActive()
{
    int localClientNum;

    for ( localClientNum = 0; localClientNum < MAX_LOCAL_CLIENTS; ++localClientNum )
    {
        if ( CL_LocalClient_IsActive(localClientNum) && clientUIActives[localClientNum].connectionState == CA_ACTIVE )
            return 1;
    }
    return 0;
}

char __cdecl CL_AnyLocalClientChallenging()
{
    int localClientNum;

    for ( localClientNum = 0; localClientNum < MAX_LOCAL_CLIENTS; ++localClientNum )
    {
        if ( CL_LocalClient_IsActive(localClientNum) && clientUIActives[localClientNum].connectionState == CA_CHALLENGING )
            return 1;
    }
    return 0;
}

void __cdecl CL_LocalClients_SetAllUsedActive()
{
    int localClientNum;
    bool beingUsed;

    for ( localClientNum = 0; localClientNum < MAX_LOCAL_CLIENTS; ++localClientNum )
    {
        beingUsed = Com_LocalClient_IsBeingUsed(localClientNum);
        if ( beingUsed )
            clientUIActives[localClientNum].flags |= 1u;
        else
            clientUIActives[localClientNum].flags &= ~1u;
    }
}

int __cdecl CL_LocalActiveIndexFromClientNum(int localClientNum)
{
    (void)localClientNum;
    return 0;
}

extern const dvar_t *name; // KISAKTODO: remove this extern hack after live/ folder done
const char *__cdecl CL_ControllerIndex_GetUsername()
{
    return name->current.string;
}

char __cdecl CL_IsSplitScreen()
{
    return CL_LocalClient_GetActiveCount() > 1 && CL_AnyLocalClientStateActive();
}

char __cdecl CL_IsRenderingSplitScreen()
{
    return CL_IsSplitScreen();
}
