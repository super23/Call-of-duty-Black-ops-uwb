#include "sv_live_stats.h"

#include <live/live_storage.h>
#include <server_mp/sv_main_mp.h>

// CoDMPServer.c @ 0x712460 — find a connected client slot for a Demonware user id.
int __cdecl SV_GetClientNumForBdOnlineUserID(unsigned __int64 uid)
{
    for (int clientNum = 0; clientNum < com_maxclients->current.integer; ++clientNum)
    {
        const client_t *cl = &svs.clients[clientNum];

        if (cl->header.state > CS_ZOMBIE && cl->dw_userID == uid)
            return clientNum;
    }
    return -1;
}

// CoDMPServer.c @ 0x7124F0 — ownerID from dwFileOperationInfo (retail used *((_QWORD *)data + 34)).
unsigned __int64 __cdecl SV_GetBdUidFromFileInfo(void *fileInfo)
{
    return static_cast<const dwFileOperationInfo *>(fileInfo)->ownerID;
}

// CoDMPServer.c @ 0x712510 — resolve dwFileOperationInfo payload to an in-game client, if still connected.
client_t *__cdecl SV_GetClientFromFileInfo(void *fileInfo)
{
    client_t *client = nullptr;

    if (!fileInfo)
        return nullptr;

    const unsigned __int64 uid = SV_GetBdUidFromFileInfo(fileInfo);

    Com_DPrintf(15, "Read uid %llu, finding client\n", uid);

    const int clientNum = SV_GetClientNumForBdOnlineUserID(uid);
    if (clientNum < 0)
    {
        Com_DPrintf(15, "Warning: Couldn't find matching client for bdOnlineUserId %llu\n", uid);
        return nullptr;
    }

    if ((unsigned int)clientNum >= (unsigned int)com_maxclients->current.integer
        && !Assert_MyHandler(
                "src/server/sv_live_stats.cpp",
                49,
                0,
                "clientNum doesn't index com_maxclients->current.integer\n\t%i not in [0, %i)",
                clientNum,
                com_maxclients->current.integer))
    {
        __debugbreak();
    }

    client = &svs.clients[clientNum];
    return client;
}

// CoDMPServer.c @ 0x7125E0
unsigned __int64 __cdecl SV_GetPlayerXuid(unsigned int clientNum)
{
    if (clientNum >= (unsigned int)com_maxclients->current.integer
        && !Assert_MyHandler(
                "src/server/sv_live_stats.cpp",
                64,
                0,
                "clientNum doesn't index com_maxclients->current.integer\n\t%i not in [0, %i)",
                clientNum,
                com_maxclients->current.integer))
    {
        __debugbreak();
    }
    return svs.clients[clientNum].dw_userID;
}

static_assert(offsetof(dwFileOperationInfo, ownerID) == 34 * sizeof(unsigned __int64),
    "SV_GetBdUidFromFileInfo must read dwFileOperationInfo::ownerID");
