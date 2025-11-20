#pragma once

MatchMakingInfo *__thiscall MatchMakingInfo::MatchMakingInfo(MatchMakingInfo *this);
void __thiscall MatchMakingInfo::serialize(MatchMakingInfo *this, bdByteBuffer *buffer);
bool __thiscall MatchMakingInfo::deserialize(MatchMakingInfo *this, bdReference<bdByteBuffer> buffer);
unsigned int __thiscall MatchMakingInfo::sizeOf(MatchMakingInfo *this);
char __thiscall MatchMakingInfo::setSessionKey(MatchMakingInfo *this, const bdSecurityKey *secKey);
char __thiscall MatchMakingInfo::getSessionKey(MatchMakingInfo *this, bdSecurityKey *secKey);
bool __thiscall MatchMakingInfo::doUpdate(
        MatchMakingInfo *this,
        int msec,
        unsigned int updateIntervalMsec,
        unsigned int forceUpdateIntervalMsec);
