#include "dwUtils.h"

bdLobbyService *__cdecl dwGetLobby(int controllerIndex)
{
  bdLobbyService *v2; // [esp+0h] [ebp-8h]
  bdLobbyService *v3; // [esp+4h] [ebp-4h]

  if ( !g_dwControllerData[controllerIndex].lobby )
  {
    v3 = (bdLobbyService *)bdMemory::allocate(0x118u);
    if ( v3 )
      v2 = bdLobbyService::bdLobbyService(v3);
    else
      v2 = 0;
    g_dwControllerData[controllerIndex].lobby = v2;
    dwInitMessaging(controllerIndex);
  }
  return g_dwControllerData[controllerIndex].lobby;
}

bdMatchMaking *__cdecl dwGetMatchmaking(int controllerIndex)
{
  if ( !g_dwControllerData[controllerIndex].lobby
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\Dw\\dwUtils.cpp",
          215,
          0,
          "%s",
          "g_dwControllerData[ controllerIndex ].lobby != BD_NULL") )
  {
    __debugbreak();
  }
  return bdLobbyService::getMatchMaking(g_dwControllerData[controllerIndex].lobby);
}

bdGroup *__cdecl dwGetGroup(int controllerIndex)
{
  bdLobbyService *lobbyService; // [esp+0h] [ebp-8h]
  bdGroup *group; // [esp+4h] [ebp-4h]

  group = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService && bdLobbyService::getStatus(lobbyService) == BD_NOT_CONNECTED )
    return bdLobbyService::getGroup(lobbyService);
  return group;
}

bdCounter *__cdecl dwGetCounter(int controllerIndex)
{
  bdLobbyService *lobbyService; // [esp+0h] [ebp-8h]
  bdCounter *counter; // [esp+4h] [ebp-4h]

  counter = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService && bdLobbyService::getStatus(lobbyService) == BD_NOT_CONNECTED )
    return bdLobbyService::getCounter(lobbyService);
  return counter;
}

bdContentStreaming *__cdecl dwGetContentStreaming(int controllerIndex)
{
  bdLobbyService *lobbyService; // [esp+0h] [ebp-8h]
  bdContentStreaming *contentStreaming; // [esp+4h] [ebp-4h]

  contentStreaming = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService && bdLobbyService::getStatus(lobbyService) == BD_NOT_CONNECTED )
  {
    contentStreaming = bdLobbyService::getContentStreaming(lobbyService);
    if ( !contentStreaming )
      Com_PrintError(16, "Could not get a handle to the Content Streaming service.\n");
  }
  else
  {
    Com_PrintError(16, "Error retrieving the Demonware lobby.\n");
  }
  return contentStreaming;
}

bdPooledStorage *__cdecl dwGetPooledStorage(int controllerIndex)
{
  bdLobbyService *lobbyService; // [esp+0h] [ebp-8h]
  bdPooledStorage *pooledStorage; // [esp+4h] [ebp-4h]

  pooledStorage = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService && bdLobbyService::getStatus(lobbyService) == BD_NOT_CONNECTED )
  {
    pooledStorage = bdLobbyService::getPooledStorage(lobbyService);
    if ( !pooledStorage )
      Com_PrintError(16, "Could not get a handle to the Pooled Storage service.\n");
  }
  else
  {
    Com_PrintError(16, "Error retrieving the Demonware lobby.\n");
  }
  return pooledStorage;
}

bdTags *__cdecl dwGetTagService(int controllerIndex)
{
  if ( !g_dwControllerData[controllerIndex].lobby
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\Dw\\dwUtils.cpp",
          293,
          0,
          "%s",
          "g_dwControllerData[ controllerIndex ].lobby != BD_NULL") )
  {
    __debugbreak();
  }
  return bdLobbyService::getTags(g_dwControllerData[controllerIndex].lobby);
}

bdVoteRank *__cdecl dwGetVoteRankService(int controllerIndex)
{
  if ( !g_dwControllerData[controllerIndex].lobby
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\Dw\\dwUtils.cpp",
          301,
          0,
          "%s",
          "g_dwControllerData[ controllerIndex ].lobby != BD_NULL") )
  {
    __debugbreak();
  }
  return bdLobbyService::getVoteRank(g_dwControllerData[controllerIndex].lobby);
}

bdStorage *__cdecl dwGetStorage(int controllerIndex)
{
  bdLobbyService *lobbyService; // [esp+0h] [ebp-8h]
  bdStorage *storage; // [esp+4h] [ebp-4h]

  storage = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService && bdLobbyService::getStatus(lobbyService) == BD_NOT_CONNECTED )
    return bdLobbyService::getStorage(lobbyService);
  return storage;
}

