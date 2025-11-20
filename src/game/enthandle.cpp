#include "enthandle.h"

void __cdecl EntHandle::Init()
{
  unsigned int i; // [esp+4h] [ebp-4h]

  for ( i = 1; i < 0x400; ++i )
  {
    g_entHandleInfoArray[i].next = i + 1;
    g_entHandleInfoArray[i].handle = 0;
  }
  g_entHandleInfoArray[1023].next = 0;
  g_entHandleInfoHead = 1;
  memset((unsigned __int8 *)g_entitiesHandleList, 0, sizeof(g_entitiesHandleList));
  g_usedEntHandle = 0;
}

void __cdecl EntHandle::Shutdown()
{
  const char *v0; // eax
  unsigned int usedEntHandle; // [esp+0h] [ebp-Ch]
  unsigned int i; // [esp+8h] [ebp-4h]

  if ( g_usedEntHandle )
  {
    Com_Printf(1, "EntHandle BEGIN\n");
    usedEntHandle = 0;
    for ( i = 1; i < 0x400; ++i )
    {
      if ( g_entHandleInfoArray[i].handle )
      {
        Com_Printf(1, "%p\n", (const void *)(8 * i + 63688520));
        ++usedEntHandle;
      }
    }
    Com_Printf(1, "EntHandle END\n");
    if ( usedEntHandle != g_usedEntHandle )
    {
      v0 = va("%d %d", usedEntHandle, g_usedEntHandle);
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\game\\enthandle.cpp",
              105,
              0,
              "%s\n\t%s",
              "usedEntHandle == g_usedEntHandle",
              v0) )
        __debugbreak();
    }
    if ( g_usedEntHandle
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\enthandle.cpp",
            106,
            0,
            "%s\n\t(g_usedEntHandle) = %i",
            "(!g_usedEntHandle)",
            g_usedEntHandle) )
    {
      __debugbreak();
    }
  }
}

void __cdecl SentientHandle::Init()
{
  memset((unsigned __int8 *)g_sentientsHandleList, 0, sizeof(g_sentientsHandleList));
}

void __cdecl EntHandleDissociate(gentity_s *ent)
{
  EntHandleDissociateInternal((EntHandleList *)(2 * (ent - g_entities) + 63686360));
}

void __cdecl EntHandleDissociateInternal(EntHandleList *entHandleList)
{
  const char *v1; // eax
  unsigned int infoIndexHead; // [esp+4h] [ebp-8h]
  unsigned int infoIndex; // [esp+8h] [ebp-4h]

  infoIndexHead = entHandleList->infoIndex;
  if ( entHandleList->infoIndex )
  {
    entHandleList->infoIndex = 0;
    infoIndex = infoIndexHead;
    do
    {
      if ( !g_entHandleInfoArray[infoIndex].handle )
      {
        v1 = va("%d %p", infoIndex, (const void *)(8 * infoIndex + 63688520));
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game\\enthandle.cpp",
                139,
                0,
                "%s\n\t%s",
                "info->handle",
                v1) )
          __debugbreak();
      }
      *(unsigned int *)g_entHandleInfoArray[infoIndex].handle = 0;
      g_entHandleInfoArray[infoIndex].handle = 0;
      if ( !g_usedEntHandle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\enthandle.cpp", 144, 0, "%s", "g_usedEntHandle") )
      {
        __debugbreak();
      }
      --g_usedEntHandle;
      infoIndex = g_entHandleInfoArray[infoIndex].next;
    }
    while ( infoIndex != infoIndexHead );
    g_entHandleInfoArray[g_entHandleInfoArray[infoIndexHead].prev].next = g_entHandleInfoHead;
    g_entHandleInfoHead = infoIndexHead;
  }
}

void __cdecl SentientHandleDissociate(sentient_s *sentient)
{
  EntHandleDissociateInternal((EntHandleList *)(2 * (sentient - g_sentients) + 63688424));
}

