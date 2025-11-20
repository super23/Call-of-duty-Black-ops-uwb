#pragma once

void __cdecl MSG_PacketAnalyze_SetPacketEntityType(
        SnapshotInfo_s *snapInfo,
        PacketEntityType packetEntityType,
        bool archived);
const char *__cdecl SV_GetEntityTypeString(unsigned int packetEntityType);
void __cdecl SV_PacketAnalyze_TrackSnapshotSize(int size);
void __cdecl SV_PacketAnalyze_TrackPacketCompression(unsigned int clientNum, int originalSize, int compressedSize);
void __cdecl SV_PacketAnalyze_TrackETypeBytes(
        int eType,
        int number,
        int bits,
        bool archived,
        const NetFieldList *fieldList);
void __cdecl SV_PacketAnalyze_TrackPS_ClearBits(int bits);
void __cdecl SV_PacketAnalyze_TrackPS_FieldDeltasBits(int bits);
void __cdecl SV_PacketAnalyze_TrackPS_HudelemBits(int bits);
void __cdecl SV_PacketAnalyze_TrackPS_StatsBits(int bits);
void __cdecl SV_PacketAnalyze_TrackPS_WeaponBits(int bits);
void __cdecl SV_PacketAnalyze_TrackPS_AmmoPoolBits(int bits);
void __cdecl SV_PacketAnalyze_TrackPS_AmmoClipBits(int bits);
void __cdecl SV_PacketAnalyze_TrackPS_ObjectivesBits(int bits);
