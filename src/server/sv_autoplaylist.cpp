#include "sv_autoplaylist.h"

#include <client_mp/cl_main_mp.h>
#include <DW/dwUtils.h>
#include <live/live_storage_pub.h>
#include <live/live_storage_win.h>
#include <qcommon/cmd.h>
#include <qcommon/common.h>
#include <server_mp/sv_init_mp.h>
#include <server_mp/sv_main_mp.h>
#include <server_mp/sv_main_pc_mp.h>
#include <universal/com_tasks.h>
#include <universal/dvar.h>
#include <universal/q_parse.h>

#include <string.h>

// Playlist target vs actual server counts (sorted by diff for weighted pick).
struct goaldiff_t
{
    int diff;
    int playlist;
};

// bdGroup::getGroupCounts result entry (layout matches retail bdGroupCount).
struct bdGroupCountEntry
{
    void *vfptr;
    unsigned int m_groupID;
    unsigned int m_groupCount;
};

sv_apstate_t s_apstate = AP_SLEEPING;

dwFileOperationInfo s_finfo;

int s_probabilities[64];

int s_geogroupid = -1;
bool firstTimeRunning = true;

char s_controlFileName[32];
unsigned char s_controlFileBuffer[1024];

int s_numgroupErrors;
int s_aptimeout;

TaskRecord *s_groupgetTask;
TaskRecord *s_controlFileTask;

bdGroupCountEntry s_bdGroupCounts[128];

static bool cachedFull;
static bool cachedFull_serverFull;

#ifdef KISAK_LIVE
const TaskDefinition task_SVSetGroups[1] =
{
    { 2uLL, "SVSetGroups", 0, &SV_SetGroupCountsComplete, &SV_GroupsFailure, NULL }
};

const TaskDefinition task_getGroupCounts[1] =
{
    { 2uLL, "SVGetGroupCounts", 0, &SV_GetGroupCountsComplete, &SV_GroupsFailure, NULL }
};

// Wraps LiveStorage_ReadDWFile; completion runs via s_finfo.fileOperationSucessFunction.
const TaskDefinition task_SVFetchControlFile[1] =
{
    { 8uLL, "SVFetchControlFile", 0, NULL, NULL, NULL }
};
#endif

void __cdecl SV_AP_DumpTable()
{
    int playlistIndex;

    if (!Dvar_GetBool("sv_ap_debug"))
        return;

    Com_Printf(15, "\n**************************************\nPlaylist\t\tProbability\n");
    for (playlistIndex = 0; playlistIndex < 64; ++playlistIndex)
        Com_Printf(15, "%i\t\t%u\n", playlistIndex, s_probabilities[playlistIndex]);
    Com_Printf(15, "\n**************************************\n");
}