bdStats *__cdecl dwGetStats(int controllerIndex)
{
  bdLobbyService *lobbyService; // [esp+0h] [ebp-8h]
  bdStats *stats; // [esp+4h] [ebp-4h]

  stats = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService )
    return bdLobbyService::getStats(lobbyService);
  return stats;
}

bdSecurityKeyMap *__cdecl dwGetSecurityKeyMap()
{
  bdSocketRouter *SocketRouter; // eax
  bdSecurityKeyMap *secKeyMap; // [esp+68h] [ebp-8h]
  bdNetImpl *net; // [esp+6Ch] [ebp-4h]

  secKeyMap = 0;
  net = bdSingleton<bdNetImpl>::getInstance();
  if ( net )
  {
    SocketRouter = bdNetImpl::getSocketRouter(net);
    return bdSocketRouter::getKeyMap(SocketRouter);
  }
  return secKeyMap;
}

bdSocketRouter *__cdecl dwGetSocketRouter()
{
  bdNetImpl *net; // [esp+68h] [ebp-8h]
  bdSocketRouter *socketRouter; // [esp+6Ch] [ebp-4h]

  socketRouter = 0;
  net = bdSingleton<bdNetImpl>::getInstance();
  if ( net )
    return bdNetImpl::getSocketRouter(net);
  return socketRouter;
}

bdAddressMap *__cdecl dwGetAddressMap()
{
  bdSocketRouter *socketRouter; // [esp+0h] [ebp-4h]

  socketRouter = dwGetSocketRouter();
  if ( socketRouter )
    return bdSocketRouter::getAddressMap(socketRouter);
  else
    return 0;
}

bdQoSProbe *__cdecl dwGetQoSProbe()
{
  bdQoSProbe *qosProbe; // [esp+0h] [ebp-8h]
  bdSocketRouter *socketRouter; // [esp+4h] [ebp-4h]

  qosProbe = 0;
  socketRouter = dwGetSocketRouter();
  if ( socketRouter )
    qosProbe = bdSocketRouter::getQoSProber(socketRouter);
  if ( !qosProbe && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\Dw\\dwUtils.cpp", 408, 0, "%s", "qosProbe") )
    __debugbreak();
  return qosProbe;
}

bdReference<bdCommonAddr> *__cdecl dwGetLocalCommonAddr(bdReference<bdCommonAddr> *result)
{
  const bdReference<bdCommonAddr> *v1; // eax
  bdReference<bdCommonAddr> v3; // [esp+54h] [ebp-Ch] BYREF
  bdNetImpl *net; // [esp+58h] [ebp-8h]
  bdReference<bdCommonAddr> localCommonAddr; // [esp+5Ch] [ebp-4h] BYREF

  localCommonAddr.m_ptr = 0;
  net = bdSingleton<bdNetImpl>::getInstance();
  if ( net )
  {
    while ( bdNetImpl::getStatus(net) == BD_NET_PENDING )
    {
      bdNetImpl::pump(net);
      NET_Sleep(0xAu);
    }
    if ( bdNetImpl::getStatus(net) == BD_NET )
    {
      v1 = (const bdReference<bdCommonAddr> *)bdNetImpl::getLocalCommonAddr(net, (int)&v3);
      bdReference<bdCommonAddr>::operator=(&localCommonAddr, v1);
      bdReference<bdRemoteTask>::~bdReference<bdRemoteTask>(&v3);
    }
  }
  result->m_ptr = localCommonAddr.m_ptr;
  if ( result->m_ptr )
    InterlockedIncrement(&result->m_ptr->m_refCount);
  bdReference<bdRemoteTask>::~bdReference<bdRemoteTask>(&localCommonAddr);
  return result;
}

bdAuthService *__cdecl dwGetAuthService(bdInetAddr authAddr, unsigned __int16 authPort, unsigned int titleID)
{
  bdAuthService *v4; // [esp+0h] [ebp-14h]
  bdAuthService *v5; // [esp+4h] [ebp-10h]
  bdAuthService *v6; // [esp+8h] [ebp-Ch]
  bdAddr addr; // [esp+Ch] [ebp-8h] BYREF

  if ( !g_authService )
  {
    bdAddr::bdAddr(&addr, &authAddr, authPort);
    v6 = (bdAuthService *)bdMemory::allocate(0x268u);
    if ( v6 )
      v4 = bdAuthService::bdAuthService(v6, titleID, &addr);
    else
      v4 = 0;
    g_authService = v4;
    bdInetAddr::~bdInetAddr(&addr.m_address);
  }
  v5 = g_authService;
  bdInetAddr::~bdInetAddr(&authAddr);
  return v5;
}

void __cdecl dwAuthServiceCleanup()
{
  if ( g_authService )
  {
    ((void (__thiscall *)(bdAuthService *, int))g_authService->~bdAuthService)(g_authService, 1);
    g_authService = 0;
  }
}

bdTitleUtilities *__cdecl dwGetTitleUtilities(int controllerIndex)
{
  bdTitleUtilities *obj; // [esp+0h] [ebp-8h]
  bdLobbyService *lobbyService; // [esp+4h] [ebp-4h]

  obj = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService )
    return bdLobbyService::getTitleUtilities(lobbyService);
  return obj;
}

