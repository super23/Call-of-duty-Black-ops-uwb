#pragma once

void __cdecl EntHandle::Init();
void __cdecl EntHandle::Shutdown();
void __cdecl SentientHandle::Init();
void __cdecl EntHandleDissociate(gentity_s *ent);
void __cdecl EntHandleDissociateInternal(EntHandleList *entHandleList);
void __cdecl SentientHandleDissociate(sentient_s *sentient);
void __thiscall EntHandle::setEnt(EntHandle *this, gentity_s *ent);
bool __thiscall EntHandle::isDefined(EntHandle *this);
gentity_s *__thiscall EntHandle::ent(EntHandle *this);
void __cdecl RemoveEntHandleInfo(EntHandleList *entHandleList, unsigned int oldInfoIndex);
unsigned int __cdecl AddEntHandleInfo(EntHandleList *entHandleList, void *handle);
void __thiscall SentientHandle::setSentient(SentientHandle *this, sentient_s *sentient);
bool __thiscall SentientHandle::isDefined(SentientHandle *this);
sentient_s *__thiscall SentientHandle::sentient(SentientHandle *this);