bool __cdecl SV_AP_ParseControlFile(unsigned __int8 *controlFile)
{
    parseInfo_t *probToken;
    int chosenPlaylist;
    unsigned int probability;
    int playlist;
    parseInfo_t *token;
    int totalProb;
    int numPlaylists;
    const char *parsePtr;
    bool success;
    int line;

    parsePtr = (const char *)controlFile;
    numPlaylists = 0;
    totalProb = 0;
    line = 0;
    success = 0;

    Com_BeginParseSession("AutoPlaylist");
    Com_SetSpaceDelimited(0);
    Com_SetCSV(1);

    while (1)
    {
        ++line;
        token = Com_Parse(&parsePtr);
        if (!token || !token->token[0])
            break;

        if (token->token[0] == '#')
        {
            Com_SkipRestOfLine(&parsePtr);
            continue;
        }

        playlist = atoi(token->token);
        if (playlist <= 0 || playlist >= 64)
        {
            if (Dvar_GetBool("sv_ap_fatal"))
                Com_Error(ERR_DROP, "AP error: %s\n", va("Failed parsing control file at line %i, playlist %i failed sanity check\n", line, playlist));
            else
                Com_PrintError(15, "AP Error: %s\n", va("Failed parsing control file at line %i, playlist %i failed sanity check\n", line, playlist));
            break;
        }

        probToken = Com_Parse(&parsePtr);
        probability = atoi(probToken->token);
        if ((int)probability < 0 || probability > 100)
        {
            if (Dvar_GetBool("sv_ap_fatal"))
                Com_Error(ERR_DROP, "AP error: %s\n", va("Failed parsing control file at line %i, probability %i failed sanity check\n", line, probability));
            else
                Com_PrintError(15, "AP Error: %s\n", va("Failed parsing control file at line %i, probability %i failed sanity check\n", line, probability));
            break;
        }

        s_probabilities[playlist] = (unsigned char)probability;
        totalProb += probability;

        if (++numPlaylists == 64)
        {
            if (Dvar_GetBool("sv_ap_fatal"))
                Com_Error(ERR_DROP, "AP error: %s\n", "Too many playlists, truncating!\n");
            else
                Com_PrintError(15, "AP Error: %s\n", "Too many playlists, truncating!\n");
            break;
        }

        if (totalProb > 100)
        {
            if (Dvar_GetBool("sv_ap_fatal"))
                Com_Error(ERR_DROP, "AP error: %s\n", va("We've exceeded 100%% total probability on line %i\n", line));
            else
                Com_PrintError(15, "AP Error: %s\n", va("We've exceeded 100%% total probability on line %i\n", line));
            break;
        }
    }

    Com_EndParseSession();

    if (totalProb != 100)
    {
        if (Dvar_GetBool("sv_ap_fatal"))
            Com_Error(ERR_DROP, "AP error: %s\n", va("Finished parsing, total probability is %i, should be 100\n", totalProb));
        else
            Com_PrintError(15, "AP Error: %s\n", va("Finished parsing, total probability is %i, should be 100\n", totalProb));
        return success;
    }

    if (Dvar_GetBool("sv_ap_debug"))
        Com_Printf(15, "AP: %s\n", va("Successfully parsed %i playlist probabilities, %i lines\n", numPlaylists, line));

    success = 1;
    SV_AP_DumpTable();

    chosenPlaylist = SV_AP_PlaylistFromDistribution();
    if (Dvar_GetBool("sv_ap_debug"))
        Com_Printf(15, "AP: %s\n", va("Choosing playlist %i\n", chosenPlaylist));

    Dvar_SetIntByName("playlist", chosenPlaylist);
    Cbuf_AddText(0, "map_rotate\n");
    return success;
}