bdMessaging *__cdecl dwGetMessaging(int controllerIndex)
{
  bdLobbyService *lobbyService; // [esp+0h] [ebp-8h]
  bdMessaging *messaging; // [esp+4h] [ebp-4h]

  messaging = 0;
  lobbyService = dwGetLobby(controllerIndex);
  if ( lobbyService )
    return bdLobbyService::getMessaging(lobbyService);
  return messaging;
}

void __cdecl dwCloseRemoteTask(overlappedTask *task)
{
  if ( task->overlappedIO.m_ptr )
  {
    task->finalStatus = bdRemoteTask::getStatus(task->overlappedIO.m_ptr);
    task->errorCode = bdRemoteTask::getErrorCode(task->overlappedIO.m_ptr);
    bdReference<bdCommonAddr>::operator=(&task->overlappedIO, 0);
  }
}

void __cdecl dwCloseRemoteTask(bdReference<bdRemoteTask> *remoteTask)
{
  if ( remoteTask )
  {
    if ( remoteTask->m_ptr )
      bdReference<bdCommonAddr>::operator=(remoteTask, 0);
  }
}

int __cdecl dwTaskStatusConvert(bdRemoteTask::bdStatus taskStatus, bdLobbyErrorCode lobbyError)
{
  taskCompleteResults status; // [esp+0h] [ebp-4h]

  status = TASK_ERROR;
  if ( taskStatus == BD && lobbyError == BD_NO_ERROR )
    return 1;
  if ( taskStatus == BD_PENDING )
    return 0;
  return status;
}

const char *__cdecl dwLobbyErrorCodeToString(bdLobbyErrorCode code)
{
  unsigned int i; // [esp+0h] [ebp-8h]

  for ( i = 0; i < 0xA4; ++i )
  {
    if ( lobbyErrorCodeLookup[i] == code )
      return lobbyErrorCodeDescs[i];
  }
  return "Unknown bdLobbyErrorCode";
}

void __cdecl dwLobbyErrorCodeToString(bdLobbyErrorCode code, char *const buffer, unsigned int bufferSize)
{
  char *v3; // eax

  v3 = (char *)dwLobbyErrorCodeToString(code);
  bdStrlcpy(buffer, v3, bufferSize);
}

unsigned int __cdecl bdStrlcpy(char *const dst, char *src, unsigned int size)
{
  unsigned int v4; // [esp+0h] [ebp-1Ch]
  unsigned int slen; // [esp+18h] [ebp-4h]

  slen = strlen(src);
  if ( size && dst )
  {
    if ( slen >= size - 1 )
      v4 = size - 1;
    else
      v4 = slen;
    memcpy((unsigned __int8 *)dst, (unsigned __int8 *)src, v4);
    dst[v4] = 0;
  }
  return slen;
}

void __cdecl dwConnectionErrorHandler(int error_context)
{
  char *v1; // eax
  char errorString[512]; // [esp+0h] [ebp-208h] BYREF
  const char *error; // [esp+204h] [ebp-4h]

  v1 = UI_SafeTranslateString("XBOXLIVE_LOBBY_CONNECTION_ERR");
  error = UI_ReplaceConversionInt(v1, error_context);
  I_strncpyz(errorString, error, 512);
  Com_Error(ERR_DROP, "%s", errorString);
}

void __cdecl dwEnterDeferredCritsec()
{
  Sys_EnterCriticalSection(CRITSECT_DEFERRED_DW);
}

void __cdecl dwLeaveDeferredCritsec()
{
  Sys_LeaveCriticalSection(CRITSECT_DEFERRED_DW);
}

