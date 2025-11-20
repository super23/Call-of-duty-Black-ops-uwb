#pragma once

bdLANDiscoveryServer *__cdecl dwGetLanDiscoveryServer();
void __cdecl dwCreateLanSession();
void __cdecl dwUpdateLanSession();
LanDiscoveryListener *__cdecl dwGetLanDiscoveryListener();
LanDiscoveryListener *__thiscall LanDiscoveryListener::LanDiscoveryListener(LanDiscoveryListener *this);
void __thiscall LanDiscoveryListener::onDiscovery(LanDiscoveryListener *this, bdReference<bdGameInfo> info);
void __thiscall LanDiscoveryListener::onDiscoveryFinished(LanDiscoveryListener *this);
void __cdecl dwStartLanDiscovery();
bdLANDiscoveryClient *__cdecl dwGetLanDiscoveryClient();
char __cdecl dwGetLanSession(int index, bdReference<bdCommonAddr> *addr, bdSecurityID *id, bdSecurityKey *key);
const bdSecurityKey *__cdecl dwGetSessionSecKey();
void __cdecl dwCreateSession(bdReference<bdCommonAddr> *task, MatchMakingInfo *const sessionInfo);
const bdSecurityID *__cdecl dwGetSessionSecID();
const bdSessionID *__cdecl dwGetSessionID();
void __cdecl dwSetSessionID(const bdSessionID *sessionID);
int __cdecl dwCreateSessionComplete(overlappedTask *const task);
void __cdecl dwUpdateSession(overlappedTask *const task, MatchMakingInfo *const sessionInfo);
taskCompleteResults __cdecl dwUpdateSessionComplete(overlappedTask *const task);
void __cdecl dwDeleteSession(int controllerIndex, overlappedTask *const task);
serverInfo_t *__cdecl FindServerBySecID(bdSecurityID *secid);
void __cdecl CL_SetPingFromQOS(int ping, bdSecurityID *secid);
void __cdecl dwMatchMaking_InviteDWUserSuccess();
void __cdecl dwMatchMaking_InviteDWUserFailure();
TaskRecord *__cdecl dwMatchMaking_InviteDWUserToSession(unsigned __int64 frienduid, bdSessionID sessionID);
void __cdecl dwMatchMaking_GetSessionByIDSuccess();
TaskRecord *__cdecl dwMatchMaking_FindSessionByID(bdSessionID *sessionID);
void __cdecl dwFindSessionsByEntityIDsFailure();
TaskRecord *__cdecl dwFindSessionsByEntityIDs(
        const unsigned __int64 *serverIDs,
        unsigned int numservers,
        MatchMakingInfo *serverinfos);
int __cdecl dwRegionCodeFromCountryCode(const char *countrycode);
bool __cdecl dwIsPagedFindInProgress();
void __cdecl dwFindSessionsPagedSuccess(TaskRecord *task);
void __cdecl dwFindSessionsPagedFailure();
bool __cdecl dwIsPagedFindTaskAlreadyStarted();
TaskRecord *__cdecl dwFindSessionsPaged(int netCodeVersion, int servertype, bool random, bool geo);
bdNetImpl *__cdecl bdSingleton<bdNetImpl>::getInstance();
bdGameInfoFactoryImpl *__cdecl bdSingleton<bdGameInfoFactoryImpl>::getInstance();
void __thiscall bdCreatorBase<bdGameInfo>::~bdCreatorBase<bdGameInfo>(bdCreatorBase<bdGameInfo> *this);
void __thiscall bdCreator<bdGameInfo,bdGameInfo>::~bdCreator<bdGameInfo,bdGameInfo>(
        bdCreator<bdGameInfo,bdGameInfo> *this);
bdGameInfo *__thiscall bdCreator<bdGameInfo,bdGameInfo>::create(bdCreator<bdGameInfo,bdGameInfo> *this);
unsigned int __thiscall bdCreator<bdGameInfo,bdGameInfo>::getSizeOf(bdCreator<bdGameInfo,bdGameInfo> *this);
void __thiscall bdArray<bdReference<bdGameInfo>>::clear(bdArray<bdReference<bdGameInfo> > *this);
void __thiscall bdArray<bdReference<bdGameInfo>>::pushBack(
        bdArray<bdReference<bdGameInfo> > *this,
        const bdReference<bdGameInfo> *value);
void bdSingleton<bdNetImpl>::destroyInstance();
void bdSingleton<bdGameInfoFactoryImpl>::destroyInstance();
void __thiscall bdArray<bdReference<bdGameInfo>>::increaseCapacity(
        bdArray<bdReference<bdGameInfo> > *this,
        unsigned int increase);
void __thiscall bdArray<bdReference<bdGameInfo>>::copyConstructArrayArray(
        bdArray<bdReference<bdGameInfo> > *this,
        bdReference<bdGameInfo> *dest,
        const bdReference<bdGameInfo> *src,
        unsigned int n);