int __cdecl SV_AP_PlaylistFromDistribution()
{
#ifdef KISAK_LIVE
    const char *debugMsg;
    bdTrulyRandomImpl *random;
    unsigned int randomIndex;
    int sortIndex;
    int playlistIndex;
    int probIndex;
    int serverIndex;
    int weightedProbs[64];
    int chosenPlaylist;
    int activePlaylistCount;
    int totalChoiceWeight;
    int totalServersInGeo;
    int choiceTable[101];
    int choiceCount;
    goaldiff_t playlistGoals[64];

    totalServersInGeo = 0;
    memset(weightedProbs, 0, sizeof(weightedProbs));
    activePlaylistCount = 0;
    totalChoiceWeight = 0;

    for (serverIndex = 0; serverIndex < 64; ++serverIndex)
    {
        totalServersInGeo += s_bdGroupCounts[serverIndex].m_groupCount;
        if (s_probabilities[serverIndex] > 0)
            ++activePlaylistCount;
    }

    if (firstTimeRunning)
        ++totalServersInGeo;

    if (Dvar_GetBool("sv_ap_debug"))
    {
        debugMsg = va("%i total servers in geogroup\n", totalServersInGeo);
        Com_Printf(15, "AP: %s\n", debugMsg);
    }

    if (totalServersInGeo <= activePlaylistCount)
    {
        memcpy(weightedProbs, s_probabilities, sizeof(weightedProbs));
        totalChoiceWeight = 100;
    }
    else
    {
        for (playlistIndex = 0; playlistIndex < 64; ++playlistIndex)
        {
            playlistGoals[playlistIndex].diff = (int)((double)totalServersInGeo * ((double)s_probabilities[playlistIndex] / 100.0))
                - (int)s_bdGroupCounts[playlistIndex].m_groupCount;
            playlistGoals[playlistIndex].playlist = playlistIndex;
        }

        qsort(playlistGoals, 64, sizeof(goaldiff_t), comparePlaylists);

        for (sortIndex = 0; sortIndex < 64; ++sortIndex)
        {
            if (playlistGoals[sortIndex].diff > 0)
            {
                weightedProbs[playlistGoals[sortIndex].playlist] =
                    (int)((double)playlistGoals[sortIndex].diff / (double)totalServersInGeo * 100.0);
                totalChoiceWeight += weightedProbs[playlistGoals[sortIndex].playlist];
            }
        }
    }

    if (totalChoiceWeight || firstTimeRunning)
    {
        if (!totalChoiceWeight && firstTimeRunning)
        {
            memcpy(weightedProbs, s_probabilities, sizeof(weightedProbs));
            for (sortIndex = 0; sortIndex < 64; ++sortIndex)
            {
                if (playlistGoals[sortIndex].diff < 0)
                    weightedProbs[sortIndex] = 0;
                totalChoiceWeight += weightedProbs[playlistGoals[sortIndex].playlist];
            }
            if (!totalChoiceWeight)
                totalChoiceWeight = 100;
        }

        choiceCount = 0;
        memset(choiceTable, 0, sizeof(choiceTable));

        for (probIndex = 0; probIndex < 64; ++probIndex)
        {
            if (choiceCount > totalChoiceWeight
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server_mp\\sv_autoplaylist.cpp",
                    184,
                    0,
                    "%s",
                    "count <= numchoices"))
            {
                __debugbreak();
            }

            while (weightedProbs[probIndex] > 0)
            {
                choiceTable[choiceCount++] = probIndex;
                --weightedProbs[probIndex];
            }
        }

        firstTimeRunning = 0;
        random = bdSingleton<bdTrulyRandomImpl>::getInstance();
        randomIndex = bdTrulyRandomImpl::getRandomUInt(random) % (unsigned int)totalChoiceWeight;
        return choiceTable[randomIndex];
    }

    firstTimeRunning = 0;
    return playlist->current.integer;
#else
    return playlist->current.integer;
#endif
}

int __cdecl comparePlaylists(const void *p1, const void *p2)
{
    const goaldiff_t *entryA = (const goaldiff_t *)p1;
    const goaldiff_t *entryB = (const goaldiff_t *)p2;
    return entryB->diff - entryA->diff;
}

void __cdecl SV_AP_GetControlFileComplete()
{
    if (Dvar_GetBool("sv_ap_debug"))
        Com_Printf(15, "AP: %s\n", "Received control file, attempting to parse\n");
    operator++(s_apstate);
}

int __cdecl SV_AP_GetControlFileFailure()
{
    if (Dvar_GetBool("sv_ap_fatal"))
        Com_Error(ERR_DROP, "AP error: %s\n", "Couldn't get control file.\n");
    else
        Com_PrintError(15, "AP Error: %s\n", "Couldn't get control file.\n");
    return 1;
}

TaskRecord *__cdecl SV_AP_GetControlFile()
{
#ifdef KISAK_LIVE
    TaskRecord *nestedTask;
    TaskRecord *task;

    task = 0;
    SV_AP_GetControlFileName(s_controlFileName, 32);
    s_finfo.isUserFile = 0;
    s_finfo.isCompressedFile = 0;
    s_finfo.fileTask.m_filename = s_controlFileName;
    s_finfo.fileBuffer = s_controlFileBuffer;
    s_finfo.bufferSize = 1024;
    s_finfo.fileOperationSucessFunction = (void (__cdecl *)(const int, void *))SV_AP_GetControlFileComplete;
    s_finfo.fileNotFoundFunction = (taskCompleteResults (__cdecl *)(const int, void *))SV_AP_GetControlFileFailure;
    nestedTask = LiveStorage_ReadDWFile(0, &s_finfo);
    if (nestedTask)
    {
        task = TaskManager2_CreateTask(task_SVFetchControlFile, 0, nestedTask, 0);
        if (task)
        {
            TaskManager2_StartTask(task);
        }
        else if (Dvar_GetBool("sv_ap_fatal"))
        {
            Com_Error(
                ERR_DROP,
                "AP error: %s\n",
                "Couldn't start fetch control file nested task. Probable connectivity issue\n");
        }
        else
        {
            Com_PrintError(
                15,
                "AP Error: %s\n",
                "Couldn't start fetch control file nested task. Probable connectivity issue\n");
        }
    }
    else if (Dvar_GetBool("sv_ap_fatal"))
    {
        Com_Error(ERR_DROP, "AP error: %s\n", "Couldn't start fetch control file dw task. Probable connectivity issue\n");
    }
    else
    {
        Com_PrintError(15, "AP Error: %s\n", "Couldn't start fetch control file dw task. Probable connectivity issue\n");
    }
    return task;
#else
    return NULL;
#endif
}