void __thiscall EntHandle::setEnt(EntHandle *this, gentity_s *ent)
{
  gentity_s *oldEnt; // [esp+4h] [ebp-8h]

  if ( EntHandle::isDefined(this) )
  {
    oldEnt = EntHandle::ent(this);
    if ( ent == oldEnt )
      return;
    RemoveEntHandleInfo((EntHandleList *)(2 * (oldEnt - g_entities) + 63686360), this->infoIndex);
    if ( !ent )
    {
      this->number = 0;
      this->infoIndex = 0;
      return;
    }
  }
  else if ( !ent )
  {
    return;
  }
  this->infoIndex = AddEntHandleInfo((EntHandleList *)(2 * (ent - g_entities) + 63686360), this);
  this->number = ent - g_entities + 1;
}

bool __thiscall EntHandle::isDefined(EntHandle *this)
{
  if ( this->number
    && !g_entities[this->number - 1].r.inuse
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\../game_mp/g_public_mp.h",
          389,
          0,
          "%s\n\t(number - 1) = %i",
          "(!number || g_entities[number - 1].r.inuse)",
          this->number - 1) )
  {
    __debugbreak();
  }
  return this->number != 0;
}

gentity_s *__thiscall EntHandle::ent(EntHandle *this)
{
  if ( (unsigned int)this->number - 1 >= 0x3FF
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\../game_mp/g_public_mp.h",
          396,
          0,
          "number - 1 doesn't index ENTITYNUM_NONE\n\t%i not in [0, %i)",
          this->number - 1,
          1023) )
  {
    __debugbreak();
  }
  if ( !g_entities[this->number - 1].r.inuse
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\../game_mp/g_public_mp.h",
          397,
          0,
          "%s\n\t(number - 1) = %i",
          "(g_entities[number - 1].r.inuse)",
          this->number - 1) )
  {
    __debugbreak();
  }
  return &g_entities[this->number - 1];
}

void __cdecl RemoveEntHandleInfo(EntHandleList *entHandleList, unsigned int oldInfoIndex)
{
  const char *v2; // eax
  EntHandleInfo *prev; // [esp+Ch] [ebp-4h]

  if ( oldInfoIndex )
  {
    if ( entHandleList->infoIndex == oldInfoIndex )
      entHandleList->infoIndex = oldInfoIndex != g_entHandleInfoArray[oldInfoIndex].next
                               ? g_entHandleInfoArray[oldInfoIndex].next
                               : 0;
    if ( !g_entHandleInfoArray[oldInfoIndex].handle )
    {
      v2 = va("%d %p", oldInfoIndex, (const void *)(8 * oldInfoIndex + 63688520));
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\game\\enthandle.cpp",
              196,
              0,
              "%s\n\t%s",
              "info->handle",
              v2) )
        __debugbreak();
    }
    g_entHandleInfoArray[oldInfoIndex].handle = 0;
    if ( !g_usedEntHandle
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\enthandle.cpp", 198, 0, "%s", "g_usedEntHandle") )
    {
      __debugbreak();
    }
    --g_usedEntHandle;
    prev = (EntHandleInfo *)(8 * g_entHandleInfoArray[oldInfoIndex].prev + 63688520);
    g_entHandleInfoArray[g_entHandleInfoArray[oldInfoIndex].next].prev = g_entHandleInfoArray[oldInfoIndex].prev;
    prev->next = g_entHandleInfoArray[oldInfoIndex].next;
    g_entHandleInfoArray[oldInfoIndex].next = g_entHandleInfoHead;
    g_entHandleInfoHead = oldInfoIndex;
  }
}

