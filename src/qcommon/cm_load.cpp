#include "cm_load.h"

void __cdecl CM_LoadMap(const char *name, int *checksum)
{
  if ( !name || !*name )
    Com_Error(ERR_DROP, &byte_CD016C);
  CM_LoadMapData(name);
  CM_InitAllThreadData();
  cm.isInUse = 1;
  *checksum = cm.checksum;
}

void CM_InitAllThreadData()
{
  int workerIndex; // [esp+0h] [ebp-4h]

  CM_InitThreadData(THREAD_CONTEXT_MAIN);
  CM_InitThreadData(THREAD_CONTEXT_BACKEND);
  for ( workerIndex = 0; workerIndex < r_smp_worker_threads->current.integer; ++workerIndex )
    CM_InitThreadData(workerIndex + 2);
  CM_InitThreadData(THREAD_CONTEXT_SERVER);
  CM_InitThreadData(THREAD_CONTEXT_OCCLUSION);
}

void __cdecl CM_InitThreadData(unsigned int threadContext)
{
  TraceThreadInfo *traceThreadInfo; // [esp+8h] [ebp-4h]

  if ( threadContext >= 0xC
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\cm_load.cpp",
          81,
          0,
          "threadContext doesn't index THREAD_CONTEXT_TRACE_COUNT\n\t%i not in [0, %i)",
          threadContext,
          12) )
  {
    __debugbreak();
  }
  traceThreadInfo = &g_traceThreadInfo[threadContext];
  traceThreadInfo->checkcount.global = 0;
  traceThreadInfo->checkcount.partitions = (int *)Hunk_Alloc(4 * cm.partitionCount, "CM_InitThreadData", 29);
  traceThreadInfo->box_brush = &g_box_brush[threadContext];
  memcpy((void *)traceThreadInfo->box_brush, cm.box_brush, sizeof(cbrush_t));
  traceThreadInfo->box_model = &g_box_model[threadContext];
  memcpy(traceThreadInfo->box_model, &cm.box_model, sizeof(cmodel_t));
  traceThreadInfo->geoms = (PhysGeomList **)(4 * threadContext + 66312524);
  *traceThreadInfo->geoms = 0;
}

void __cdecl CM_LoadMapData(const char *name)
{
  if ( useFastFile->current.enabled )
    CM_LoadMapData_FastFile(name);
  else
    CM_LoadMapData_LoadObj(name);
}

void __cdecl CM_LoadMapData_LoadObj(const char *name)
{
  if ( !cm.isInUse || I_stricmp(cm.name, name) )
  {
    ProfLoad_Begin("Load bsp collision");
    CM_LoadMapFromBsp(name, 1);
    ProfLoad_End();
    ProfLoad_Begin("Load static model collision");
    CM_LoadStaticModels();
    ProfLoad_End();
    ProfLoad_Begin("Load dynamic entities");
    DynEnt_LoadEntities();
    ProfLoad_End();
  }
}

void __cdecl CM_LoadMapData_FastFile(const char *name)
{
  if ( DB_FindXAssetHeader(ASSET_TYPE_CLIPMAP_PVS, name, 1, -1).clipMap != &cm
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\cm_load.cpp", 168, 0, "%s", "clipMap == &cm") )
  {
    __debugbreak();
  }
}

void __cdecl Rope_InitRopes()
{
  int i; // [esp+0h] [ebp-4h]

  g_mapRopeCount = 0;
  for ( i = 0; i < cm.num_constraints; ++i )
  {
    if ( cm.constraints[i].type == CONSTRAINT_ROPE )
      ++g_mapRopeCount;
  }
  g_totalRopeCount = g_mapRopeCount;
  g_entsWithRopesCount = 0;
}

void __cdecl ShutdownRopes()
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < cm.num_constraints; ++i )
    cm.constraints[i].constraintHandle = 0;
  g_entsWithRopesCount = 0;
  g_totalRopeCount = 0;
  g_mapRopeCount = 0;
}

void __cdecl CM_Shutdown()
{
  const char *savedName; // [esp+0h] [ebp-4h]

  savedName = cm.name;
  Com_Memset((unsigned int *)&cm, 0, 332);
  cm.name = savedName;
  if ( cm.isInUse )
  {
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\cm_load.cpp", 295, 0, "%s", "!cm.isInUse") )
      __debugbreak();
  }
}

void __cdecl CM_Unload()
{
  if ( !useFastFile->current.enabled
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\cm_load.cpp", 308, 0, "%s", "IsFastFileLoad()") )
  {
    __debugbreak();
  }
  if ( cm.isInUse )
    Sys_Error("Cannot unload collision while it is in use");
}

int __cdecl CM_LeafCluster(unsigned int leafnum)
{
  if ( leafnum >= cm.numLeafs
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\cm_load.cpp",
          319,
          0,
          "leafnum doesn't index cm.numLeafs\n\t%i not in [0, %i)",
          leafnum,
          cm.numLeafs) )
  {
    __debugbreak();
  }
  return cm.leafs[leafnum].cluster;
}

void __cdecl CM_ModelBounds(unsigned int model, float *mins, float *maxs)
{
  cmodel_t *v3; // eax

  v3 = CM_ClipHandleToModel(model);
  *mins = v3->mins[0];
  mins[1] = v3->mins[1];
  mins[2] = v3->mins[2];
  *maxs = v3->maxs[0];
  maxs[1] = v3->maxs[1];
  maxs[2] = v3->maxs[2];
}

int __cdecl CM_ModelSurfaceFlags(unsigned int model)
{
  cLeafBrushNode_s *node; // [esp+Ch] [ebp-10h]
  int surfaceFlags; // [esp+18h] [ebp-4h]

  surfaceFlags = 0;
  if ( model < cm.numSubModels )
  {
    node = &cm.leafbrushNodes[CM_ClipHandleToModel(model)->leaf.leafBrushNode];
    if ( node->leafBrushCount > 0 )
      return cm.brushes[*node->data.leaf.brushes].axial_sflags[0][0];
  }
  return surfaceFlags;
}

unsigned __int8 *__cdecl CM_Hunk_Alloc(unsigned int size, const char *name, int type)
{
  return Hunk_Alloc(size, name, type);
}

void __cdecl CM_Hunk_CheckTempMemoryHighClear()
{
  Hunk_CheckTempMemoryHighClear();
}

unsigned int __cdecl CM_Hunk_AllocateTempMemoryHigh(int size, const char *name)
{
  return Hunk_AllocateTempMemoryHigh(size, name);
}

void __cdecl CM_Hunk_ClearTempMemoryHigh()
{
  Hunk_ClearTempMemoryHigh();
}