void __cdecl SV_AP_GetControlFileName(char *buf, int buflen)
{
#ifdef KISAK_LIVE
    const char *filename;

    if ((!sv_geolocation || !sv_geolocation->current.string[0])
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\server_mp\\sv_autoplaylist.cpp",
            299,
            0,
            "%s",
            "sv_geolocation && sv_geolocation->current.string[0]"))
    {
        __debugbreak();
    }

    if (sv_geolocation && sv_geolocation->current.string[0])
    {
        filename = va("%s%s", sv_geolocation->current.string, ".csv");
        I_strncpyz(buf, filename, buflen);
    }
    else if (Dvar_GetBool("sv_ap_fatal"))
    {
        Com_Error(
            ERR_DROP,
            "AP error: %s\n",
            "sv_geolocation isn't set. This has to be set for the autoplaylist tech to work\n");
    }
    else
    {
        Com_PrintError(
            15,
            "AP Error: %s\n",
            "sv_geolocation isn't set. This has to be set for the autoplaylist tech to work\n");
    }
#endif
}

void __cdecl SV_SetGroupCountsComplete()
{
#ifdef KISAK_LIVE
    Com_Printf(15, "Group set complete\n");
    s_apstate = AP_SLEEPING;
    s_numgroupErrors = 0;
    if (!Dvar_GetBool("sv_ap_enabled") && SV_ShouldMapRotate())
    {
        SV_SetShouldMapRotate(0);
        Cbuf_AddText(0, "map_rotate\n");
    }
#endif
}

void __cdecl SV_GetGroupCountsComplete()
{
#ifdef KISAK_LIVE
    Com_Printf(15, "Groups get complete\n");
    s_numgroupErrors = 0;
    if (!LiveStorage_DoWeHavePlaylists()
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\server_mp\\sv_autoplaylist.cpp",
            388,
            0,
            "%s",
            "LiveStorage_DoWeHavePlaylists()"))
    {
        __debugbreak();
    }
    operator++(s_apstate);
#endif
}

void __cdecl SV_GroupsFailure(TaskRecord *task)
{
#ifdef KISAK_LIVE
    enum bdLobbyErrorCode errorCode;

    if (task->remoteTask.m_ptr)
        errorCode = bdRemoteTask::getErrorCode(task->remoteTask.m_ptr);
    else
        errorCode = (enum bdLobbyErrorCode)-1;
    SV_GroupError("Remote async task failed: %i", errorCode);
#endif
}

void SV_GroupError(const char *fmt, ...)
{
    char msg[132];
    va_list va;

    va_start(va, fmt);
    _vsnprintf(msg, 0x80u, fmt, va);
    va_end(va);
    Com_PrintError(15, "GROUPS: %s\n", msg);
}

