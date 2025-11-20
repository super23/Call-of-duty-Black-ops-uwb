#include "stringed_remote.h"

char __cdecl ProcessStringEdCmds()
{
  EXT_Link<LocalizeEntry> *Current; // [esp+78h] [ebp-18h]
  char *Value; // [esp+7Ch] [ebp-14h]
  LocalizeEntry *newEntry; // [esp+80h] [ebp-10h]
  LocalizeEntry *newEntrya; // [esp+80h] [ebp-10h]
  char *Reference; // [esp+84h] [ebp-Ch]
  int i; // [esp+88h] [ebp-8h]

  Sys_EnterCriticalSection(CRITSECT_STRINGED_COMMAND);
  for ( i = 0; i < StringEdCommandCount; ++i )
  {
    Reference = strtok(StringEdCommands[i], "!");
    Value = strtok(0, "\n");
    newEntry = 0;
    for ( Current = overrideLocalizedEntryList.m_Head; Current; Current = Current->m_Next )
    {
      if ( !strcmp(Reference, Current->m_Data->name) )
      {
        newEntry = Current->m_Data;
        break;
      }
    }
    if ( newEntry )
    {
      Com_LiveDeallocate((void *)newEntry->value);
      newEntry->value = (const char *)Com_LiveAllocate(strlen(Value) + 1);
      I_strncpyz((char *)newEntry->value, Value, strlen(Value) + 1);
    }
    else
    {
      newEntrya = (LocalizeEntry *)Com_LiveAllocate(8u);
      newEntrya->name = (const char *)Com_LiveAllocate(strlen(Reference) + 1);
      I_strncpyz((char *)newEntrya->name, Reference, strlen(Reference) + 1);
      newEntrya->value = (const char *)Com_LiveAllocate(strlen(Value) + 1);
      I_strncpyz((char *)newEntrya->value, Value, strlen(Value) + 1);
      EXT_List<LocalizeEntry>::Add(&overrideLocalizedEntryList, newEntrya, 0);
    }
  }
  StringEdCommandCount = 0;
  Sys_LeaveCriticalSection(CRITSECT_STRINGED_COMMAND);
  return 1;
}

char *__cdecl GetString(const char *PackageAndStringReference)
{
  EXT_Link<LocalizeEntry> *Current; // [esp+18h] [ebp-8h]
  char *value; // [esp+1Ch] [ebp-4h]

  value = 0;
  Sys_EnterCriticalSection(CRITSECT_STRINGED_COMMAND);
  for ( Current = overrideLocalizedEntryList.m_Head; Current; Current = Current->m_Next )
  {
    if ( !strcmp(Current->m_Data->name, PackageAndStringReference) )
    {
      value = (char *)Current->m_Data->value;
      break;
    }
  }
  Sys_LeaveCriticalSection(CRITSECT_STRINGED_COMMAND);
  return value;
}

EXT_Link<LocalizeEntry> *__thiscall EXT_List<LocalizeEntry>::Add(
        EXT_List<LocalizeEntry> *this,
        LocalizeEntry *m_Data,
        bool First)
{
  EXT_Link<LocalizeEntry> *v4; // [esp+0h] [ebp-10h]
  EXT_Link<LocalizeEntry> *v6; // [esp+8h] [ebp-8h]

  v6 = (EXT_Link<LocalizeEntry> *)operator new(0xCu);
  if ( v6 )
  {
    v6->m_Next = 0;
    v6->m_Last = 0;
    v6->m_Data = 0;
    v4 = v6;
  }
  else
  {
    v4 = 0;
  }
  if ( !v4 )
    return 0;
  v4->m_Data = m_Data;
  if ( this->m_Head && this->m_Tail )
  {
    if ( First )
    {
      this->m_Head->m_Last = v4;
      v4->m_Next = this->m_Head;
      this->m_Head = v4;
    }
    else
    {
      this->m_Tail->m_Next = v4;
      v4->m_Last = this->m_Tail;
      this->m_Tail = v4;
    }
  }
  else
  {
    this->m_Head = v4;
    this->m_Tail = v4;
  }
  ++this->m_Count;
  return v4;
}