unsigned int __cdecl AddEntHandleInfo(EntHandleList *entHandleList, void *handle)
{
  unsigned int v3; // [esp+0h] [ebp-14h]
  unsigned int infoIndexHead; // [esp+8h] [ebp-Ch]
  unsigned int infoIndex; // [esp+Ch] [ebp-8h]
  EntHandleInfo *infoHead; // [esp+10h] [ebp-4h]

  infoIndex = g_entHandleInfoHead;
  if ( !g_entHandleInfoHead )
    Com_Error(ERR_DROP, "ENT_HANDLE_COUNT exceeded - increase size");
  g_entHandleInfoHead = g_entHandleInfoArray[infoIndex].next;
  if ( (int)g_maxUsedEntHandle < (int)++g_usedEntHandle )
    v3 = g_usedEntHandle;
  else
    v3 = g_maxUsedEntHandle;
  g_maxUsedEntHandle = v3;
  g_entHandleInfoArray[infoIndex].handle = handle;
  infoIndexHead = entHandleList->infoIndex;
  if ( entHandleList->infoIndex )
  {
    infoHead = (EntHandleInfo *)(8 * infoIndexHead + 63688520);
    g_entHandleInfoArray[infoIndex].next = g_entHandleInfoArray[infoIndexHead].next;
    g_entHandleInfoArray[infoIndex].prev = infoIndexHead;
    g_entHandleInfoArray[infoHead->next].prev = infoIndex;
    infoHead->next = infoIndex;
  }
  else
  {
    g_entHandleInfoArray[infoIndex].next = infoIndex;
    g_entHandleInfoArray[infoIndex].prev = infoIndex;
    entHandleList->infoIndex = infoIndex;
  }
  return infoIndex;
}

void __thiscall SentientHandle::setSentient(SentientHandle *this, sentient_s *sentient)
{
  sentient_s *oldSentient; // [esp+4h] [ebp-8h]

  if ( SentientHandle::isDefined(this) )
  {
    oldSentient = SentientHandle::sentient(this);
    if ( sentient == oldSentient )
      return;
    RemoveEntHandleInfo((EntHandleList *)(2 * (oldSentient - g_sentients) + 63688424), this->infoIndex);
    if ( !sentient )
    {
      this->number = 0;
      this->infoIndex = 0;
      return;
    }
  }
  else if ( !sentient )
  {
    return;
  }
  this->infoIndex = AddEntHandleInfo((EntHandleList *)(2 * (sentient - g_sentients) + 63688424), this);
  this->number = sentient - g_sentients + 1;
}

bool __thiscall SentientHandle::isDefined(SentientHandle *this)
{
  if ( this->number
    && !g_sentients[this->number - 1].ent
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\sentient.h",
          98,
          0,
          "%s\n\t(number - 1) = %i",
          "(!number || g_sentients[number - 1].ent)",
          this->number - 1) )
  {
    __debugbreak();
  }
  if ( this->number
    && !g_sentients[this->number - 1].ent->r.inuse
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\sentient.h",
          99,
          0,
          "%s\n\t(number - 1) = %i",
          "(!number || g_sentients[number - 1].ent->r.inuse)",
          this->number - 1) )
  {
    __debugbreak();
  }
  return this->number != 0;
}

sentient_s *__thiscall SentientHandle::sentient(SentientHandle *this)
{
  if ( (unsigned int)this->number - 1 >= 0x30
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\sentient.h",
          106,
          0,
          "number - 1 doesn't index MAX_SENTIENTS\n\t%i not in [0, %i)",
          this->number - 1,
          48) )
  {
    __debugbreak();
  }
  if ( !g_sentients[this->number - 1].ent
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\sentient.h",
          107,
          0,
          "%s\n\t(number - 1) = %i",
          "(g_sentients[number - 1].ent)",
          this->number - 1) )
  {
    __debugbreak();
  }
  if ( !g_sentients[this->number - 1].ent->r.inuse
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\game\\sentient.h",
          108,
          0,
          "%s\n\t(number - 1) = %i",
          "(g_sentients[number - 1].ent->r.inuse)",
          this->number - 1) )
  {
    __debugbreak();
  }
  return &g_sentients[this->number - 1];
}