TaskRecord *__cdecl SV_GetGroupCounts()
{
#ifdef KISAK_LIVE
    const bdReference<bdCommonAddr> *groupCountsTask;
    bdReference<bdCommonAddr> remoteTaskRef;
    int groupIdIndex;
    int nextGroupId;
    TaskRecord *task;
    unsigned int groupIds[129];
    bdGroup *group;

    task = TaskManager2_CreateTask(task_getGroupCounts, 0, 0, 0);
    group = dwGetGroup(0);
    memset(groupIds, 0, sizeof(groupIds));
    nextGroupId = s_geogroupid;
    for (groupIdIndex = 0; groupIdIndex < 128; ++groupIdIndex)
        groupIds[groupIdIndex] = nextGroupId++;
    groupCountsTask = (const bdReference<bdCommonAddr> *)bdGroup::getGroupCounts(
        group,
        (int)&remoteTaskRef,
        (int)groupIds,
        0x80u,
        (int)&s_bdGroupCounts,
        0x80u);
    bdReference<bdCommonAddr>::operator=((bdReference<bdCommonAddr> *)&task->remoteTask, groupCountsTask);
    bdReference<bdRemoteTask>::~bdReference<bdRemoteTask>(&remoteTaskRef);
    TaskManager2_StartTask(task);
    return task;
#else
    return NULL;
#endif
}

void __cdecl SV_Groups_SetGroupMembership(bool full)
{
#ifdef KISAK_LIVE
    int licenseType;
    const bdReference<bdCommonAddr> *setGroupsTask;
    bdReference<bdCommonAddr> remoteTaskRef;
    TaskRecord *task;
    int numgroups;
    unsigned int groupIDs[3];
    bdGroup *group;

    if (!playlist
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server_mp\\sv_autoplaylist.cpp", 420, 0, "%s", "playlist"))
    {
        __debugbreak();
    }

    if (!cachedFull || !full)
    {
        cachedFull = full;
        if (s_geogroupid == -1)
        {
            SV_GroupError("No group to join");
        }
        else
        {
            group = dwGetGroup(0);
            if (group)
            {
                numgroups = 0;
                groupIDs[0] = playlist->current.integer + s_geogroupid;
                numgroups = 1;
                if (SV_IsWagerServer())
                {
                    groupIDs[numgroups++] = 492;
                }
                else
                {
                    licenseType = SV_GetLicenseType();
                    if (SV_IsServerRanked(licenseType))
                        groupIDs[numgroups] = 490;
                    else
                        groupIDs[numgroups] = 491;
                    ++numgroups;
                }
                if (full)
                    groupIDs[numgroups++] = s_geogroupid + playlist->current.integer + 64;
                task = TaskManager2_CreateTask(task_SVSetGroups, 0, 0, 0);
                setGroupsTask = (const bdReference<bdCommonAddr> *)bdGroup::setGroups(group, (int)&remoteTaskRef, (int)groupIDs, numgroups);
                bdReference<bdCommonAddr>::operator=((bdReference<bdCommonAddr> *)&task->remoteTask, setGroupsTask);
                bdReference<bdRemoteTask>::~bdReference<bdRemoteTask>(&remoteTaskRef);
                TaskManager2_StartTask(task);
            }
            else
            {
                SV_GroupError("Couldn't get group access, probable connectivity problem");
            }
        }
    }
#endif
}

void __cdecl SV_Groups_ParseGeos(const char *geoblob)
{
#ifdef KISAK_LIVE
    int regionCode;
    parseInfo_t *countryToken;
    parseInfo_t *geoToken;
    const char *geoLocation;
    int locationIndex;

    if (!geoblob
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\server_mp\\sv_autoplaylist.cpp",
            474,
            0,
            "%s",
            "NULL != geoblob"))
    {
        __debugbreak();
    }

    if (sv_geolocation && sv_geolocation->current.string[0])
    {
        Com_Printf(15, "Setting location to %s\n", sv_geolocation->current.string);
    }
    else if (sv_geolocation)
    {
        Dvar_SetString((dvar_s *)sv_geolocation, "LAX");
    }

    geoLocation = sv_geolocation->current.string;
    locationIndex = 0;
    Com_BeginParseSession("geogroups");
    Com_SetSpaceDelimited(1);

    while (1)
    {
        while (1)
        {
            geoToken = Com_Parse(&geoblob);
            if (!geoToken || geoToken->token[0] != '#')
                break;
            Com_SkipRestOfLine(&geoblob);
        }
        if (!geoToken || !geoToken->token[0])
            break;

        if (!I_stricmp(geoToken->token, geoLocation))
        {
            s_geogroupid = (locationIndex << 7) + 500;
            Com_DPrintf(15, "GEOGROUPS: Set groupid to %i from geo %s\n", s_geogroupid, geoLocation);
            countryToken = Com_ParseOnLine(&geoblob);
            regionCode = atoi(countryToken->token);
            SV_SetRegion(regionCode);
            break;
        }

        Com_SkipRestOfLine(&geoblob);
        ++locationIndex;
    }

    Com_EndParseSession();

    if (s_geogroupid == -1)
    {
        SV_GroupError("Failed to find match for geo %s\n", geoLocation);
    }
    else if (!Dvar_GetBool("sv_ap_enabled"))
    {
        SV_Groups_SetGroupMembership(0);
    }
#endif
}

