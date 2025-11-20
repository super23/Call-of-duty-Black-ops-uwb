#pragma once

PublicProfileInfo *__thiscall PublicProfileInfo::PublicProfileInfo(PublicProfileInfo *this);
void __thiscall PublicProfileInfo::serialize(PublicProfileInfo *this, bdByteBuffer *buffer);
bool __thiscall PublicProfileInfo::deserialize(PublicProfileInfo *this, bdReference<bdByteBuffer> buffer);
unsigned int __thiscall PublicProfileInfo::sizeOf(PublicProfileInfo *this);
PrivateProfileInfo *__thiscall PrivateProfileInfo::PrivateProfileInfo(PrivateProfileInfo *this);
void __thiscall PrivateProfileInfo::serialize(PrivateProfileInfo *this, bdByteBuffer *buffer);
bool __thiscall PrivateProfileInfo::deserialize(PrivateProfileInfo *this, bdReference<bdByteBuffer> buffer);
unsigned int __thiscall PrivateProfileInfo::sizeOf(PrivateProfileInfo *this);
