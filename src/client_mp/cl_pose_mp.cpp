#include "cl_pose_mp.h"

char *__cdecl CL_AllocSkelMemory(unsigned int size)
{
  char *result; // [esp+4h] [ebp-8h]
  int skelMemPos; // [esp+8h] [ebp-4h]
  unsigned int sizea; // [esp+14h] [ebp+8h]

  if ( !size && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_pose_mp.cpp", 33, 0, "%s", "size") )
    __debugbreak();
  sizea = (size + 15) & 0xFFFFFFF0;
  if ( sizea > 0x3FFF0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_pose_mp.cpp",
          36,
          0,
          "%s",
          "size <= CL_SKEL_MEMORY_SIZE - SKEL_MEM_ALIGNMENT") )
  {
    __debugbreak();
  }
  if ( !clients[R_GetLocalClientNum()].skelMemoryStart
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_pose_mp.cpp",
          38,
          0,
          "%s",
          "skel_glob->skelMemoryStart") )
  {
    __debugbreak();
  }
  skelMemPos = _InterlockedExchangeAdd(&clients[R_GetLocalClientNum()].skelMemPos, sizea);
  if ( (int)(sizea + skelMemPos) > g_maxSkelMemUsed )
  {
    g_maxSkelMemUsed = sizea + skelMemPos;
    if ( showUsage )
      Com_PrintWarning(14, "CL_AllocSkelMemory(): %i max used out of %i total\n", sizea + skelMemPos, 262128);
  }
  result = &clients[R_GetLocalClientNum()].skelMemoryStart[skelMemPos];
  if ( sizea + skelMemPos > 0x3FFF0 )
    return 0;
  if ( !result
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_pose_mp.cpp", 63, 0, "%s", "result") )
  {
    __debugbreak();
  }
  return result;
}

int __cdecl CL_GetSkelTimeStamp()
{
  return clients[R_GetLocalClientNum()].skelTimeStamp;
}

int __cdecl CL_DObjCreateSkelForBones(const DObj *obj, int *partBits, DObjAnimMat **pMatOut)
{
  char *buf; // [esp+0h] [ebp-Ch]
  unsigned int len; // [esp+4h] [ebp-8h]
  int timeStamp; // [esp+8h] [ebp-4h]

  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_pose_mp.cpp", 95, 0, "%s", "obj") )
    __debugbreak();
  timeStamp = CL_GetSkelTimeStamp();
  if ( DObjSkelExists(obj, timeStamp) )
  {
    *pMatOut = I_dmaGetDObjSkel(obj);
    return DObjSkelAreBonesUpToDate(obj, partBits);
  }
  else
  {
    len = DObjGetAllocSkelSize(obj);
    buf = CL_AllocSkelMemory(len);
    if ( buf )
    {
      *pMatOut = (DObjAnimMat *)buf;
      DObjCreateSkel(obj, buf, timeStamp);
      return 0;
    }
    else
    {
      *pMatOut = 0;
      if ( warnCount_0 != timeStamp )
      {
        warnCount_0 = timeStamp;
        Com_PrintWarning(14, "WARNING: CL_SKEL_MEMORY_SIZE exceeded - not calculating skeleton\n");
      }
      return 1;
    }
  }
}