void __cdecl SV_AP_Start()
{
#ifdef KISAK_LIVE
    if (!Dvar_GetBool("sv_ap_enabled"))
        return;

    if (s_apstate == AP_SLEEPING)
    {
        s_apstate = AP_START;
    }
    else if (!Assert_MyHandler(
        "C:\\projects_pc\\cod\\codsrc\\src\\server_mp\\sv_autoplaylist.cpp",
        572,
        0,
        "Tried starting autoplaylist when not in AP_SLEEPING\n"))
    {
        __debugbreak();
    }
#endif
}

void __cdecl SV_AP_Frame()
{
#ifdef KISAK_LIVE
    switch (s_apstate)
    {
    case AP_START:
        if (Dvar_GetBool("sv_ap_debug"))
            Com_Printf(15, "AP: %s\n", "Initing..\n");
        s_groupgetTask = SV_GetGroupCounts();
        s_controlFileTask = SV_AP_GetControlFile();
        if (s_groupgetTask && s_controlFileTask)
        {
            s_apstate = AP_GETTINGDATA;
            s_aptimeout = Sys_Milliseconds();
            if (Dvar_GetBool("sv_ap_debug"))
                Com_Printf(15, "AP: %s\n", "AP_GETTINGDATA\n");
        }
        else
        {
            if (Dvar_GetBool("sv_ap_fatal"))
                Com_Error(ERR_DROP, "AP error: %s\n", "Couldn't start data tasks\n");
            else
                Com_PrintError(15, "AP Error: %s\n", "Couldn't start data tasks\n");
            s_apstate = AP_ERROR;
        }
        break;

    case AP_GETTINGDATA:
    case AP_GETONE:
    case AP_SETTINGDATA:
        if ((int)(Sys_Milliseconds() - s_aptimeout) > 60000)
        {
            if (Dvar_GetBool("sv_ap_fatal"))
                Com_Error(ERR_DROP, "AP error: %s\n", "Data timeout\n");
            else
                Com_PrintError(15, "AP Error: %s\n", "Data timeout\n");
            s_apstate = AP_ERROR;
        }
        break;

    case AP_CHOOSE:
        if (Dvar_GetBool("sv_ap_debug"))
            Com_Printf(15, "AP: %s\n", "Fetched data ok\n");
        SV_AP_ParseControlFile(s_controlFileBuffer);
        SV_Groups_SetGroupMembership(0);
        s_aptimeout = Sys_Milliseconds();
        s_apstate = AP_SETTINGDATA;
        break;

    case AP_SLEEPING:
        if (com_sv_running && com_sv_running->current.enabled)
        {
            if (SV_AP_ServerIsFull())
            {
                if (!cachedFull_serverFull)
                {
                    cachedFull_serverFull = 1;
                    SV_Groups_SetGroupMembership(1);
                }
            }
            else if (cachedFull_serverFull)
            {
                cachedFull_serverFull = 0;
                SV_Groups_SetGroupMembership(0);
            }
        }
        break;

    default:
        return;
    }
#endif
}

bool __cdecl SV_AP_ServerIsFull()
{
    int clientIndex;

    if (!com_sv_running || !com_sv_running->current.enabled)
        return 1;

    for (clientIndex = 0; clientIndex < com_maxclients->current.integer; ++clientIndex)
    {
        if (svs.clients[clientIndex].header.state == CS_FREE)
            return 0;
    }
    return 1;
}
