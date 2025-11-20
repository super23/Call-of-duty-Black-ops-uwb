#include "MatchMakingQueries_win32.h"

MatchMaking_PC_QUICKMATCH_Query *__thiscall MatchMaking_PC_QUICKMATCH_Query::MatchMaking_PC_QUICKMATCH_Query(
        MatchMaking_PC_QUICKMATCH_Query *this)
{
  bdSessionParams::bdSessionParams(this);
  this->__vftable = (MatchMaking_PC_QUICKMATCH_Query_vtbl *)&MatchMaking_PC_QUICKMATCH_Query::`vftable';
  return this;
}

void __thiscall MatchMaking_PC_QUICKMATCH_Query::serialize(MatchMaking_PC_QUICKMATCH_Query *this, bdByteBuffer *buffer)
{
  bdHashMap<bdReference<bdCommonAddr>,bdArray<bdQoSProbe::bdQoSProbeEntryWrapper>,bdHashingClass>::releaseIterator((int)buffer);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_SERVERTYPE);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_NETCODE_VERSION);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_MAXPLAYERS);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_PLAYLIST);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_PLAYLIST_VERSION);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_LICENSE);
}

MatchMaking_PC_WAGER_Query *__thiscall MatchMaking_PC_WAGER_Query::MatchMaking_PC_WAGER_Query(
        MatchMaking_PC_WAGER_Query *this)
{
  bdSessionParams::bdSessionParams(this);
  this->__vftable = (MatchMaking_PC_WAGER_Query_vtbl *)&MatchMaking_PC_WAGER_Query::`vftable';
  return this;
}

void __thiscall MatchMaking_PC_WAGER_Query::serialize(MatchMaking_PC_WAGER_Query *this, bdByteBuffer *buffer)
{
  bdHashMap<bdReference<bdCommonAddr>,bdArray<bdQoSProbe::bdQoSProbeEntryWrapper>,bdHashingClass>::releaseIterator((int)buffer);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_SERVERTYPE);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_LICENSE);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_PLAYLIST_MIN);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_PLAYLIST_MAX);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_NETCODE_VERSION);
  bdByteBuffer::writeInt32(buffer, this->m_memberPARAM_PLAYLIST_VERSION);
}

MatchMaking_PC_GEO_Query *__thiscall MatchMaking_PC_GEO_Query::MatchMaking_PC_GEO_Query(MatchMaking_PC_GEO_Query *this)
{
  bdSessionParams::bdSessionParams(this);
  this->__vftable = (MatchMaking_PC_GEO_Query_vtbl *)&MatchMaking_PC_GEO_Query::`vftable';
  return this;
}

void __thiscall MatchMaking_PC_GEO_Query::serialize(MatchMaking_PC_GEO_Query *this, bdByteBuffer *buffer)
{
  bdHashMap<bdReference<bdCommonAddr>,bdArray<bdQoSProbe::bdQoSProbeEntryWrapper>,bdHashingClass>::releaseIterator((int)buffer);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_NETCODE_VERSION);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_GAMETYPE);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_SERVERTYPE);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_DDL_VERSION);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_RANKED);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_LICENSE);
  bdByteBuffer::writeUInt32(buffer, this->m_memberPARAM_GEO);
}

